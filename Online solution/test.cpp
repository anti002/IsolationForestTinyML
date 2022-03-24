#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct DecisionNode
{
    int child_left;
    int child_right;
    std::vector<std::vector<float>> data_left;
    std::vector<std::vector<float>> data_right;
    int q_value;
    float x_value;
};

struct Leaf
{
    int size;
    int depth;
    int child_left;
    int child_right;
};

struct Node
{
    Leaf leaf;
    DecisionNode decision_node;
    bool isLeaf;
    int node_id;
};

std::vector<Node> node_stack;
int current_node_id = 0;
const int NUMBER_OF_TREES = 100;
const int SAMPLE_SIZE = 256;

float c(float size)
{
    if (size > 2)
    {
        float temp = (2 * (log(size -1) + 0.5772156649)) - (2*(size-1)/size);
        return temp;
    }
    if (size == 2)
    {
        return 1;
    }
    return 0;
}


/*
    Takes a .CSV file and parses the rows into a vector of floats
    path has to be changed for this version of the code
*/
std::vector<std::vector<float>> parseCSV()
{   
    std::vector<std::vector<float>> parsedCsv;
    std::ifstream data("C:\\Users\\anton\\OneDrive\\Skrivbord\\Thesis_Code\\IsolationForestTinyML\\DatSets\\test.csv");
    std::string line;
    while(std::getline(data,line))
    {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<float> parsedRow;
        while(std::getline(lineStream,cell,','))
        {
            parsedRow.push_back(std::stof(cell));
        }

        parsedCsv.push_back(parsedRow);
    }
    return parsedCsv;
};

class iTree
{
    public:
    int current_height, height_limit, id;
    std::vector<std::vector<float>> data_left;
    std::vector<std::vector<float>> data_right;

    iTree (int current_height, int height_limit)
    {
        this->current_height = current_height;
        this->height_limit = height_limit;
        this->id = current_node_id;
    }
    
    Node fit(std::vector<std::vector<float>> sub_sample)
    {
        Node current_node;
        if (current_height >= height_limit || sub_sample.size() <= 1)
        {   
            current_node.isLeaf = true;
            Leaf leaf;
            current_node.node_id = current_node_id;
            current_node_id++;
            leaf.child_left = 0;
            leaf.child_right = 0;
            leaf.size = sub_sample.size();
            leaf.depth = this->current_height;
            current_node.leaf = leaf;  
            node_stack.push_back(current_node);      
            return current_node;
        }
        else
        {
            current_node.isLeaf = false;
            DecisionNode decision_node;
            decision_node.q_value = rand() % sub_sample[0].size();
            decision_node.x_value = sub_sample[rand() % sub_sample.size()][decision_node.q_value];
            current_node.decision_node = decision_node;
            current_node.node_id = current_node_id;
            current_node_id++;
            
            //split the data
            for (size_t i = 0; i < sub_sample.size(); i++)
            {
                if (sub_sample[i][decision_node.q_value] < decision_node.x_value)
                {
                    this->data_left.push_back(sub_sample[i]);
                }
                else
                {
                    this->data_right.push_back(sub_sample[i]);
                }
            }

            iTree left_subtree = iTree(this->current_height + 1, this->height_limit);
            iTree right_subtree = iTree(this->current_height + 1, this->height_limit);
                       
            Node left_node = left_subtree.fit(data_left);
            Node right_node = right_subtree.fit(data_right);

            current_node.decision_node.child_left = left_node.node_id;
            current_node.decision_node.child_right = right_node.node_id;

            node_stack.push_back(current_node);
            return current_node;
        }
    };
};

/*
    Methood used for taking a random sub sample from the data set
*/
template<class BidiIter >
BidiIter random_unique(BidiIter begin, BidiIter end, size_t num_random) {
    size_t left = std::distance(begin, end);
    while (num_random--) {
        BidiIter r = begin;
        std::advance(r, rand()%left);
        std::swap(*begin, *r);
        ++begin;
        --left;
    }
    return begin;
}

int next_tree_id_left(std::vector<Node> tree, int temp_id)
{
    int key = tree[temp_id].decision_node.child_left;
    for (size_t k = 0; k < tree.size(); k++)
    {
        if (tree[k].node_id == key)
        {
            return k;
        }
    }
}

int next_tree_id_right(std::vector<Node> tree, int temp_id)
{
    int key = tree[temp_id].decision_node.child_right;
    for (size_t k = 0; k < tree.size(); k++)
    {
        if (tree[k].node_id == key)
        {
            return k;
        }
    }
}

std::vector<float> path_length(std::vector<Node> tree, std::vector<std::vector<float>> parsedCsv)
{
    std::vector<float> edges;   

    for (size_t i = 0; i < parsedCsv.size(); i++)
    {
        float avg = 0;
        int temp_id = tree.size()-1;
        while (!tree[temp_id].isLeaf)
        {
            int q_value = tree[temp_id].decision_node.q_value;
            float splitValue_data_set = parsedCsv[i][q_value];
            float splitValue_node = tree[temp_id].decision_node.x_value;
            
            if (splitValue_data_set < splitValue_node)
            {
                temp_id = next_tree_id_left(tree, temp_id);
            }
            else
            {
                temp_id = next_tree_id_right(tree, temp_id);
            }
        }
        avg += tree[temp_id].leaf.depth + c(tree[temp_id].leaf.size);
        edges.push_back(avg);
    }
    return edges;
}


std::vector<float> decision_function(std::vector<float> average_length, std::vector<std::vector<float>> parsedCsv)
{
    std::vector<float> scores;
    for (size_t i = 0; i < average_length.size(); i++)
    {
        scores.push_back(0.5 - pow(2, (-1 * average_length[i])/c(parsedCsv.size())));
    }
    return scores;
}

class iForest 
{
    public:
    int t_trees, sample_size;
    std::vector<float> anomaly_scores;
    iForest (int t_trees, int sample_size)
    {
        this->t_trees = t_trees;
        this->sample_size = sample_size;
    }

    void fit(std::vector<std::vector<float>> dataSet)
    {
        std::vector<std::vector<float>> dataSet_ = dataSet;
        std::vector<std::vector<float>> all_paths;
        std::vector<float> average_paths;
        int height_limit = (int)ceil(log2(this->sample_size));
        
        if (dataSet.size() < sample_size)
        {
            this->sample_size = dataSet.size();
        }
        for (size_t i = 0; i < t_trees; i++)
        {
            random_unique(dataSet.begin(), dataSet.end(), this->sample_size);
            
            iTree tree = iTree(0, height_limit);
            Node root = tree.fit(dataSet);
            all_paths.push_back(path_length(node_stack, dataSet_));
            current_node_id = 0;
            std::vector<Node>().swap(node_stack);
        }

        for (size_t i = 0; i < all_paths[0].size(); i++)
        {
            float total = 0;
            for (size_t j = 0; j < all_paths.size(); j++)
            {
                total += all_paths[j][i];
            }
            average_paths.push_back(total/all_paths.size());
        }
        anomaly_scores = decision_function(average_paths, dataSet_);
    };    
};

int main()
{
    std::vector<std::vector<float>> parsedCsv = parseCSV();
    
    iForest clf = iForest(NUMBER_OF_TREES, SAMPLE_SIZE);
    clf.fit(parsedCsv);
    for (size_t i = 0; i < clf.anomaly_scores.size(); i++)
    {
        std::cout << clf.anomaly_scores[i] << std::endl;
    }
}