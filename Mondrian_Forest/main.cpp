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
    int id, parent_id, child_left_id, child_right_id, split_dimension;
};

std::vector<std::vector<Node>> Forest;
std::vector<Node> Tree;


/*
    Helper methods and variables
*/
int node_counter;
int data_dim;
const int NUMBER_OF_TREES = 100;
const int SAMPLE_SIZE = 256;
const float LIFE_TIME = 10;

/*
    Takes a .CSV file and parses the rows into a vector of floats
    path has to be changed for this version of the code
*/
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

std::vector<std::vector<float>> cleanDataset(std::vector<std::vector<float>> data)
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

Node findChild(std::vector<Node> tree, int child_id)
{
    
    for (size_t i = 0; i < tree.size(); i++)
    {
        if (tree[i].id == child_id)
        {
            return tree[i];
        }
    }  
}

float exp_dist(float lamda)
{
    std::default_random_engine generator;
    std::exponential_distribution<double> distribution(lamda);
    int randLoops = rand() % 1000;
    int i = 0;
    while (i < randLoops)
    {
        distribution(generator);
        i++;
    }
    return distribution(generator);
}

int get_dim(Node node, float lamda)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    float pick = dis(gen);
    
    float limit;

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
    Mondrian implementation
*/
//Algorithm 2
void SampleMondrianBlock(Node j, float life_time, float t_parent)
{
    //Line 1
    //Line 2
    float lamda = 0;
    float min = 0;
    float max = 0;
    
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
        j.l_d.push_back(min);
        j.u_d.push_back(max);
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
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(lower, upper);
        float split_location = dis(gen);


        //Line 8
        
        
        //Save node features
        j.split_dimension = sample_dimension;
        j.split_location = split_location;
        
        Tree.push_back(j);
        
        //Line 9
        Node left_child;
        node_counter++;
        left_child.id = node_counter;
        j.child_left_id = left_child.id;

        for (size_t i = 0; i < j.data.size(); i++)
        {
            if(j.data[i][sample_dimension] <= split_location)
            {
                left_child.data.push_back(j.data[i]);
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
                right_child.data.push_back(j.data[i]);
            }
        }
        right_child.parent_id = j.id;
        Tree[right_child.parent_id].child_right_id = right_child.id;
        SampleMondrianBlock(right_child, life_time, j.split_time);
        
    }
    else
    {
        //Line 12
        j.split_time = life_time;
        j.leaf = true;
        j.child_left_id = -1;
        j.child_right_id = -1;
        j.id = node_counter;
        Tree.push_back(j);
    }
}

//Algorithm 1
void SampleMondrianTree(float life_time, std::vector<std::vector<float>> data)
{
    Node root;
    node_counter = 0;
    root.id = node_counter;
    root.data = data;
    root.split_time = 0;
    data_dim = data[0].size();
    SampleMondrianBlock(root, life_time, root.split_time);
}

//Algorithm 4
void ExtendMondrianBlock(float life_time, Node node, std::vector<float> instance, float t_parent)
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
            std::uniform_real_distribution<> dis(node.u_d[node.split_dimension], instance[node.split_dimension]);
            split_location = dis(gen);
        }
        else
        {
            std::uniform_real_distribution<> dis(instance[node.split_dimension], node.l_d[node.split_dimension]);
            split_location = dis(gen);
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
        
        //Fel, ska jämföras med instancen min(l_d, x), kan va löst med loopen
        //newParent.l_d = node.l_d;
        //newParent.u_d = node.u_d;
        for (size_t i = 0; i < instance.size(); i++)
            {
                if (node.l_d[i] < instance[i])
                {
                    newParent.l_d.push_back(node.l_d[i]);
                }
                else
                {
                    newParent.l_d.push_back(instance[i]);
                }

                if (node.u_d[i] > instance[i])
                {
                    newParent.u_d.push_back(node.u_d[i]);
                }
                else
                {
                    newParent.u_d.push_back(instance[i]);
                }
            }

        Node newLeaf;
        node_counter++;
        newLeaf.id = node_counter;
        newLeaf.leaf = true;
        newLeaf.parent_id = newParent.id;

        newLeaf.data.push_back(instance);

        if (instance[newParent.split_dimension] < newParent.split_location)
        {
            newParent.child_left_id = node.id;
            newParent.child_right_id = newLeaf.id;

            Tree[newParent.parent_id].child_left_id = newParent.id;

            Tree.push_back(newParent);
            SampleMondrianBlock(newLeaf, life_time, t_parent);
        }
        else
        {
            newParent.child_right_id = node.id;
            newParent.child_left_id = newLeaf.id;

            Tree[newParent.parent_id].child_right_id = newParent.id;

            Tree.push_back(newParent);
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
void ExtendMondrianTree(float life_time, std::vector<float> instance)
{
    ExtendMondrianBlock(life_time, Tree[0], instance, 0);
}

int main()
{
    std::vector<std::vector<float>> data;
    std::vector<std::vector<float>> parsedCsv = parseCSV();
    parsedCsv = cleanDataset(parsedCsv);
    data.push_back({11, -11, 7, 4, 0});
    data.push_back({8, -6, -5, -1, 4});
    data.push_back({100, -5, -1, -12, 6});
    data.push_back({-6, -18, 11, 15, -8});
    /*data.push_back({-31, -22, 71, -40, -7});
    data.push_back({-5, 34, 92, 76, 38});
    data.push_back({-46, 33, 66, -31, -97});
    data.push_back({-41, 79, -57, 63, 1});
    data.push_back({60, 51, 91, 21, 62});
    data.push_back({54, 35, -13, -48, -36});
    data.push_back({51, 65, 13, -68, 36});
    data.push_back({74, 85, 23, 28, 33});*/
    
    
    std::vector<float> last_row;
    last_row = parsedCsv[128];
    parsedCsv.pop_back();


    for (size_t i = 0; i < NUMBER_OF_TREES; i++)
    {
        SampleMondrianTree(LIFE_TIME, parsedCsv);
        Forest.push_back(Tree);
        std::vector<Node>().swap(Tree);
    }
     
    for (size_t i = 0; i < Forest.size(); i++)
    {
        Tree = Forest[i];
        ExtendMondrianTree(LIFE_TIME, last_row);
        Forest[i] = Tree;
        std::vector<Node>().swap(Tree);
    }
    
    
    
    for (size_t j = 0; j < Forest.size(); j++)
    {
        Tree = Forest[j];
        for (size_t i = 0; i < Tree.size(); i++)
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
        }
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