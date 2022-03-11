#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct Node
{
    //data for leaf
    int size;
    int depth;
    int node_id;
    int child_left;
    int child_right;
    //data for decision node
    std::vector<std::vector<float>> data_left;
    std::vector<std::vector<float>> data_right;
    int q_value;
    float x_value;
};

struct Tree{
    int child_id_left, child_id_right;
    int feature;
    int n_samples;
    int depth;
    float threshold;
};

std::vector<Tree> iTree1;
int current_node_id = 0;

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
    int current_height, height_limit, n_nodes;
    Node root;
    std::vector<std::vector<float>> data_left;
    std::vector<std::vector<float>> data_right;

    iTree (int current_height, int height_limit)
    {
        this->current_height = current_height;
        this->height_limit = height_limit;
    }
    
    Node fit(std::vector<std::vector<float>> sub_sample)
    {
        if (current_height >= height_limit || sub_sample.size() <= 2 )
        {
            Node leaf;
            leaf.node_id = current_node_id;
            current_node_id++;
            leaf.child_left = 0;
            leaf.child_right = 0;
            leaf.q_value = -2;
            leaf.x_value = -2.0;
            leaf.size = sub_sample.size();
            leaf.depth = this->current_height;
            iTree1.push_back({leaf.child_left, leaf.child_right, leaf.q_value, (int)sub_sample.size(), leaf.depth, leaf.x_value});            
            return leaf;
        }
        else
        {
            Node internal_node;
            internal_node.node_id = current_node_id;
            current_node_id++;
            internal_node.q_value = rand() % sub_sample[0].size();
            internal_node.x_value = sub_sample[rand() % sub_sample.size()][internal_node.q_value];
            internal_node.depth = this->current_height;
            
            //split the data
            for (size_t i = 0; i < sub_sample.size(); i++)
            {
                if (sub_sample[i][internal_node.q_value] < internal_node.x_value)
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
            std::cout << "Fit ";
            std::cout << current_node_id << std::endl;
            Node right_node = right_subtree.fit(data_right);
            
            iTree1.push_back({left_node.node_id, right_node.node_id, internal_node.q_value, (int)sub_sample.size(), internal_node.depth, internal_node.x_value});            

            return internal_node;
        }
    };
};

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

    std::vector<iTree> fit(std::vector<std::vector<float>> dataSet)
    {
        std::vector<iTree> iForest;
        int height_limit = (int)ceil(log2(sample_size));
        int n_rows = dataSet.size();
        if (n_rows < sample_size)
        {
            this->sample_size = n_rows;
        }
        for (size_t i = 0; i < t_trees; i++)
        {
            random_unique(dataSet.begin(), dataSet.end(), this->sample_size);
            for (size_t i = dataSet.size(); i > this->sample_size ; i--)
            {
                dataSet.pop_back();
            }

            iTree tree = iTree(0, height_limit);
            tree.fit(dataSet);

            iForest.push_back(tree);
            
        }
        return iForest;
    };    
};

int main(){
    std::vector<std::vector<float>> parsedCsv = parseCSV();
    
    iForest clf = iForest(1, 256);
    int depthcount = 0;
    std::vector<iTree> estimators = clf.fit(parsedCsv);
    /*for (size_t i = iTree1.size() - 1; i > 0; i--)
    {   
        std::cout << "Node";
        std::cout << iTree1.size() - i;
        std::cout << ": ";
        std::cout << iTree1[i].child_id_left;
        std::cout << " ";
        std::cout << iTree1[i].child_id_right;
        std::cout << " ";
        std::cout << iTree1[i].feature;
        std::cout << " ";
        std::cout << iTree1[i].threshold;
        std::cout << " ";
        std::cout << iTree1[i].depth;
        std::cout << " ";
        std::cout << iTree1[i].n_samples << std::endl;
    }*/
}