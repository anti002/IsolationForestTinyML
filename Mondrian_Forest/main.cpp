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
    std::vector<std::vector<float>> data, left_block, right_block;
    std::vector<float> l_d, u_d;
    bool leaf = false;
    float split_time;
    int id, parent_id, child_left_id, child_right_id, split_dimension, split_location;
};

std::vector<Node> Tree;


/*
    Helper methods and variables
*/
int node_counter;

Node findChild(std::vector<Node> tree, int child_id)
{
    Node found_child;
    for (size_t i = 0; i < tree.size(); i++)
    {
        if (tree[i].id == child_id)
        {
            found_child = tree[i];
        }
        return found_child;    
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

    float limit = 0;
    for (size_t i = 0; i < node.u_d.size(); i++)
    {
        limit += (node.u_d[i] + node.l_d[i]);
        if (limit > pick)
        {
            return i;
        }
    }
}

void set_child_id(Node child, bool left)
{
    for (size_t i = 0; i < Tree.size(); i++)
    {
        if (Tree[i].id == child.parent_id && left)
        {
            Tree[i].child_left_id = child.id;
        }

        if (Tree[i].id == child.parent_id && !left)
        {
            Tree[i].child_right_id = child.id;
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
    Tree.push_back(j);
    //Line 2
    float lamda = 0;

    for (size_t i = 0; i < j.data[0].size(); i++)
    {
        for (size_t k = 0; k < j.data.size(); k++)
        {
            float min = 0;
            float max = 0;
            if (j.data[k][i] < min)
            {
                min = j.data[k][i];
            }

            if (j.data[k][i] > max)
            {
                max = j.data[k][i];
            }

            j.l_d.push_back(min);
            j.u_d.push_back(max);

            lamda += (max - min);
        }
    }

    //Line 3 gör till C++ egna exp dist

    float E = exp_dist(lamda);
    
    //Line 4
    //if (t_parent + E < life_time)
    if(j.data.size() > 1)
    {
        //Line 5
        j.split_time = t_parent + E;
    
        //Line 6
        int sample_dimension = get_dim(j, lamda);
        
        //Line 7
        float upper = *max_element(j.data[sample_dimension].begin(), j.data[sample_dimension].end());
        float lower = *min_element(j.data[sample_dimension].begin(), j.data[sample_dimension].end());
        
        float split_location = (rand() / ((double)RAND_MAX + 1) * (upper - lower) + lower);

        //Line 8
        for (size_t i = 0; i < j.data.size(); i++)
        {
            if(j.data[sample_dimension][i] <= split_location)
            {
                j.left_block.push_back(j.data[sample_dimension]);
            }
            else
            {
                j.right_block.push_back(j.data[sample_dimension]);
            }
        }
        
        //Save node features
        j.split_dimension = sample_dimension;
        j.split_location = split_location;

        Tree.push_back(j);

        //Line 9
        //Behöver hitta föräldern till nya föräldern och ändra på dens child_id
        Node left_child;
        node_counter++;
        left_child.id = node_counter;
        j.child_left_id = left_child.id;

        left_child.data = j.left_block;
        left_child.parent_id = j.id;
        SampleMondrianBlock(left_child, life_time, j.split_time);

        //Line10
        //Behöver hitta föräldern till nya föräldern och ändra på dens child_id
        Node right_child;
        node_counter++;
        right_child.id = node_counter;
        j.child_right_id = right_child.id;
        
        right_child.data = j.right_block;
        right_child.parent_id = j.id;
        SampleMondrianBlock(right_child, life_time, j.split_time);
        
    }
    else
    {
        //Line 12
        j.split_time = life_time;
        j.leaf = true;
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
    SampleMondrianBlock(root, life_time, 0);
}

//Algorithm 4
void ExtendMondrianBlock(std::vector<Node> Tree, float life_time, Node node, std::vector<float> instance, float t_parent)
{
    //Line 1
    float e_l = 0;
    float e_u = 0;

    for (size_t i = 0; i < instance.size(); i++)
    {
        for (size_t j = 0; j < node.data.size(); j++)
        {
            float min = 0;
            float max = 0;
            if (node.data[j][i] - instance[i] > 0)
            {
                min = node.data[j][i];
            }

            if (instance[i] - node.data[j][i] > 0)
            {
                max = node.data[j][i];
            }

            node.l_d.push_back(min);
            node.u_d.push_back(max);

            e_l += min;
            e_u += max;
        }
    }
    
    //Line 2
    float lamda = e_l + e_u;
    float E = exp_dist(lamda);

    node.split_time = E + t_parent;

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
        node.parent_id = newParent.id;

        newParent.split_dimension = split_dimension;
        newParent.split_location = split_location;
        newParent.split_time = t_parent + E;
        newParent.l_d = node.l_d;
        newParent.u_d = node.u_d;

        
        Node newLeaf;
        node_counter++;
        newLeaf.id = node_counter;
        newLeaf.leaf = true;
        newLeaf.parent_id = newParent.id;
        //Might be incorrect, but it adds one point as it should to the leaf.
        newLeaf.data.push_back(instance);

        if (instance[newParent.split_dimension] < newParent.split_location)
        {
            newParent.child_left_id = newLeaf.id;
            newParent.child_right_id = node.id;

            Tree.push_back(newParent);
            SampleMondrianBlock(newLeaf, life_time, t_parent);
        }
        else
        {
            newParent.child_right_id = newLeaf.id;
            newParent.child_left_id = node.id;

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
                ExtendMondrianBlock(Tree, life_time, findChild(Tree, node.child_left_id), instance, node.split_time);
            }
            else
            {
                //Line 14
                ExtendMondrianBlock(Tree, life_time, findChild(Tree, node.child_right_id), instance, node.split_time);
            }
        }   
    }
}

//Algorithm 3
void ExtendMondrianTree(std::vector<Node> Tree, float life_time, std::vector<float> instance)
{
    ExtendMondrianBlock(Tree, life_time, Tree[0], instance, 0);
}

int main()
{
    std::vector<std::vector<float>> data;
    data.push_back({-1,2,3,-4,5,6});
    data.push_back({1323,-422,533,44,5,436});
    data.push_back({154234,-2,35452,4,5,66});
    data.push_back({761,2,31324,4,-435,64674});
    data.push_back({-154,652,983,476,-64,766});

    SampleMondrianTree(3, data);
}