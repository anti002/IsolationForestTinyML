#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>

using namespace std;

/*
Nod ska ha block left och block right

    float l_max, l_min, split_time;
    int id, id_parent, split_threshold, split_feature;
    bool internal;
*/
struct Node
{
    std::vector<std::vector<float>> data;
    std::vector<std::vector<float>> left_block;
    std::vector<std::vector<float>> right_block;
    bool leaf = false;
    float split_time;
    int parent_id;
};

std::vector<Node> Tree;

void SampleMondrianTree(float life_time, std::vector<std::vector<float>> data)
{
    Node root;
    root.data = data;
    SampleMondrianBlock(root, life_time, 0);
}


/*
D_N(j) är all info i noden, därav behövs inte parametern om vi endast skickar noden istället.
*/
void SampleMondrianBlock(Node j, float life_time, float t_parent)
{
    Tree.push_back(j);

    float rate = 0;

    for (size_t i = 0; i < j.data[0].size(); i++)
    {
        float upper = *max_element(j.data[i].begin(), j.data[i].end());
        float lower = *min_element(j.data[i].begin(), j.data[i].end());

        rate += (upper - lower);
    }

    float E = exp_dist(rate, j.data.size());
    
    if (t_parent + E < life_time)
    {
        j.split_time = t_parent + E;
    
        int sample_dimension = rand() % j.data[0].size();
        
        float upper = *max_element(j.data[sample_dimension].begin(), j.data[sample_dimension].end());
        float lower = *min_element(j.data[sample_dimension].begin(), j.data[sample_dimension].end());
        
        float split_value = (rand() / ((double)RAND_MAX + 1) * (upper - lower) + lower);

        for (size_t i = 0; i < j.data.size(); i++)
        {
            if(j.data[sample_dimension][i] <= split_value)
            {
                j.left_block.push_back(j.data[sample_dimension]);
            }
            else
            {
                j.right_block.push_back(j.data[sample_dimension]);
            }
        }
        
        Node left_child;
        left_child.data = j.left_block;
        Node right_child;
        right_child.data = j.right_block;

        SampleMondrianBlock(left_child, life_time, j.split_time);
        SampleMondrianBlock(right_child, life_time, j.split_time);
        
    }
    else
    {
        j.split_time = life_time;
        j.leaf = true;
        Tree.push_back(j);
    }
}

void ExtendMondrianTree(std::vector<Node> Tree, float life_time, std::vector<float> instance)
{
    ExtendMondrianBlock(Tree, life_time, Tree[0], instance, 0);
}

void ExtendMondrianBlock(std::vector<Node> Tree, float life_time, Node node, std::vector<float> instance, float t_parent)
{
    float e_l = 0;
    float e_u = 0;

    for (size_t i = 0; i < node.data[0].size(); i++)
    {
        double min = *min_element(node.data[i].begin(), node.data[i].end());
        double max = *max_element(node.data[i].begin(), node.data[i].end());

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
    
    float sample_rate = e_l + e_u;
    float E = exp_dist(sample_rate, node.data.size());

    node.split_time = E + t_parent;

}

float exp_dist(float lamda, int samples)
{
    return lamda*exp(-lamda * samples);
}


int main()
{
    
}





