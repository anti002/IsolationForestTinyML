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
    std::vector<float> l_d, u_d;
    bool leaf = false;
    float split_time, split_location;
    short int id, parent_id, child_left_id, child_right_id, split_dimension, size;
};

std::vector<std::vector<Node>> Forest;
std::vector<Node> Tree;


/*
    Helper methods and variables
*/
unsigned int node_counter;
char data_dim;
const char NUMBER_OF_TREES = 100;
const short int SAMPLE_SIZE = 256;
const float LIFE_TIME = 1;

/*
    Takes a .CSV file and parses the rows into a vector of floats
    path has to be changed for this version of the code
*/
std::vector<std::vector<float>> parseCSV()
{   
    std::vector<std::vector<float>> parsedCsv;
    std::ifstream data("C:\\Users\\anton\\OneDrive\\Skrivbord\\Thesis_Code\\IsolationForestTinyML\\DatSets\\pima.csv");
    std::string line;
    while(std::getline(data,line))
    {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<float> parsedRow;
        while(std::getline(lineStream,cell,','))
        {
            parsedRow.emplace_back(std::stof(cell));
        }

        parsedCsv.emplace_back(parsedRow);
    }
    return parsedCsv;
};

/*
    Method for taking random unique sub samples from data set
*/
std::vector<std::vector<float>> random_unique(const std::vector<std::vector<float>> & data, int sample)
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

/*
    Method for cleaning the data set form identical instances
*/
std::vector<std::vector<float>> cleanDataset(std::vector<std::vector<float>> & data)
{
    for (size_t i = 0; i < data.size(); i++)
    {

        for (size_t j = 0; j < data.size(); j++)
        {
            if(i!=j)
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

/*
    Method for finding the child of a node based in the id of the child
*/
Node findChild(const std::vector<Node> & tree, int child_id)
{
    
    for (size_t i = 0; i < tree.size(); i++)
    {
        if (tree[i].id == child_id)
        {
            return tree[i];
        }
    }  
}

/*
    Method for getting a random value from the exponential distribution with the rate of lambda
*/
float exp_dist(float lamda)
{
    std::default_random_engine generator;
    std::exponential_distribution<double> distribution(lamda);
    return distribution(generator);
}

/*
    Method for getting a random dimension with the probability based on upper and lower value of the dimensions
*/
int get_dim(Node & node, float lamda)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    float pick = dis(gen);
    
    float limit = 0;

    for (size_t i = 0; i < node.l_d.size(); i++)
    {
        limit += (node.u_d[i] - node.l_d[i]) / lamda;
        if (limit > pick)
        {
            return i;
        }
    }
}

/*
    Method for getting a random split location
*/
float get_split_location(float lower, float upper)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(lower, upper);
    return dis(gen);;
}

/*
    Method for calculating the c(n)
*/
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

/*
    Method for finding the location in the tree of the child node based on if we look for the left or right child
*/
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

/*
    Method for calculating path length
*/
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
            int length = 0;
            
            while (length == 0 || !tree[current_node_pos].leaf)
            {
                float splitValue_attribute = parsedCsv[i][tree[current_node_pos].split_dimension];
                float splitValue_node = tree[current_node_pos].split_location;
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
void SampleMondrianBlock(Node & j, float life_time, float t_parent)
{
    //Line 1
    //Line 2
    float lamda = 0;
    float min = 0;
    float max = 0;
    j.size = j.data.size();

    j.u_d.reserve(j.data[0].size());
    j.l_d.reserve(j.data[0].size());
    
    for (size_t i = 0; i < data_dim; i++)
    {
        for (size_t k = 0; k < j.data.size(); k++)
        {
            
            if (j.data[k][i] < min)
            {
                min = j.data[k][i];
            }

            if (j.data[k][i] > max)
            {
                max = j.data[k][i];
            }

        }
        lamda += (max - min);
        j.l_d.emplace_back(min);
        j.u_d.emplace_back(max);
    }
    //Line 3

    float E = exp_dist(lamda);
    
    //Line 4
    if(j.data.size() > 1)
    {
        //Line 5
        j.split_time = t_parent + E;
    
        //Line 6
        int sample_dimension = get_dim(j, lamda);
        //Line 7

        float upper = j.data[0][sample_dimension];
        float lower = j.data[0][sample_dimension];

        while (upper == lower)
        {
            sample_dimension = get_dim(j, lamda);
            upper = j.data[0][sample_dimension];
            lower = j.data[0][sample_dimension];
            for (size_t i = 0; i < j.data.size(); i++)
            {
                if (j.data[i][sample_dimension] < lower)
                {
                    lower = j.data[i][sample_dimension];
                }

                if (j.data[i][sample_dimension] > upper)
                {
                    upper = j.data[i][sample_dimension];
                }
            }
        }
        float split_location = get_split_location(lower, upper); 


        //Line 8
        
        
        //Save node attributes
        j.split_dimension = sample_dimension;
        j.split_location = split_location;
        
        Tree.emplace_back(j);
        
        //Line 9
        Node left_child;
        node_counter++;
        left_child.id = node_counter;
        j.child_left_id = left_child.id;

        for (size_t i = 0; i < j.data.size(); i++)
        {
            if(j.data[i][sample_dimension] <= split_location)
            {
                left_child.data.emplace_back(j.data[i]);
            }
        }
        left_child.parent_id = j.id;
        Tree[left_child.parent_id].child_left_id = left_child.id;
        
        SampleMondrianBlock(left_child, life_time, j.split_time);

        //Line10
        Node right_child;
        node_counter++;
        right_child.id = node_counter;
        j.child_right_id = right_child.id;
        
        for (size_t i = 0; i < j.data.size(); i++)
        {
            if(j.data[i][sample_dimension] > split_location)
            {
                right_child.data.emplace_back(j.data[i]);
            }
        }

        right_child.parent_id = j.id;
        Tree[right_child.parent_id].child_right_id = right_child.id;
        SampleMondrianBlock(right_child, life_time, j.split_time);
        std::vector<std::vector<float>>().swap(j.data);
        std::vector<std::vector<float>>().swap(left_child.data);
        std::vector<std::vector<float>>().swap(right_child.data);
        
    }
    else
    {
        //Line 12
        j.split_time = life_time;
        j.leaf = true;
        j.child_left_id = -1;
        j.child_right_id = -1;
        j.id = node_counter;
        Tree.emplace_back(j);
    }
}

//Algorithm 1
void SampleMondrianTree(float life_time, std::vector<std::vector<float>> data)
{
    if (data.size() > SAMPLE_SIZE)
    {
        data = random_unique(data, SAMPLE_SIZE);
    }
    Node root;
    node_counter = 0;
    root.id = node_counter;
    root.data = data;
    root.split_time = 0;
    data_dim = data[0].size();
    SampleMondrianBlock(root, life_time, root.split_time);
}

//Algorithm 4
void ExtendMondrianBlock(float life_time, Node node, const std::vector<float> & instance, float t_parent)
{
    //Line 1
    float e_l = 0;
    float e_u = 0;

    for (size_t i = 0; i < instance.size(); i++)
    {
        float min = 0;
        float max = 0;
        if ((node.l_d[i] - instance[i]) > 0)
        {
            min = node.l_d[i] - instance[i];
        }

        if ((instance[i] - node.u_d[i]) > 0)
        {
            max  = instance[i] - node.u_d[i];
        }
        
        e_l += min;
        e_u += max;
    }
    
    //Line 2
    float lamda = e_l + e_u;
    float E = exp_dist(lamda);
    
    if(t_parent + E < node.split_time)
    {
        //Line 4
        int split_dimension = get_dim(node, lamda);
        //Line 5
        std::random_device rd;
        std::mt19937 gen(rd());
        float split_location;
        if (instance[node.split_dimension] > node.u_d[node.split_dimension])
        {
            split_location = get_split_location(node.u_d[node.split_dimension], instance[node.split_dimension]);
        }
        else
        {
            split_location = get_split_location(instance[node.split_dimension], node.l_d[node.split_dimension]);
        }
        
        //Line 6
        Node newParent;
        node_counter++;
        newParent.id = node_counter;

        newParent.parent_id = node.parent_id;
        Tree[node.id].parent_id = newParent.id;

        newParent.split_dimension = split_dimension;
        newParent.split_location = split_location;
        newParent.split_time = t_parent + E;

        Tree[node.child_left_id].parent_id = newParent.id;
        Tree[node.child_right_id].parent_id = newParent.id;
        
        for (size_t i = 0; i < instance.size(); i++)
            {
                if (node.l_d[i] < instance[i])
                {
                    newParent.l_d.emplace_back(node.l_d[i]);
                }
                else
                {
                    newParent.l_d.emplace_back(instance[i]);
                }

                if (node.u_d[i] > instance[i])
                {
                    newParent.u_d.emplace_back(node.u_d[i]);
                }
                else
                {
                    newParent.u_d.emplace_back(instance[i]);
                }
            }

        Node newLeaf;
        node_counter++;
        newLeaf.id = node_counter;
        newLeaf.leaf = true;
        newLeaf.parent_id = newParent.id;

        newLeaf.data.emplace_back(instance);

        if (instance[newParent.split_dimension] < newParent.split_location)
        {
            newParent.child_left_id = node.id;
            newParent.child_right_id = newLeaf.id;

            Tree[newParent.parent_id].child_left_id = newParent.id;

            Tree.emplace_back(newParent);
            SampleMondrianBlock(newLeaf, life_time, t_parent);
        }
        else
        {
            newParent.child_right_id = node.id;
            newParent.child_left_id = newLeaf.id;

            Tree[newParent.parent_id].child_right_id = newParent.id;

            Tree.emplace_back(newParent);
            SampleMondrianBlock(newLeaf, life_time, t_parent);
        }
    }
    else
    {
        //Line 11
        float min = *min_element(instance.begin(), instance.end());
        float max = *max_element(instance.begin(), instance.end());
        for (size_t i = 0; i < node.l_d.size(); i++)
        {
            if (node.l_d[i] > min)
            {
                node.l_d[i] = min;
            }

            if (node.u_d[i] < max)
            {
                node.u_d[i] = max;
            }           
        }
        //Line 12
        if (node.leaf)
        {
            
        }
        else
        {
            //Line 13
            if (instance[node.split_dimension] <= node.split_location)
            {
                //Line 14
                ExtendMondrianBlock(life_time, findChild(Tree, node.child_left_id), instance, node.split_time);
            }
            else
            {
                //Line 14
                ExtendMondrianBlock(life_time, findChild(Tree, node.child_right_id), instance, node.split_time);
            }
        }   
    }
}

//Algorithm 3
void ExtendMondrianTree(float life_time, std::vector<float> & instance)
{
    ExtendMondrianBlock(life_time, Tree[0], instance, 0);
}

int main()
{

    std::vector<std::vector<float>> data;
    std::vector<std::vector<float>> parsedCsv = parseCSV();
    parsedCsv = cleanDataset(parsedCsv);
    
    int testparam = 0;
    int n_tests = 10;
    float scores[parsedCsv.size()] = {0};
    
    while (testparam < n_tests)
    {
        srand(time(0));
        for (size_t i = 0; i < NUMBER_OF_TREES; i++)
        {
            SampleMondrianTree(LIFE_TIME, parsedCsv);
            Forest.emplace_back(Tree);
            std::vector<Node>().swap(Tree);
        }

        std::cout << testparam + 1 << " Forests built" << std::endl;

        std::vector<float> anomalyScores = decision_function(Forest, parsedCsv);
        for (size_t i = 0; i < anomalyScores.size(); i++)
        {
            scores[i] += anomalyScores[i]/n_tests;
        }
        
        std::cout << testparam + 1 << " Scores saved" << std::endl;
        testparam++;
        std::vector<std::vector<Node>>().swap(Forest);
    }
    
    for (size_t i = 0; i < parsedCsv.size(); i++)
    {
        std::cout << scores[i] << std::endl;
    }

    /*for (size_t i = 0; i < Tree.size(); i++)
    {
        std::cout << "Node id: " << Tree[i].id 
                  << " Left child id: " << Tree[i].child_left_id 
                  << " Right child id: " << Tree[i].child_right_id 
                  << " Parent id: " << Tree[i].parent_id 
                  << " Leaf: " << Tree[i].leaf 
                  << " Data size " << Tree[i].data.size()
                  << " Split dim: " << Tree[i].split_dimension 
                  << " Split value: " << Tree[i].split_location 
                  << " Split time: " << Tree[i].split_time
                  << std::endl;
    }*/
}