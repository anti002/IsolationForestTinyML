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
    std::vector<std::vector<float>> data;

    //data for decision node
    std::vector<std::vector<float>> data_left;
    std::vector<std::vector<float>> data_right;
    int q_value;
    float x_value;
};



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
        this->root = {};
    }
    
    Node fit(std::vector<std::vector<float>> sub_sample)
    {
        if (current_height >= height_limit || sub_sample.size() <= 2 )
        {
            root.size = sub_sample.size();
            root.data = sub_sample;
            return root;
        }
        else
        {
            int q_value = rand() % sub_sample[0].size();
            double x_value = sub_sample[rand() % sub_sample.size()][q_value];

            for (size_t i = 0; i < sub_sample.size(); i++)
            {
                if (sub_sample[i][q_value] < x_value)
                {
                    this->data_left.push_back(sub_sample[i]);
                }
                else
                {
                    this->data_right.push_back(sub_sample[i]);
                }
            }
            
            iTree left_subtree = iTree(this->current_height + 1, this->current_height);
            iTree right_subtree = iTree(this->current_height + 1, this->current_height);

            left_subtree.fit(data_left);
            right_subtree.fit(data_right);

            root.data_left = this->data_left;
            root.data_right = this->data_right;
            root.q_value = q_value;
            root.x_value = x_value;

            return root;

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
    
    iForest forest = iForest(100, 256);

    forest.fit(parsedCsv);
    
}