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

class iForest 
{
    public:
    int t_trees, sample_size;
    iForest (int t_trees, int sample_size)
    {
        this->t_trees = t_trees;
        this->sample_size = sample_size;
    }

    std::vector<std::vector<Node>> fit(std::vector<std::vector<float>> dataSet)
    {
        std::vector<std::vector<Node>> iForest;
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
            iForest.push_back(node_stack);
            current_node_id = 0;
            int sum = 0;
            std::vector<Node>().swap(node_stack);
        }
        return iForest;
    };    
};

std::vector<float> path_length(std::vector<std::vector<Node>> forest, std::vector<std::vector<float>> parsedCsv)
{
    std::vector<float> edges;
    for (size_t i = 0; i < parsedCsv.size(); i++)
    {
        float avg = 0;
        for (size_t j = 0; j < forest.size(); j++)
        {
            int temp_id = forest[j].size()-1;
            
            while (!forest[j][temp_id].isLeaf)
            {
                int q_value = forest[j][temp_id].decision_node.q_value;
                float splitValue_data_set = parsedCsv[i][q_value];
                float splitValue_node = forest[j][temp_id].decision_node.x_value;
                
                if (splitValue_data_set < splitValue_node)
                {
                    int key = forest[j][temp_id].decision_node.child_left;
                    for (size_t k = 0; k < forest[j].size(); k++)
                    {
                        if (forest[j][k].node_id == key)
                        {
                            temp_id = k;
                            break;
                        }
                    }
                }
                else
                {
                    int key = forest[j][temp_id].decision_node.child_right;
                    for (size_t l = 0; l < forest[j].size(); l++)
                    {
                        if (forest[j][l].node_id == key)
                        {
                            temp_id = l;
                            break;
                        }
                    }
                }
            }
            float leaf_size = forest[j][temp_id].leaf.size;
            float path_length = forest[j][temp_id].leaf.depth + c(leaf_size);
            avg += path_length;
            //std::cout << path_length << std::endl;
        }
        float average_path = avg/forest.size();
        edges.push_back(average_path);
    }
    return edges;
}


std::vector<float> decision_function(std::vector<std::vector<Node>> forest, std::vector<std::vector<float>> parsedCsv)
{
    std::vector<float> scores;
    std::vector<float> average_length = path_length(forest, parsedCsv);
    for (size_t i = 0; i < average_length.size(); i ++)
    {
        //std::cout << average_length[i] << std::endl;
        float scorep =  0.5 - pow(2, (-1 * average_length[i])/c(parsedCsv.size()));
        //std::cout << scorep << std::endl;
        scores.push_back(scorep);
    }
    return scores;
}


int main()
{
    std::vector<std::vector<float>> parsedCsv = parseCSV();
    
    iForest clf = iForest(100, 256);
    std::vector<std::vector<Node>> estimators = clf.fit(parsedCsv);
    
    std::vector<float> scores = decision_function(estimators, parsedCsv);

    float total = 0;
    for (size_t i = 0; i < scores.size(); i++)
    {
        std::cout << scores[i] << std::endl;
    }
    //std::cout << total << std::endl;
}