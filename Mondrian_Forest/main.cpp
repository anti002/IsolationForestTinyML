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
    std::ifstream data("C:\\Users\\anton\\OneDrive\\Skrivbord\\Thesis_Code\\IsolationForestTinyML\\DatSets\\glass.csv");
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
    
    //Find and save the min and max values of the data block
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

        //Ensures that the upper and lower values of the features are different.
        //The algorithm can't pick a split location if the upper and lower values are identical
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
        //Create child nodes and give them all the attributes/data needed
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
        //Empty non necessary data
        std::vector<std::vector<float>>().swap(j.data);
        std::vector<std::vector<float>>().swap(left_child.data);
        std::vector<std::vector<float>>().swap(right_child.data);
        
    }
    else
    {
        //Line 12
        //Create the leaf node and give it's attributes
        j.split_time = life_time;
        j.leaf = true;
        j.child_left_id = -1;
        j.child_right_id = -1;
        j.id = node_counter;
        j.split_dimension = get_dim(j, lamda);
        Tree.emplace_back(j);
    }
}

//Algorithm 1
void SampleMondrianTree(float life_time, std::vector<std::vector<float>> data)
{
    //Get the sub sample
    if (data.size() > SAMPLE_SIZE)
    {
        data = random_unique(data, SAMPLE_SIZE);
    }
    //Create root node and give initial values
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
    //Get the values needed for the exponential distribution rate
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
    
    //Check if the node shall be inserted in the tree
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
        //Create the new node and rearrange all the id's needed for it to be inserted correctly
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
        
        //Update the upper and lower bounds of the current node
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

        //Create the new leaf that has to be the sibling to the inserted node
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
    for (size_t i = 0; i < Forest.size(); i++)
    {
        Tree = Forest[i];
   
        ExtendMondrianBlock(life_time, Tree[0], instance, 0);
    }
}

//Everything in the main() function is written for different tests
int main()
{std::vector<std::vector<float>> dataSet;
    dataSet.push_back({-1.29,4.97,-91.27});
dataSet.push_back({-1.67,4.91,-90.47});
dataSet.push_back({-0.98,5.35,-89.99});
dataSet.push_back({-0.84,5.25,-91.32});
dataSet.push_back({-0.84,5.00,-91.22});
dataSet.push_back({0.82,4.24,-90.14});
dataSet.push_back({2.15,4.79,-89.50});
dataSet.push_back({1.35,4.35,-88.59});
dataSet.push_back({0.72,3.67,-87.79});
dataSet.push_back({-0.62,4.25,-87.08});
dataSet.push_back({3.34,4.70,-89.29});
dataSet.push_back({6.76,4.37,-88.12});
dataSet.push_back({12.43,4.31,-83.97});
dataSet.push_back({17.07,4.63,-81.30});
dataSet.push_back({20.81,5.80,-76.87});
dataSet.push_back({24.16,6.30,-72.77});
dataSet.push_back({26.33,6.98,-66.88});
dataSet.push_back({28.66,7.12,-60.50});
dataSet.push_back({29.50,8.50,-55.35});
dataSet.push_back({28.55,8.97,-48.03});
dataSet.push_back({25.42,9.72,-39.95});
dataSet.push_back({18.59,9.34,-31.18});
dataSet.push_back({12.62,9.91,-29.03});
dataSet.push_back({9.79,8.69,-27.95});
dataSet.push_back({6.24,8.74,-27.04});
dataSet.push_back({-0.95,8.90,-25.90});
dataSet.push_back({-9.30,9.29,-26.50});
dataSet.push_back({-14.23,9.83,-26.76});
dataSet.push_back({-14.31,10.75,-25.84});
dataSet.push_back({-15.36,10.67,-25.83});
dataSet.push_back({-13.94,10.06,-26.46});
dataSet.push_back({-11.54,9.48,-25.74});
dataSet.push_back({-8.07,9.25,-25.81});
dataSet.push_back({-3.39,8.78,-25.12});
dataSet.push_back({2.81,8.19,-24.45});
dataSet.push_back({9.30,9.06,-26.23});
dataSet.push_back({14.69,8.73,-29.06});
dataSet.push_back({18.54,8.34,-31.71});
dataSet.push_back({22.17,8.34,-35.19});
dataSet.push_back({24.79,8.18,-37.94});
dataSet.push_back({26.98,9.12,-40.95});
dataSet.push_back({28.94,9.18,-44.81});
dataSet.push_back({29.85,7.84,-48.84});
dataSet.push_back({30.20,6.60,-54.48});
dataSet.push_back({28.61,6.58,-63.38});
dataSet.push_back({25.32,6.52,-70.79});
dataSet.push_back({21.07,6.01,-76.64});
dataSet.push_back({16.78,5.93,-78.58});
dataSet.push_back({13.39,6.46,-82.78});
dataSet.push_back({10.95,6.88,-84.23});
dataSet.push_back({7.59,6.56,-85.61});
dataSet.push_back({5.04,7.31,-86.15});
dataSet.push_back({2.32,7.54,-86.30});
dataSet.push_back({0.24,7.92,-88.76});
dataSet.push_back({-1.12,8.04,-87.49});
dataSet.push_back({-1.82,8.39,-87.43});
dataSet.push_back({-0.56,8.15,-87.18});
dataSet.push_back({1.51,7.46,-86.88});
dataSet.push_back({3.54,6.45,-86.49});
dataSet.push_back({6.45,5.99,-85.11});
dataSet.push_back({9.77,5.65,-83.34});
dataSet.push_back({12.65,5.24,-81.45});
dataSet.push_back({16.26,4.97,-78.34});
dataSet.push_back({19.17,4.46,-76.15});
dataSet.push_back({21.69,4.85,-71.61});
dataSet.push_back({23.75,4.08,-68.43});
dataSet.push_back({25.10,2.72,-63.85});
dataSet.push_back({26.18,1.66,-60.41});
dataSet.push_back({25.70,0.73,-57.25});
dataSet.push_back({25.67,1.67,-55.82});
dataSet.push_back({26.89,3.41,-56.38});
dataSet.push_back({28.72,6.20,-54.68});
dataSet.push_back({30.91,10.41,-53.45});
dataSet.push_back({31.92,15.22,-51.33});
dataSet.push_back({32.62,21.07,-51.78});
dataSet.push_back({31.80,28.59,-51.43});
dataSet.push_back({30.00,34.58,-54.02});
dataSet.push_back({27.03,39.65,-55.93});
dataSet.push_back({22.99,44.40,-55.20});
dataSet.push_back({19.30,48.17,-54.90});
dataSet.push_back({16.04,49.93,-55.59});
dataSet.push_back({15.87,50.05,-58.09});
dataSet.push_back({13.98,51.42,-59.06});
dataSet.push_back({13.04,51.83,-61.22});
dataSet.push_back({12.87,52.55,-59.79});
dataSet.push_back({14.43,51.29,-57.17});
dataSet.push_back({16.72,50.70,-57.75});
dataSet.push_back({21.45,46.34,-56.70});
dataSet.push_back({23.88,43.42,-55.38});
dataSet.push_back({26.72,38.82,-54.15});
dataSet.push_back({29.25,35.02,-52.89});
dataSet.push_back({31.14,28.36,-52.34});
dataSet.push_back({32.30,22.23,-50.39});
dataSet.push_back({30.99,15.08,-48.44});
dataSet.push_back({29.31,8.96,-47.07});
dataSet.push_back({26.97,4.39,-46.15});
dataSet.push_back({23.10,-1.11,-46.33});
dataSet.push_back({19.57,-4.61,-46.23});
dataSet.push_back({14.49,-7.24,-47.90});
dataSet.push_back({9.30,-9.36,-46.58});
dataSet.push_back({5.08,-10.82,-47.60});
dataSet.push_back({-1.77,-11.57,-47.73});
dataSet.push_back({-6.71,-11.98,-47.95});
dataSet.push_back({-10.42,-10.90,-47.92});
dataSet.push_back({-14.99,-10.17,-50.73});
dataSet.push_back({-17.79,-8.81,-49.62});
dataSet.push_back({-19.76,-8.15,-50.82});
dataSet.push_back({-19.29,-8.42,-50.84});
dataSet.push_back({-17.88,-8.53,-50.26});
dataSet.push_back({-12.21,-11.25,-51.70});
dataSet.push_back({-6.31,-11.85,-51.97});
dataSet.push_back({-0.16,-12.54,-51.17});
dataSet.push_back({6.04,-12.05,-50.28});
dataSet.push_back({10.39,-10.18,-52.64});
dataSet.push_back({15.53,-8.81,-53.00});
dataSet.push_back({19.45,-6.16,-53.77});
dataSet.push_back({22.41,-3.31,-52.99});
dataSet.push_back({25.50,0.92,-52.21});
dataSet.push_back({27.50,3.17,-51.78});
dataSet.push_back({28.12,5.52,-50.82});
dataSet.push_back({28.14,5.74,-49.83});
dataSet.push_back({28.05,6.27,-46.85});
dataSet.push_back({27.51,6.81,-44.23});
dataSet.push_back({25.43,6.29,-42.40});
dataSet.push_back({24.69,6.26,-40.73});
dataSet.push_back({22.68,6.53,-38.39});
dataSet.push_back({19.31,6.36,-33.69});
dataSet.push_back({14.54,6.64,-30.08});
dataSet.push_back({7.80,6.51,-28.09});
dataSet.push_back({2.78,7.17,-25.76});
dataSet.push_back({-1.23,8.11,-26.26});
dataSet.push_back({-5.25,8.70,-25.79});
dataSet.push_back({-7.25,9.56,-25.42});
dataSet.push_back({-8.72,9.25,-25.73});
dataSet.push_back({-9.74,9.05,-25.73});
dataSet.push_back({-9.91,8.95,-24.38});
dataSet.push_back({-8.53,8.46,-25.56});
dataSet.push_back({-5.47,8.34,-26.81});
dataSet.push_back({-0.94,7.96,-26.25});
dataSet.push_back({4.99,8.09,-25.62});
dataSet.push_back({10.89,8.22,-27.83});
dataSet.push_back({17.46,8.59,-31.20});
dataSet.push_back({23.07,8.22,-36.25});
dataSet.push_back({26.77,8.53,-42.44});
dataSet.push_back({27.53,6.63,-47.28});
dataSet.push_back({28.09,4.97,-52.36});
dataSet.push_back({27.95,4.26,-56.32});
dataSet.push_back({28.11,4.72,-58.12});
dataSet.push_back({27.04,5.53,-62.93});
dataSet.push_back({23.50,5.42,-70.54});
dataSet.push_back({19.60,6.04,-76.29});
dataSet.push_back({12.63,6.03,-80.60});
dataSet.push_back({7.35,6.67,-84.83});
dataSet.push_back({5.14,7.90,-85.67});
dataSet.push_back({2.06,8.09,-86.52});
dataSet.push_back({-0.31,9.45,-88.12});
dataSet.push_back({-1.86,9.40,-88.24});
dataSet.push_back({-1.18,10.21,-89.28});
dataSet.push_back({0.56,10.39,-89.38});
dataSet.push_back({2.14,9.30,-88.10});
dataSet.push_back({4.76,8.68,-87.40});
dataSet.push_back({6.88,6.68,-84.42});
dataSet.push_back({8.91,3.25,-81.99});
dataSet.push_back({12.85,2.93,-80.33});
dataSet.push_back({15.99,1.01,-75.02});
dataSet.push_back({18.44,-1.26,-71.87});
dataSet.push_back({20.08,-0.43,-69.14});
dataSet.push_back({20.01,-2.16,-68.68});
dataSet.push_back({18.31,-4.11,-65.42});
dataSet.push_back({21.33,-2.72,-64.39});
dataSet.push_back({22.55,-1.34,-60.93});
dataSet.push_back({24.00,-0.57,-58.91});
dataSet.push_back({26.00,2.34,-57.45});
dataSet.push_back({26.60,3.38,-57.50});
dataSet.push_back({28.19,5.87,-55.74});
dataSet.push_back({29.26,8.36,-55.15});
dataSet.push_back({30.76,11.17,-56.24});
dataSet.push_back({31.26,14.54,-54.04});
dataSet.push_back({30.65,20.63,-54.60});
dataSet.push_back({30.04,26.07,-55.33});
dataSet.push_back({28.41,31.02,-54.87});
dataSet.push_back({26.45,34.92,-54.97});
dataSet.push_back({23.55,38.44,-56.32});
dataSet.push_back({20.15,42.11,-56.80});
dataSet.push_back({16.69,44.30,-58.12});
dataSet.push_back({14.11,45.73,-57.03});
dataSet.push_back({12.94,46.28,-57.58});
dataSet.push_back({11.74,47.38,-58.61});
dataSet.push_back({11.06,47.81,-60.41});
dataSet.push_back({11.90,48.18,-59.57});
dataSet.push_back({15.38,45.54,-59.55});
dataSet.push_back({19.32,43.49,-58.86});
dataSet.push_back({23.54,38.05,-57.17});
dataSet.push_back({27.17,31.77,-56.73});
dataSet.push_back({29.80,25.48,-57.21});
dataSet.push_back({31.09,19.06,-56.08});
dataSet.push_back({30.14,10.73,-56.02});
dataSet.push_back({27.49,4.27,-56.27});
dataSet.push_back({23.11,-2.11,-56.71});
dataSet.push_back({17.09,-6.74,-56.52});
dataSet.push_back({11.02,-9.89,-54.91});
dataSet.push_back({5.08,-11.10,-53.83});
dataSet.push_back({-3.31,-11.18,-52.87});
dataSet.push_back({-8.28,-10.44,-51.83});
dataSet.push_back({-14.78,-9.03,-51.60});
dataSet.push_back({-18.26,-7.43,-50.74});
dataSet.push_back({-20.57,-5.51,-49.22});
dataSet.push_back({-22.55,-4.26,-51.20});
dataSet.push_back({-20.06,-5.53,-51.23});
dataSet.push_back({-14.01,-9.02,-52.58});
dataSet.push_back({-9.07,-10.38,-53.64});
dataSet.push_back({-0.02,-11.35,-55.53});
dataSet.push_back({6.91,-10.02,-56.40});
dataSet.push_back({14.54,-7.59,-56.02});
dataSet.push_back({20.08,-3.76,-55.03});
dataSet.push_back({24.13,1.10,-56.48});
dataSet.push_back({26.10,3.99,-57.65});
dataSet.push_back({28.02,7.19,-57.09});
dataSet.push_back({28.54,9.22,-56.69});
dataSet.push_back({29.10,11.43,-56.23});
dataSet.push_back({29.42,11.84,-56.13});
dataSet.push_back({29.28,12.27,-55.40});
dataSet.push_back({28.50,11.57,-53.88});
dataSet.push_back({28.19,11.21,-52.00});
dataSet.push_back({28.37,11.57,-51.15});
dataSet.push_back({27.16,10.60,-49.38});
dataSet.push_back({26.18,9.31,-48.23});
dataSet.push_back({25.76,8.53,-47.05});
dataSet.push_back({23.89,5.88,-48.62});
dataSet.push_back({26.88,-1.64,-62.84});
dataSet.push_back({54.49,-72.62,-186.47});
dataSet.push_back({399.99,399.99,399.99});
dataSet.push_back({399.99,-268.48,-27.67});
dataSet.push_back({-400.00,200.61,29.25});
dataSet.push_back({-53.44,-22.33,-95.00});
dataSet.push_back({-16.67,-12.95,-63.06});
dataSet.push_back({-26.70,-13.38,-57.01});
dataSet.push_back({-24.67,-16.88,-56.87});
dataSet.push_back({-29.76,-6.31,-36.72});
dataSet.push_back({-38.48,4.30,-2.12});
dataSet.push_back({-22.40,-1.05,0.44});
dataSet.push_back({-18.03,-15.92,0.96});
dataSet.push_back({6.09,-11.08,-13.81});
dataSet.push_back({2.47,1.57,-20.63});
dataSet.push_back({-0.15,1.90,-20.32});
dataSet.push_back({2.00,2.76,-20.06});
dataSet.push_back({5.66,4.17,-18.79});
dataSet.push_back({9.08,5.68,-20.01});
dataSet.push_back({12.30,6.07,-21.28});
dataSet.push_back({16.19,7.74,-22.77});
dataSet.push_back({20.76,9.58,-25.34});
dataSet.push_back({23.34,9.20,-30.36});
dataSet.push_back({27.50,11.38,-33.23});
dataSet.push_back({29.14,11.04,-39.15});
dataSet.push_back({30.88,13.24,-44.98});
dataSet.push_back({30.62,14.04,-52.99});
dataSet.push_back({26.94,11.16,-58.45});
dataSet.push_back({22.53,12.38,-65.30});
dataSet.push_back({16.91,12.38,-70.02});
dataSet.push_back({14.09,11.11,-71.45});
dataSet.push_back({11.80,10.30,-71.53});
dataSet.push_back({8.46,11.91,-75.20});
dataSet.push_back({5.00,11.36,-75.92});
dataSet.push_back({2.75,11.57,-76.53});
dataSet.push_back({1.57,11.85,-77.17});
dataSet.push_back({1.93,11.52,-75.65});
dataSet.push_back({3.08,11.99,-76.54});
dataSet.push_back({3.81,12.00,-76.66});
dataSet.push_back({6.43,12.39,-76.21});
dataSet.push_back({10.41,10.84,-75.77});
dataSet.push_back({14.29,9.12,-73.18});
dataSet.push_back({19.74,7.56,-67.10});
dataSet.push_back({22.86,5.83,-60.63});
dataSet.push_back({23.35,3.72,-58.08});
dataSet.push_back({22.08,1.23,-54.03});
dataSet.push_back({21.56,1.09,-51.67});
dataSet.push_back({22.31,0.31,-49.49});
dataSet.push_back({22.20,0.93,-49.00});
dataSet.push_back({24.80,2.97,-47.40});
dataSet.push_back({25.17,4.70,-48.06});
dataSet.push_back({27.83,7.04,-47.46});
dataSet.push_back({29.48,10.69,-47.91});
dataSet.push_back({30.18,14.12,-46.89});
dataSet.push_back({31.34,19.19,-48.21});
dataSet.push_back({31.34,24.19,-47.24});
dataSet.push_back({30.20,30.49,-47.27});
dataSet.push_back({28.00,37.76,-47.40});
dataSet.push_back({24.35,43.15,-45.98});
dataSet.push_back({20.91,47.09,-46.80});
dataSet.push_back({17.41,49.24,-48.91});
dataSet.push_back({14.50,51.48,-50.37});
dataSet.push_back({11.34,52.97,-52.55});
dataSet.push_back({9.22,54.35,-53.61});
dataSet.push_back({8.13,54.58,-51.87});
dataSet.push_back({9.23,54.63,-51.66});
dataSet.push_back({10.96,53.85,-52.11});
dataSet.push_back({14.73,50.39,-50.95});
dataSet.push_back({18.66,47.58,-50.38});
dataSet.push_back({23.46,42.14,-49.34});
dataSet.push_back({26.25,38.23,-49.12});
dataSet.push_back({29.03,31.96,-47.58});
dataSet.push_back({30.80,26.21,-47.19});
dataSet.push_back({30.36,18.09,-48.41});
dataSet.push_back({28.55,11.87,-49.83});
dataSet.push_back({25.26,5.15,-49.30});
dataSet.push_back({21.19,0.87,-51.27});
dataSet.push_back({17.43,-3.44,-49.51});
dataSet.push_back({11.67,-6.99,-49.93});
dataSet.push_back({5.24,-9.46,-48.03});
dataSet.push_back({-1.66,-10.07,-46.56});
dataSet.push_back({-7.46,-9.73,-45.31});
dataSet.push_back({-11.99,-9.46,-44.18});
dataSet.push_back({-15.78,-7.82,-44.48});
dataSet.push_back({-18.46,-6.62,-43.15});
dataSet.push_back({-20.89,-5.80,-42.99});
dataSet.push_back({-19.69,-6.38,-42.38});
dataSet.push_back({-15.87,-7.54,-43.68});
dataSet.push_back({-9.83,-9.11,-44.90});
dataSet.push_back({-4.30,-9.64,-45.04});
dataSet.push_back({3.19,-9.70,-48.02});
dataSet.push_back({10.53,-7.50,-47.51});
dataSet.push_back({15.37,-5.80,-48.45});
dataSet.push_back({18.43,-3.74,-48.51});
dataSet.push_back({22.07,-0.89,-50.24});
dataSet.push_back({25.10,4.15,-51.50});
dataSet.push_back({27.23,5.87,-49.28});
dataSet.push_back({26.87,6.74,-46.97});
dataSet.push_back({26.88,5.83,-44.92});
dataSet.push_back({25.17,4.20,-44.92});
dataSet.push_back({25.98,5.54,-44.69});
dataSet.push_back({26.43,5.47,-44.36});
dataSet.push_back({25.37,5.69,-43.97});
dataSet.push_back({26.05,6.35,-41.98});
dataSet.push_back({26.09,7.39,-39.97});
dataSet.push_back({25.54,7.56,-37.44});
dataSet.push_back({24.45,7.15,-37.12});
dataSet.push_back({23.32,7.57,-34.16});
dataSet.push_back({23.44,8.29,-32.84});
dataSet.push_back({21.62,6.96,-30.38});
dataSet.push_back({19.54,6.93,-29.50});
dataSet.push_back({18.66,6.70,-27.16});
dataSet.push_back({15.91,4.93,-25.22});
dataSet.push_back({12.55,3.28,-23.44});
dataSet.push_back({10.58,0.18,-23.27});
dataSet.push_back({8.58,-3.42,-21.72});
dataSet.push_back({7.86,-5.48,-22.45});
dataSet.push_back({7.25,-7.70,-23.35});
dataSet.push_back({7.41,-7.15,-24.18});
dataSet.push_back({9.07,-3.80,-24.40});
dataSet.push_back({9.79,-1.84,-27.11});
dataSet.push_back({14.81,3.54,-26.64});
dataSet.push_back({17.58,4.50,-29.90});
dataSet.push_back({21.07,5.35,-31.86});
dataSet.push_back({24.69,6.71,-35.34});
dataSet.push_back({26.17,5.40,-40.82});
dataSet.push_back({29.24,7.25,-44.46});
dataSet.push_back({29.91,5.63,-49.52});
dataSet.push_back({29.09,4.09,-55.16});
dataSet.push_back({28.33,3.99,-60.18});
dataSet.push_back({25.94,0.71,-64.21});
dataSet.push_back({22.97,1.04,-70.13});
dataSet.push_back({21.08,0.43,-73.56});
dataSet.push_back({17.52,0.70,-79.53});
dataSet.push_back({17.33,0.55,-80.24});
dataSet.push_back({17.60,2.67,-82.01});
dataSet.push_back({23.89,3.88,-86.82});
dataSet.push_back({35.63,8.67,-101.32});
dataSet.push_back({97.53,34.00,-167.82});
dataSet.push_back({356.34,189.21,31.68});
dataSet.push_back({79.91,399.99,399.99});
dataSet.push_back({-240.45,-265.76,-400.00});
dataSet.push_back({-40.99,-61.89,-161.05});
dataSet.push_back({-29.03,-55.80,-109.69});
dataSet.push_back({-1.33,-17.49,-83.85});
dataSet.push_back({7.59,1.73,-81.79});
dataSet.push_back({12.21,5.32,-75.89});
dataSet.push_back({15.63,6.16,-70.53});
dataSet.push_back({19.65,6.42,-66.11});
dataSet.push_back({22.51,4.96,-61.17});
dataSet.push_back({25.04,5.42,-57.43});
dataSet.push_back({26.21,5.47,-54.74});
dataSet.push_back({26.86,6.02,-52.15});
dataSet.push_back({27.19,6.05,-49.46});
dataSet.push_back({27.25,5.94,-47.46});
dataSet.push_back({26.99,5.73,-44.34});
dataSet.push_back({26.75,6.07,-45.30});
dataSet.push_back({26.28,5.90,-43.85});
dataSet.push_back({25.70,5.02,-44.09});
dataSet.push_back({25.81,5.42,-44.10});
dataSet.push_back({23.94,2.89,-43.97});
dataSet.push_back({22.74,0.72,-45.00});
dataSet.push_back({21.13,-0.67,-43.51});
dataSet.push_back({18.35,-3.47,-44.53});
dataSet.push_back({16.00,-4.93,-44.59});
dataSet.push_back({13.26,-7.12,-41.92});
dataSet.push_back({10.07,-8.98,-43.88});
dataSet.push_back({6.19,-10.21,-44.26});
dataSet.push_back({1.55,-11.33,-43.07});
dataSet.push_back({-1.56,-11.34,-43.09});
dataSet.push_back({-5.81,-11.38,-42.21});
dataSet.push_back({-10.07,-10.57,-40.94});
dataSet.push_back({-14.51,-9.28,-41.33});
dataSet.push_back({-17.68,-7.54,-39.92});
dataSet.push_back({-20.29,-6.45,-39.87});
dataSet.push_back({-21.75,-5.20,-39.64});
dataSet.push_back({-23.05,-4.69,-41.03});
dataSet.push_back({-21.62,-5.70,-40.54});
dataSet.push_back({-16.83,-8.18,-41.58});
dataSet.push_back({-11.54,-10.66,-42.09});
dataSet.push_back({-5.75,-11.17,-42.38});
dataSet.push_back({1.14,-11.12,-43.63});
dataSet.push_back({7.60,-9.45,-45.37});
dataSet.push_back({14.48,-6.84,-44.82});
dataSet.push_back({19.76,-3.19,-44.71});
dataSet.push_back({23.77,1.20,-44.95});
dataSet.push_back({27.60,6.08,-44.73});
dataSet.push_back({29.58,11.71,-44.25});
dataSet.push_back({31.01,18.38,-45.00});
dataSet.push_back({30.49,24.35,-44.59});
dataSet.push_back({29.76,30.73,-44.97});
dataSet.push_back({27.09,37.30,-45.52});
dataSet.push_back({23.02,42.50,-47.06});
dataSet.push_back({18.54,46.92,-48.10});
dataSet.push_back({14.90,49.00,-48.93});
dataSet.push_back({11.95,50.85,-49.48});
dataSet.push_back({10.28,51.03,-51.31});
dataSet.push_back({10.74,50.74,-52.17});
dataSet.push_back({12.83,50.40,-51.86});
dataSet.push_back({17.19,46.88,-51.67});
dataSet.push_back({21.97,42.99,-50.49});
dataSet.push_back({25.56,37.23,-50.01});
dataSet.push_back({28.34,31.18,-49.71});
dataSet.push_back({30.31,23.40,-48.96});
dataSet.push_back({29.82,15.80,-49.93});
dataSet.push_back({28.05,9.39,-50.37});
dataSet.push_back({26.87,6.12,-49.84});
dataSet.push_back({25.63,3.33,-49.10});
dataSet.push_back({25.90,2.66,-49.02});
dataSet.push_back({25.88,3.58,-47.63});
dataSet.push_back({26.72,3.12,-46.63});
dataSet.push_back({26.76,3.74,-45.00});
dataSet.push_back({26.55,4.52,-41.89});
dataSet.push_back({26.32,4.86,-40.00});
dataSet.push_back({25.72,5.82,-38.04});
dataSet.push_back({25.26,5.97,-35.46});
dataSet.push_back({22.55,5.30,-33.31});
dataSet.push_back({21.50,5.44,-31.57});
dataSet.push_back({19.07,4.81,-28.81});
dataSet.push_back({15.71,3.54,-27.28});
dataSet.push_back({12.54,2.19,-25.84});
dataSet.push_back({8.23,-0.55,-25.33});
dataSet.push_back({4.86,-2.27,-26.31});
dataSet.push_back({2.76,-4.03,-28.26});
dataSet.push_back({1.50,-5.16,-27.66});
dataSet.push_back({0.89,-6.40,-28.65});
dataSet.push_back({1.12,-6.04,-28.04});
dataSet.push_back({2.87,-4.46,-27.76});
dataSet.push_back({4.57,-2.42,-26.01});
dataSet.push_back({7.84,-0.67,-28.02});
dataSet.push_back({11.61,1.40,-27.92});
dataSet.push_back({14.98,2.70,-28.16});
dataSet.push_back({19.63,5.47,-30.29});
dataSet.push_back({21.36,4.68,-31.64});
dataSet.push_back({24.06,5.73,-35.16});
dataSet.push_back({27.33,7.12,-39.37});
dataSet.push_back({28.65,7.30,-44.64});
dataSet.push_back({28.98,7.65,-49.26});
dataSet.push_back({27.78,7.29,-54.16});
dataSet.push_back({26.15,5.98,-57.35});
dataSet.push_back({23.43,5.43,-61.55});
dataSet.push_back({19.97,6.20,-67.21});
dataSet.push_back({17.10,6.56,-68.92});
dataSet.push_back({15.42,6.67,-70.00});
dataSet.push_back({14.49,6.74,-70.28});
dataSet.push_back({13.11,8.11,-72.45});
dataSet.push_back({11.08,9.31,-74.19});
dataSet.push_back({8.23,10.08,-76.48});
dataSet.push_back({5.16,9.03,-76.67});
dataSet.push_back({2.09,9.46,-77.06});
dataSet.push_back({0.01,9.40,-76.07});
dataSet.push_back({-0.34,7.81,-76.49});
dataSet.push_back({-0.26,7.31,-76.50});
dataSet.push_back({0.68,7.18,-75.18});
dataSet.push_back({4.52,6.73,-75.04});
dataSet.push_back({6.74,5.03,-72.79});
dataSet.push_back({9.62,5.38,-72.83});
dataSet.push_back({12.73,4.52,-71.34});
dataSet.push_back({14.84,4.91,-69.48});
dataSet.push_back({17.47,3.55,-66.19});
dataSet.push_back({19.17,3.16,-63.71});
dataSet.push_back({20.19,2.31,-58.79});
dataSet.push_back({20.73,0.63,-55.99});
dataSet.push_back({19.56,-0.74,-53.10});
dataSet.push_back({16.35,-3.17,-51.10});
dataSet.push_back({13.27,-4.42,-48.28});
dataSet.push_back({11.62,-5.25,-47.19});
dataSet.push_back({8.69,-6.62,-47.17});
dataSet.push_back({7.18,-8.01,-47.95});
dataSet.push_back({4.82,-8.23,-47.46});
dataSet.push_back({1.37,-9.24,-48.52});
dataSet.push_back({-0.93,-10.18,-48.33});
dataSet.push_back({-5.44,-11.11,-49.79});
dataSet.push_back({-10.60,-12.92,-52.47});
dataSet.push_back({-17.07,-17.50,-62.90});
dataSet.push_back({-28.67,-38.34,-85.27});
dataSet.push_back({-37.72,-113.89,-95.74});
dataSet.push_back({-33.94,-270.65,-20.26});
dataSet.push_back({26.70,-400.00,196.45});
dataSet.push_back({164.39,-400.00,399.99});
dataSet.push_back({222.40,-259.18,399.99});
dataSet.push_back({227.33,-90.21,399.99});
dataSet.push_back({153.38,-159.62,399.99});
dataSet.push_back({90.22,-248.73,399.99});
dataSet.push_back({-203.42,-395.00,-61.05});
dataSet.push_back({-400.00,-400.00,-173.07});
dataSet.push_back({-79.08,399.99,-400.00});
dataSet.push_back({399.99,399.99,399.99});
dataSet.push_back({399.99,-116.03,399.99});
dataSet.push_back({44.52,-400.00,399.99});
dataSet.push_back({37.57,-400.00,399.99});
dataSet.push_back({-25.39,-345.51,0.05});
dataSet.push_back({-8.98,-129.26,-76.44});
dataSet.push_back({5.46,-40.80,-75.79});
dataSet.push_back({3.27,-11.27,-59.13});
dataSet.push_back({8.06,-7.82,-51.28});
dataSet.push_back({12.88,-5.18,-48.88});
dataSet.push_back({16.56,-2.21,-46.36});
dataSet.push_back({21.59,2.84,-46.77});
dataSet.push_back({25.76,9.05,-44.01});
dataSet.push_back({26.94,15.37,-43.43});
dataSet.push_back({27.75,21.23,-43.31});
dataSet.push_back({27.26,27.37,-44.80});
dataSet.push_back({25.72,32.48,-46.70});
dataSet.push_back({23.54,36.82,-47.30});
dataSet.push_back({21.19,39.82,-48.39});
dataSet.push_back({19.10,41.93,-49.95});
dataSet.push_back({15.82,45.20,-49.17});
dataSet.push_back({12.74,47.23,-49.38});
dataSet.push_back({10.23,49.08,-50.70});
dataSet.push_back({7.60,49.65,-51.25});
dataSet.push_back({6.71,50.26,-52.31});
dataSet.push_back({6.12,50.71,-52.92});
dataSet.push_back({6.57,50.73,-52.98});
dataSet.push_back({6.25,51.05,-53.70});
dataSet.push_back({9.41,49.84,-51.56});
dataSet.push_back({13.13,47.63,-50.37});
dataSet.push_back({17.94,44.54,-49.96});
dataSet.push_back({21.78,39.78,-48.40});
dataSet.push_back({25.60,33.56,-47.67});
dataSet.push_back({28.04,26.75,-48.06});
dataSet.push_back({28.92,22.19,-48.66});
dataSet.push_back({28.88,18.04,-49.02});
dataSet.push_back({28.88,13.11,-48.36});
dataSet.push_back({27.17,7.53,-49.10});
dataSet.push_back({26.16,5.98,-48.56});
dataSet.push_back({26.46,4.87,-47.11});
dataSet.push_back({26.32,3.92,-47.78});
dataSet.push_back({25.68,3.87,-46.47});
dataSet.push_back({25.16,3.44,-44.81});
dataSet.push_back({24.99,3.85,-42.11});
dataSet.push_back({24.90,4.85,-39.65});
dataSet.push_back({23.33,4.74,-35.33});
dataSet.push_back({21.73,4.35,-33.57});
dataSet.push_back({19.32,3.87,-30.57});
dataSet.push_back({17.21,2.95,-29.17});
dataSet.push_back({14.56,3.14,-27.04});
dataSet.push_back({10.77,2.26,-25.38});
dataSet.push_back({7.30,1.97,-22.74});
dataSet.push_back({4.83,0.10,-22.79});
dataSet.push_back({2.91,-0.62,-21.96});
dataSet.push_back({0.99,-3.11,-22.08});
dataSet.push_back({-0.05,-5.27,-22.49});
dataSet.push_back({-0.96,-6.91,-24.00});
dataSet.push_back({0.33,-6.25,-22.97});
dataSet.push_back({2.25,-4.91,-24.69});
dataSet.push_back({4.47,-3.00,-23.50});
dataSet.push_back({7.29,-0.74,-25.15});
dataSet.push_back({10.22,-0.18,-25.72});
dataSet.push_back({13.44,1.11,-27.54});
dataSet.push_back({16.83,2.77,-29.82});
dataSet.push_back({20.46,4.33,-32.60});
dataSet.push_back({23.25,6.29,-36.65});
dataSet.push_back({26.15,6.54,-41.21});
dataSet.push_back({26.16,7.68,-45.69});
dataSet.push_back({26.34,7.52,-49.66});
dataSet.push_back({25.31,6.32,-54.42});
dataSet.push_back({23.88,6.84,-59.07});
dataSet.push_back({21.34,6.26,-61.30});
dataSet.push_back({19.35,6.48,-63.92});
dataSet.push_back({16.11,6.81,-67.79});
dataSet.push_back({13.10,7.67,-69.47});
dataSet.push_back({9.44,8.68,-72.38});
dataSet.push_back({7.28,9.57,-72.90});
dataSet.push_back({5.49,10.13,-73.54});
dataSet.push_back({3.04,10.82,-75.34});
dataSet.push_back({0.39,11.33,-75.13});
dataSet.push_back({0.04,11.16,-76.48});
dataSet.push_back({0.17,11.18,-75.78});
dataSet.push_back({0.74,11.12,-74.29});
dataSet.push_back({2.34,9.53,-75.59});
dataSet.push_back({4.79,7.57,-73.19});
dataSet.push_back({7.13,7.09,-73.17});
dataSet.push_back({9.79,6.54,-70.04});
dataSet.push_back({12.37,6.57,-68.80});
dataSet.push_back({15.10,6.03,-66.35});
dataSet.push_back({18.42,5.81,-62.95});
dataSet.push_back({21.20,5.55,-58.64});
dataSet.push_back({21.29,5.37,-56.90});
dataSet.push_back({22.17,5.25,-56.46});
dataSet.push_back({21.46,6.10,-54.38});
dataSet.push_back({22.27,5.82,-53.55});
dataSet.push_back({22.46,5.53,-51.72});
dataSet.push_back({22.17,4.86,-50.82});
dataSet.push_back({22.18,4.76,-51.07});
dataSet.push_back({22.38,4.15,-49.16});
dataSet.push_back({21.94,3.67,-47.13});
dataSet.push_back({22.35,4.93,-45.15});
dataSet.push_back({25.04,8.13,-46.86});
dataSet.push_back({25.99,11.54,-46.42});
dataSet.push_back({27.09,15.14,-44.67});
dataSet.push_back({27.69,19.40,-45.31});
dataSet.push_back({27.51,24.33,-44.47});
dataSet.push_back({26.38,28.94,-42.03});
dataSet.push_back({24.40,33.48,-44.35});
dataSet.push_back({22.35,37.37,-45.56});
dataSet.push_back({19.43,41.04,-47.09});
dataSet.push_back({16.75,44.46,-48.85});
dataSet.push_back({13.76,45.72,-47.89});
dataSet.push_back({10.40,48.03,-48.69});
dataSet.push_back({8.01,48.32,-49.71});
dataSet.push_back({6.80,48.95,-50.09});
dataSet.push_back({5.25,49.87,-50.01});
dataSet.push_back({6.62,49.04,-49.96});
dataSet.push_back({6.69,48.69,-50.22});
dataSet.push_back({10.80,48.44,-48.18});
dataSet.push_back({14.48,45.68,-47.92});
dataSet.push_back({18.13,42.25,-48.54});
dataSet.push_back({22.40,37.59,-47.31});
dataSet.push_back({25.29,31.68,-45.07});
dataSet.push_back({27.92,25.44,-45.70});
dataSet.push_back({27.95,20.26,-44.30});
dataSet.push_back({28.15,14.48,-46.50});
dataSet.push_back({26.76,9.11,-45.41});
dataSet.push_back({24.15,3.45,-44.60});
dataSet.push_back({21.36,-0.62,-44.98});
dataSet.push_back({18.09,-4.19,-45.91});
dataSet.push_back({13.88,-7.46,-45.65});
dataSet.push_back({9.01,-10.24,-47.17});
dataSet.push_back({3.00,-11.10,-46.73});
dataSet.push_back({-1.14,-12.54,-46.85});
dataSet.push_back({-6.99,-12.28,-48.33});
dataSet.push_back({-11.11,-10.58,-46.22});
dataSet.push_back({-15.56,-9.72,-45.07});
dataSet.push_back({-18.55,-8.63,-44.53});
dataSet.push_back({-18.47,-8.58,-45.10});
dataSet.push_back({-20.50,-7.98,-43.86});
dataSet.push_back({-19.47,-8.78,-45.03});
dataSet.push_back({-17.05,-9.18,-44.14});
dataSet.push_back({-14.25,-10.35,-45.83});
dataSet.push_back({-10.36,-11.49,-46.03});
dataSet.push_back({-6.27,-12.07,-46.39});
dataSet.push_back({-0.93,-11.45,-47.53});
dataSet.push_back({5.42,-10.78,-46.70});
dataSet.push_back({9.89,-9.00,-47.12});
dataSet.push_back({15.15,-6.04,-48.19});
dataSet.push_back({19.95,-2.27,-47.08});
dataSet.push_back({22.44,1.99,-48.35});
dataSet.push_back({25.15,6.59,-48.41});
dataSet.push_back({26.51,9.67,-49.12});
dataSet.push_back({26.43,11.13,-48.83});
dataSet.push_back({26.25,11.63,-47.88});
dataSet.push_back({26.27,11.15,-49.32});
dataSet.push_back({25.67,11.16,-48.78});
dataSet.push_back({26.35,10.53,-47.63});
dataSet.push_back({26.11,10.68,-46.81});
dataSet.push_back({26.12,9.96,-46.19});
dataSet.push_back({25.21,9.39,-44.49});
dataSet.push_back({24.93,9.03,-42.46});
dataSet.push_back({23.77,7.64,-41.02});
dataSet.push_back({23.45,7.13,-40.06});
dataSet.push_back({22.23,6.19,-39.65});
dataSet.push_back({21.34,3.83,-37.84});
dataSet.push_back({21.79,-0.81,-36.28});
dataSet.push_back({28.43,-13.88,-40.69});
dataSet.push_back({68.29,-57.47,-19.42});
dataSet.push_back({160.50,-164.17,283.39});
dataSet.push_back({-171.56,110.75,399.99});
dataSet.push_back({-400.00,-37.61,-400.00});
dataSet.push_back({-54.79,-40.61,-117.02});
dataSet.push_back({-36.28,-15.58,-40.32});
dataSet.push_back({-14.79,-9.42,-22.36});
dataSet.push_back({-2.11,-7.82,-24.41});
dataSet.push_back({2.81,-5.32,-25.81});
dataSet.push_back({4.49,-3.48,-28.72});
dataSet.push_back({3.27,-2.67,-30.27});
dataSet.push_back({3.53,-1.34,-29.68});
dataSet.push_back({4.61,-0.67,-27.80});
dataSet.push_back({7.67,0.95,-28.22});
dataSet.push_back({10.80,3.25,-27.99});
dataSet.push_back({13.60,5.14,-27.05});
dataSet.push_back({16.69,6.02,-29.06});
dataSet.push_back({19.63,7.13,-31.36});
dataSet.push_back({23.65,8.84,-33.73});
dataSet.push_back({24.77,9.66,-37.04});
dataSet.push_back({26.68,9.46,-40.64});
dataSet.push_back({27.37,9.50,-44.91});
dataSet.push_back({27.94,11.93,-47.96});
dataSet.push_back({27.54,13.10,-53.54});
dataSet.push_back({26.17,12.18,-57.31});
dataSet.push_back({23.36,11.67,-62.13});
dataSet.push_back({21.01,11.56,-65.94});
dataSet.push_back({16.71,9.96,-68.85});
dataSet.push_back({10.47,9.73,-72.47});
dataSet.push_back({6.51,10.00,-75.51});
dataSet.push_back({3.17,9.19,-75.60});
dataSet.push_back({1.75,10.27,-75.43});
dataSet.push_back({-0.01,10.49,-75.61});
dataSet.push_back({-2.69,11.07,-77.37});
dataSet.push_back({-4.08,12.40,-77.03});
dataSet.push_back({-4.04,13.10,-76.83});
dataSet.push_back({-0.85,11.34,-76.10});
dataSet.push_back({2.04,9.69,-75.15});
dataSet.push_back({5.22,7.76,-74.41});
dataSet.push_back({7.14,7.50,-72.92});
dataSet.push_back({9.53,6.40,-70.68});
dataSet.push_back({12.26,6.47,-69.09});
dataSet.push_back({14.33,4.70,-65.30});
dataSet.push_back({16.58,3.72,-62.62});
dataSet.push_back({16.85,1.09,-58.11});
dataSet.push_back({17.54,-0.65,-53.43});
dataSet.push_back({16.52,-2.43,-50.60});
dataSet.push_back({14.33,-4.25,-47.11});
dataSet.push_back({14.10,-5.19,-45.43});
dataSet.push_back({12.92,-5.02,-45.72});
dataSet.push_back({12.94,-5.47,-45.06});
dataSet.push_back({15.12,-3.91,-45.12});
dataSet.push_back({17.41,-2.93,-46.40});
dataSet.push_back({19.48,-0.34,-45.97});
dataSet.push_back({22.01,2.99,-45.78});
dataSet.push_back({24.23,5.37,-46.07});
dataSet.push_back({25.89,9.12,-44.58});
dataSet.push_back({26.73,12.77,-44.71});
dataSet.push_back({27.45,16.81,-46.36});
dataSet.push_back({28.08,21.24,-47.03});
dataSet.push_back({27.48,26.76,-46.96});
dataSet.push_back({26.71,29.13,-48.65});
dataSet.push_back({25.59,31.69,-48.22});
dataSet.push_back({23.89,34.63,-48.91});
dataSet.push_back({22.64,36.46,-48.90});
dataSet.push_back({20.75,39.50,-50.16});
dataSet.push_back({18.10,42.21,-52.39});
dataSet.push_back({16.26,44.53,-50.87});
dataSet.push_back({11.02,47.64,-51.05});
dataSet.push_back({9.17,49.00,-51.89});
dataSet.push_back({8.91,48.49,-51.84});
dataSet.push_back({7.97,48.27,-51.95});
dataSet.push_back({10.64,46.80,-52.19});
dataSet.push_back({15.10,44.47,-51.07});
dataSet.push_back({20.07,38.88,-50.96});
dataSet.push_back({24.44,31.81,-50.48});
dataSet.push_back({26.98,25.55,-48.78});
dataSet.push_back({27.75,17.92,-48.49});
dataSet.push_back({27.11,11.39,-48.55});
dataSet.push_back({24.60,6.25,-46.68});
dataSet.push_back({21.57,1.70,-46.98});
dataSet.push_back({17.38,-3.96,-46.13});
dataSet.push_back({12.74,-6.64,-45.92});
dataSet.push_back({8.02,-9.22,-46.08});
dataSet.push_back({4.53,-10.22,-46.44});
dataSet.push_back({0.77,-11.12,-44.54});
dataSet.push_back({-2.29,-11.76,-43.82});
dataSet.push_back({-5.66,-11.45,-43.19});
dataSet.push_back({-8.37,-11.01,-42.99});
dataSet.push_back({-13.21,-10.60,-42.85});
dataSet.push_back({-16.56,-9.05,-42.20});
dataSet.push_back({-18.75,-8.83,-41.71});
dataSet.push_back({-17.94,-8.72,-42.44});
dataSet.push_back({-17.69,-8.48,-41.74});
dataSet.push_back({-12.83,-10.33,-42.50});
dataSet.push_back({-6.47,-11.79,-43.91});
dataSet.push_back({-2.42,-11.67,-43.52});
dataSet.push_back({4.48,-10.63,-45.68});
dataSet.push_back({12.10,-8.96,-46.07});
dataSet.push_back({17.07,-5.88,-45.96});
dataSet.push_back({21.25,-1.60,-45.02});
dataSet.push_back({22.12,0.32,-45.98});
dataSet.push_back({25.09,3.87,-45.68});
dataSet.push_back({26.34,6.34,-46.04});
dataSet.push_back({26.62,7.84,-44.63});
dataSet.push_back({26.81,9.36,-45.46});
dataSet.push_back({25.95,8.95,-44.80});
dataSet.push_back({26.38,8.54,-44.73});
dataSet.push_back({25.27,8.17,-43.09});
dataSet.push_back({24.69,7.62,-42.15});
dataSet.push_back({23.95,7.04,-38.50});
dataSet.push_back({21.90,6.13,-36.93});
dataSet.push_back({21.06,6.57,-35.27});
dataSet.push_back({19.51,5.57,-32.98});
dataSet.push_back({18.04,4.81,-31.45});
dataSet.push_back({15.97,4.60,-31.45});
dataSet.push_back({14.32,5.15,-28.63});
dataSet.push_back({11.00,3.17,-29.41});
dataSet.push_back({8.42,1.48,-29.60});
dataSet.push_back({6.27,1.50,-28.96});
dataSet.push_back({5.30,-1.03,-30.20});
dataSet.push_back({2.93,-3.43,-31.18});
dataSet.push_back({2.66,-6.62,-33.41});
dataSet.push_back({0.26,-8.22,-32.39});
dataSet.push_back({0.01,-8.59,-32.06});
dataSet.push_back({-1.33,-7.03,-30.53});
dataSet.push_back({0.38,-5.75,-29.24});
dataSet.push_back({2.06,-2.54,-28.03});
dataSet.push_back({6.46,0.99,-27.81});
dataSet.push_back({11.13,2.84,-27.81});
dataSet.push_back({15.38,4.79,-29.85});
dataSet.push_back({19.75,6.69,-32.52});
dataSet.push_back({22.75,7.36,-35.52});
dataSet.push_back({25.00,8.81,-39.82});
dataSet.push_back({26.48,10.08,-43.80});
dataSet.push_back({27.93,11.56,-48.08});
dataSet.push_back({26.04,13.00,-55.10});
dataSet.push_back({26.26,14.94,-59.70});
dataSet.push_back({24.79,16.50,-62.46});
dataSet.push_back({21.79,18.14,-66.63});
dataSet.push_back({19.13,17.20,-69.07});
dataSet.push_back({14.36,16.78,-72.45});
dataSet.push_back({10.91,16.43,-74.21});
dataSet.push_back({10.51,16.26,-75.16});
dataSet.push_back({9.02,17.96,-75.95});
dataSet.push_back({6.14,18.44,-76.89});
dataSet.push_back({5.54,18.02,-76.64});
dataSet.push_back({4.80,17.04,-75.73});
dataSet.push_back({4.98,16.15,-76.22});
dataSet.push_back({6.84,15.25,-75.32});
dataSet.push_back({10.16,12.84,-73.16});
dataSet.push_back({11.91,11.30,-72.72});
dataSet.push_back({15.76,9.67,-67.64});
dataSet.push_back({19.87,7.13,-63.20});
dataSet.push_back({20.86,4.77,-57.57});
dataSet.push_back({22.20,4.06,-56.77});
dataSet.push_back({22.83,3.55,-54.04});
dataSet.push_back({23.30,4.10,-53.93});
dataSet.push_back({23.79,4.14,-51.72});
dataSet.push_back({22.40,2.91,-49.35});
dataSet.push_back({20.65,0.73,-45.78});
dataSet.push_back({20.53,0.49,-44.64});
dataSet.push_back({19.89,-0.51,-45.70});
dataSet.push_back({19.34,-1.00,-45.34});
dataSet.push_back({19.27,-0.89,-45.47});
dataSet.push_back({19.47,-0.46,-46.01});
dataSet.push_back({20.64,0.48,-45.89});
dataSet.push_back({21.57,1.95,-46.09});
dataSet.push_back({23.02,4.50,-46.03});
dataSet.push_back({25.99,9.75,-46.55});
dataSet.push_back({27.01,12.79,-47.50});
dataSet.push_back({27.44,18.46,-46.23});
dataSet.push_back({28.42,22.83,-47.18});
dataSet.push_back({27.12,27.15,-47.05});
dataSet.push_back({26.09,33.37,-48.00});
dataSet.push_back({23.47,37.83,-49.06});
dataSet.push_back({21.39,41.71,-48.24});
dataSet.push_back({17.96,44.46,-50.46});
dataSet.push_back({15.82,46.34,-49.38});
dataSet.push_back({13.31,48.10,-49.63});
dataSet.push_back({11.24,49.44,-49.48});
dataSet.push_back({8.74,50.27,-49.57});
dataSet.push_back({8.74,51.00,-50.84});
dataSet.push_back({9.23,50.40,-48.97});
dataSet.push_back({11.06,49.87,-48.54});
dataSet.push_back({14.05,46.74,-49.32});
dataSet.push_back({18.59,43.40,-48.00});
dataSet.push_back({21.74,39.29,-46.26});
dataSet.push_back({25.52,34.02,-47.06});
dataSet.push_back({27.21,28.96,-46.29});
dataSet.push_back({28.49,22.60,-47.44});
dataSet.push_back({27.93,17.24,-47.62});
dataSet.push_back({28.12,12.01,-48.49});
dataSet.push_back({26.34,7.35,-48.43});
dataSet.push_back({23.45,2.05,-49.37});
dataSet.push_back({19.63,-2.55,-48.45});
dataSet.push_back({15.39,-6.35,-48.49});
dataSet.push_back({9.20,-9.20,-49.50});
dataSet.push_back({3.36,-11.00,-48.54});
dataSet.push_back({-3.05,-11.62,-49.26});
dataSet.push_back({-9.11,-11.41,-49.08});
dataSet.push_back({-13.77,-10.90,-46.89});
dataSet.push_back({-16.32,-10.18,-45.34});
dataSet.push_back({-16.75,-9.73,-44.91});
dataSet.push_back({-18.07,-9.44,-43.86});
dataSet.push_back({-17.27,-10.23,-44.41});
dataSet.push_back({-15.20,-10.78,-43.54});
dataSet.push_back({-11.30,-11.40,-43.40});
dataSet.push_back({-7.07,-11.73,-43.33});
dataSet.push_back({-2.16,-11.56,-44.63});
dataSet.push_back({1.46,-11.39,-43.49});
dataSet.push_back({5.96,-10.17,-44.01});
dataSet.push_back({9.94,-8.68,-43.31});
dataSet.push_back({14.20,-6.12,-42.49});
dataSet.push_back({17.02,-3.80,-41.61});
dataSet.push_back({20.07,-1.57,-42.74});
dataSet.push_back({21.86,1.67,-44.14});
dataSet.push_back({24.67,5.15,-44.12});
dataSet.push_back({26.21,8.39,-44.86});
dataSet.push_back({27.71,11.62,-45.95});
dataSet.push_back({27.34,13.85,-47.12});
dataSet.push_back({27.67,16.03,-45.92});
dataSet.push_back({27.42,16.27,-48.32});
dataSet.push_back({27.76,16.33,-47.29});
dataSet.push_back({27.76,16.44,-48.00});
dataSet.push_back({27.58,15.87,-48.05});
dataSet.push_back({27.65,15.64,-48.39});
dataSet.push_back({27.49,14.28,-46.24});
dataSet.push_back({26.57,13.43,-44.90});
dataSet.push_back({25.84,11.21,-41.97});
dataSet.push_back({23.38,8.18,-40.12});
dataSet.push_back({21.50,6.10,-38.15});
dataSet.push_back({19.25,4.39,-36.69});
dataSet.push_back({16.98,3.16,-34.77});
dataSet.push_back({14.79,2.31,-33.47});
dataSet.push_back({11.58,0.22,-31.64});
dataSet.push_back({8.11,-2.14,-31.63});
dataSet.push_back({5.58,-4.42,-29.87});
dataSet.push_back({3.16,-5.91,-28.53});
dataSet.push_back({1.05,-6.91,-26.87});
dataSet.push_back({0.51,-6.19,-23.63});
dataSet.push_back({0.51,-5.19,-21.52});
dataSet.push_back({-0.17,-3.85,-20.15});
dataSet.push_back({0.11,-3.99,-20.68});
dataSet.push_back({2.08,-1.54,-20.74});
dataSet.push_back({4.90,1.81,-19.25});
dataSet.push_back({9.19,4.76,-22.11});
dataSet.push_back({13.64,7.67,-24.08});
dataSet.push_back({18.69,8.80,-27.10});
dataSet.push_back({21.53,10.50,-30.22});
dataSet.push_back({23.95,11.35,-33.76});
dataSet.push_back({26.54,12.71,-35.95});
dataSet.push_back({28.76,13.62,-42.77});
dataSet.push_back({28.19,13.98,-49.04});
dataSet.push_back({28.53,13.72,-54.06});
dataSet.push_back({24.95,13.94,-58.78});
dataSet.push_back({23.58,13.16,-63.54});
dataSet.push_back({19.69,13.15,-67.87});
dataSet.push_back({15.72,13.43,-70.72});
dataSet.push_back({11.44,13.89,-72.99});
dataSet.push_back({7.46,14.44,-74.73});
dataSet.push_back({4.50,15.65,-75.71});
dataSet.push_back({1.98,16.67,-77.06});
dataSet.push_back({-1.34,15.53,-77.21});
dataSet.push_back({-2.69,14.72,-77.28});
dataSet.push_back({-1.10,14.23,-77.28});
dataSet.push_back({-0.40,13.62,-76.77});
dataSet.push_back({2.42,11.76,-75.61});
dataSet.push_back({6.93,9.48,-73.96});
dataSet.push_back({10.10,7.98,-71.01});
dataSet.push_back({11.00,6.74,-71.04});
dataSet.push_back({13.81,5.42,-66.71});
dataSet.push_back({17.60,3.96,-62.33});
dataSet.push_back({20.47,3.05,-58.20});
dataSet.push_back({21.40,3.32,-53.22});
dataSet.push_back({22.22,3.17,-49.93});
dataSet.push_back({22.71,3.09,-46.51});
dataSet.push_back({22.63,2.97,-46.02});
dataSet.push_back({22.58,3.20,-44.57});
dataSet.push_back({22.01,2.76,-44.37});
dataSet.push_back({21.86,2.54,-44.12});
dataSet.push_back({22.11,2.80,-43.73});
dataSet.push_back({21.13,1.54,-43.87});
dataSet.push_back({22.52,3.02,-44.06});
dataSet.push_back({23.67,4.53,-43.42});
dataSet.push_back({25.48,6.97,-43.07});
dataSet.push_back({25.71,8.04,-44.08});
dataSet.push_back({27.16,10.75,-43.29});
dataSet.push_back({28.48,14.75,-44.68});
dataSet.push_back({28.91,19.80,-44.93});
dataSet.push_back({28.74,25.95,-46.48});
dataSet.push_back({26.54,33.00,-47.61});
dataSet.push_back({24.71,37.94,-48.80});
dataSet.push_back({20.95,42.88,-48.69});
dataSet.push_back({18.59,44.93,-48.00});
dataSet.push_back({15.49,47.19,-47.57});
dataSet.push_back({14.14,48.57,-49.26});
dataSet.push_back({11.02,49.46,-48.93});
dataSet.push_back({9.34,50.81,-49.82});
dataSet.push_back({7.84,50.89,-49.71});
dataSet.push_back({9.35,50.18,-50.34});
dataSet.push_back({11.02,48.99,-50.52});
dataSet.push_back({14.89,46.97,-48.80});
dataSet.push_back({20.06,40.93,-48.19});
dataSet.push_back({23.43,36.57,-48.63});
dataSet.push_back({26.21,29.98,-47.60});
dataSet.push_back({28.63,22.64,-47.31});
dataSet.push_back({27.55,13.75,-45.72});
dataSet.push_back({25.20,6.26,-43.59});
dataSet.push_back({22.57,1.25,-42.53});
dataSet.push_back({18.70,-3.98,-43.13});
dataSet.push_back({14.93,-6.97,-42.65});
dataSet.push_back({8.11,-10.58,-42.59});
dataSet.push_back({4.14,-13.42,-42.99});
dataSet.push_back({-1.88,-17.18,-39.22});
dataSet.push_back({-9.05,-27.76,-29.90});
    //std::vector<std::vector<float>> parsedCsv = parseCSV();
    dataSet = cleanDataset(dataSet);
    std::vector<std::vector<float>> testingData = dataSet;
    int testparam = 0;
    int n_tests = 1;
    float scores[dataSet.size()] = {0};

    /*int final_rows = int(dataSet.size() * 0.25);
    std::cout << final_rows << std::endl;
    std::vector<std::vector<float>> final_instances;
    for (size_t i = 0; i < final_rows; i++)
    {
        final_instances.push_back({dataSet[999-(final_rows-i)]});
    }
    int deletCount = 0;
    while (deletCount < final_rows)
    {
        dataSet.pop_back();
        deletCount++;
    } */
    
    while (testparam < n_tests)
    {
        srand(time(0));
        for (size_t i = 0; i < NUMBER_OF_TREES; i++)
        {
            SampleMondrianTree(LIFE_TIME, dataSet);
            Forest.emplace_back(Tree);
            std::vector<Node>().swap(Tree);
        }

        std::cout << testparam + 1 << " Forests built" << std::endl;

        /*for (size_t i = 0; i < final_instances.size(); i++)
        {
            ExtendMondrianTree(10, final_instances[i]);
        }
        
        std::cout << testparam + 1 << " Forests extended" << std::endl;
        */

        std::vector<float> anomalyScores = decision_function(Forest, testingData);
        for (size_t i = 0; i < anomalyScores.size(); i++)
        {
            scores[i] += anomalyScores[i]/n_tests;
        }
        
        std::cout << testparam + 1 << " Scores saved" << std::endl;
        testparam++;
        std::vector<std::vector<Node>>().swap(Forest);
    }
    
    for (size_t i = 0; i < testingData.size(); i++)
    {
        std::cout << scores[i] << std::endl;
    }

}