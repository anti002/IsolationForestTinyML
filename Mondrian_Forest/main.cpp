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

//Algorithm 1
void SampleMondrianTree(float life_time, std::vector<std::vector<float>> data)
{
    Node root;
    root.data = data;
    SampleMondrianBlock(root, life_time, 0, 0);
}


//Algorithm 2
void SampleMondrianBlock(Node j, float life_time, float t_parent, int node_id_counter)
{
    //Line 1
    j.id = node_id_counter;

    //Line 2
    float rate = 0;
    for (size_t i = 0; i < j.data[0].size(); i++)
    {
        j.l_d.push_back(*max_element(j.data[i].begin(), j.data[i].end()));
        j.u_d.push_back(*min_element(j.data[i].begin(), j.data[i].end()));

        rate += (j.u_d[i] - j.l_d[i]);
    }

    //Line 3 gör till C++ egna exp dist
    std::default_random_engine generator;
    std::exponential_distribution<double> distribution(1/rate);

    float E = exp_dist(1/rate, j.data.size());
    
    //Line 4
    if (t_parent + E < life_time)
    {
        //Line 5
        j.split_time = t_parent + E;
    
        //Line 6
        int sample_dimension = rand() % j.data[0].size();
        
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
        Node left_child;
        left_child.data = j.left_block;
        SampleMondrianBlock(left_child, life_time, j.split_time, node_id_counter++);

        //Line10
        Node right_child;
        right_child.data = j.right_block;
        SampleMondrianBlock(right_child, life_time, j.split_time, node_id_counter++);
        
    }
    else
    {
        //Line 12
        j.split_time = life_time;
        j.leaf = true;
        Tree.push_back(j);
    }
}

//Algorithm 3
void ExtendMondrianTree(std::vector<Node> Tree, float life_time, std::vector<float> instance)
{
    ExtendMondrianBlock(Tree, life_time, Tree[0], instance, 0);
}

//Algorithm 4
void ExtendMondrianBlock(std::vector<Node> Tree, float life_time, Node node, std::vector<float> instance, float t_parent)
{
    //Line 1
    float e_l = 0;
    float e_u = 0;
    for (size_t i = 0; i < node.data[0].size(); i++)
    {
        float min = *min_element(node.data[i].begin(), node.data[i].end());
        float max = *max_element(node.data[i].begin(), node.data[i].end());

        float minimum = min - instance[i];
        
        if (minimum < 0)
        {
            minimum = 0;
        }
        
        float maximum = instance[i] - max;
        
        if (maximum < 0)
        {
            maximum = 0;
        }
        e_l += minimum;
        e_u += maximum;
    }
    
    //Line 2
    float sample_rate = e_l + e_u;
    float E = exp_dist(1/sample_rate, node.data.size());

    node.split_time = E + t_parent;

    if(t_parent + E < node.split_time)
    {
        //e_l eller u_l kommer vara 0 därav adderas de och tas som probability. Kolla Lucianos kod rad 125
        //När vi sätter in en ny parent så kommer en ny leaf skapas, den ska bara ha 1 datapunkt och infinity i split time
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

float exp_dist(float lamda)
{
    std::default_random_engine generator;
    std::exponential_distribution<double> distribution(lamda);

    return distribution(generator);
}


int main()
{
    
}





