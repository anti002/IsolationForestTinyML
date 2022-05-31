#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>

using namespace std;

struct Node
{
  std::vector<std::vector<float>> data;
  bool leaf = false;
  float split_val_node;
  unsigned int id, parent_id, child_left_id, child_right_id, split_feature_node, size;
};

std::vector<std::vector<Node>> Forest;

/*
    Helper methods and variables
*/
short int node_counter;
unsigned char data_dim;
const int SAMPLE_SIZE = 256;
unsigned char NUMBER_OF_TREES = 100;
std::vector<std::vector<float>> parsedCsv;
unsigned short int node_ammount = 0;

std::vector<std::vector<float>> parseCSV()
{   
    std::vector<std::vector<float>> parsedCsv;
    std::ifstream data("C:\\Users\\anton\\OneDrive\\Skrivbord\\Thesis_Code\\IsolationForestTinyML\\DatSets\\wine.csv");
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

std::vector<std::vector<float>> cleanDataset(std::vector<std::vector<float>>  data)
{
    for (size_t i = 0; i < data.size(); i++)
    {
        for (size_t j = 0; j < data.size(); j++)
        {
            if (i != j)
            {
                if (data[i] == data[j])
                {
                data.erase(data.begin() + j);
                }
            }
        }
    }
    return data;
}

std::vector<std::vector<float>> random_unique(std::vector<std::vector<float>> data, int sample)
{
    std::vector<std::vector<float>> sub_sample;
    for (size_t i = 0; i < sample; i++)
    {
        int random = rand() % data.size();
        sub_sample.push_back(data[random]);
        for (size_t j = 0; j < sub_sample.size(); j++)
        {
            if (sub_sample[j] == data[random] && j != i)
            {
                sub_sample.pop_back();
                i--;
            }
        }
    }
    return sub_sample;
}

float c(float size)
{
    if (size > 2)
    {
        float temp = (2. * (log(size -1.) + 0.5772156649)) - (2.*(size-1.)/size);
        return temp;
    }
    if (size == 2)
    {
        return 1;
    }
    return 0;
}

int find_node_pos(const std::vector<Node> & tree, const Node & current_node, bool left)
{
    for (size_t i = 0; i < tree.size(); i++)
    {
        if (tree[i].id == current_node.child_left_id && left)
        {
            return i;
        }

        if (tree[i].id == current_node.child_right_id && !left)
        {
            return i;
        } 
    }  
}

float get_split(std::vector<std::vector<float>> data, int q_value)
{
    std::vector<float> x_values;

    for (size_t i = 0; i < data.size(); i++)
    {
        x_values.push_back(data[i][q_value]);
    }

    double min = *min_element(x_values.begin(), x_values.end());
    double max = *max_element(x_values.begin(), x_values.end());

    float split_value = (rand() / ((double)RAND_MAX + 1) * (max - min) + min);
    
    return split_value;
}

std::vector<float> path_length(const std::vector<vector<Node>> & forest, const std::vector<std::vector<float>> & parsedCsv)
{
    std::vector<float> edges;
    for (size_t i = 0; i < parsedCsv.size(); i++)
    {
        float avg = 0;
        float avg_depth = 0;
        for (size_t j = 0; j < forest.size(); j++)
        {
            std::vector<Node> tree = forest[j];
            int current_node_pos = 0;
            char length = 0;
            
            while (length == 0 || !tree[current_node_pos].leaf)
            {
                float splitValue_attribute = parsedCsv[i][tree[current_node_pos].split_feature_node];
                float splitValue_node = tree[current_node_pos].split_val_node;
                if (splitValue_attribute < splitValue_node)
                {
                    current_node_pos = find_node_pos(tree, tree[current_node_pos], true);
                    length += 1;
                }
                else
                {
                    current_node_pos = find_node_pos(tree, tree[current_node_pos], false);
                    length += 1;
                }
            }
            
            float leaf_size = tree[current_node_pos].size;
            float path_length = length + c(leaf_size);
            
            avg += path_length;
        }

        float average_path = avg/forest.size();
        edges.emplace_back(average_path);
    }
    return edges;
}


std::vector<float> decision_function(const std::vector<vector<Node>> & forest, const std::vector<std::vector<float>> & parsedCsv)
{
    std::vector<float> scores;
    float score = 0;
    std::vector<float> average_length = path_length(forest, parsedCsv);
    for (size_t i = 0; i < parsedCsv.size(); i++)
    {
        if (parsedCsv.size() <= SAMPLE_SIZE)
        {
            scores.emplace_back(0.5 - pow(2, (-1 * average_length[i])/c(parsedCsv.size())));
        }
        else
        {
            scores.emplace_back(0.5 - pow(2, (-1 * average_length[i])/c(SAMPLE_SIZE)));
        }
    }
    return scores;
}


/*
    Mondrian implementation
*/
//Algorithm 2
void SampleMondrianBlock(std::vector<Node> & Tree, Node & j, int height, int height_limit)
{
    node_ammount++;
    j.size = j.data.size();

    if (j.data.size() > 1 && height <= height_limit)
    {
        j.split_feature_node = rand() % data_dim;

        float upper = j.data[0][j.split_feature_node];
        float lower = j.data[0][j.split_feature_node];
        
        while (upper == lower)
        {
            j.split_feature_node = rand() % data_dim;
            /*std::cout << "stuck!!! " 
            << "Feature: " << j.split_feature_node 
            << " Upper: " << upper
            << " Lower: " << lower << std::endl;*/
            upper = j.data[0][j.split_feature_node];
            lower = j.data[0][j.split_feature_node];
            for (size_t i = 0; i < j.data.size(); i++)
            {
                if (j.data[i][j.split_feature_node] < lower)
                {
                    lower = j.data[i][j.split_feature_node];
                }

                if (j.data[i][j.split_feature_node] > upper)
                {
                    upper = j.data[i][j.split_feature_node];
                }
            }
        }

        j.split_val_node = get_split(j.data, j.split_feature_node);

        Tree.emplace_back(j);

        //Line 9
        Node left_child;
        node_counter++;
        left_child.id = node_counter;
        j.child_left_id = left_child.id;

        std::vector<std::vector<float>> temp;
        for (size_t i = 0; i < j.data.size(); i++)
        {
            if (j.data[i][j.split_feature_node] <= j.split_val_node)
            {
                left_child.data.emplace_back(j.data[i]);
            }
        }

        for (size_t i = 0; i < j.data.size(); i++)
        {
            if (j.data[i][j.split_feature_node] > j.split_val_node)
            {
                temp.emplace_back(j.data[i]);
            }
        }

        std::vector<std::vector<float>>().swap(j.data);
        left_child.parent_id = j.id;
        Tree[left_child.parent_id].child_left_id = left_child.id;

        SampleMondrianBlock(Tree, left_child, height + 1, height_limit);
        std::vector<std::vector<float>>().swap(left_child.data);

        //Line10
        Node right_child;
        node_counter++;
        right_child.data = temp;
        std::vector<std::vector<float>>().swap(temp);
        right_child.id = node_counter;
        j.child_right_id = right_child.id;
        right_child.parent_id = j.id;
        Tree[right_child.parent_id].child_right_id = right_child.id;

        SampleMondrianBlock(Tree, right_child, height + 1, height_limit);
        std::vector<std::vector<float>>().swap(right_child.data);

    }
    else
    {
        //Line 12
        j.leaf = true;
        j.id = node_counter;
        j.child_left_id = -1;
        j.child_right_id = -1;
        Tree.emplace_back(j);
    }
}

//Algorithm 1
void SampleMondrianTree(std::vector<Node> & tree, std::vector<std::vector<float>> data)
{
    if (data.size() > SAMPLE_SIZE)
    {
        data = random_unique(data, SAMPLE_SIZE);
    }
    std::cout << data.size() << std::endl;
    data = cleanDataset(data);
    std::cout << data.size() << std::endl;
    Node root;
    node_counter = 0;
    root.id = node_counter;
    root.data = data;
    data_dim = data[0].size();
    SampleMondrianBlock(tree, root, 0, 8);
}

int main() 
{
    std::vector<std::vector<float>> parsedCsv = parseCSV();
    std::cout << parsedCsv.size() << std::endl;
    parsedCsv = cleanDataset(parsedCsv);
    std::cout << parsedCsv.size() << std::endl;

    for (size_t i = 0; i < NUMBER_OF_TREES; i++)
    {   
        std::cout << i << std::endl;
        Forest.emplace_back();
        SampleMondrianTree(Forest[i], parsedCsv);
    }
    
    /*for (size_t j = 0; j < Forest.size(); j++)
    {
        std::vector<Node> Tree = Forest[j];
        for (size_t i = 0; i < Tree.size(); i++)
        {
        std::cout << "Node id: " << Tree[i].id 
                  << " Left child id: " << Tree[i].child_left_id 
                  << " Right child id: " << Tree[i].child_right_id 
                  << " Leaf: " << Tree[i].leaf 
                  << " Data size " << Tree[i].data.size()
                  << std::endl;
        }
    }*/

    std::vector<float> anomalyScores = decision_function(Forest, parsedCsv);
    for (size_t i = 0; i < anomalyScores.size(); i++)
    {
        std::cout << anomalyScores[i] << std::endl;
    }
}
