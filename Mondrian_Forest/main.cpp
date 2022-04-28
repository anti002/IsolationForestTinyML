#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

const int LIFETIME = 0;
const int N_TREES = 1;

struct Node
{
    float l_max, l_min, split_time;
    int id, id_parent, split_threshold, split_feature;
    bool internal;
};

float sample_exp(float rate)
{   
    return 1;
};

class MondrianForest
{
    public:
    int t_trees;
    std::vector<MondrianTree> estimators;

    MondrianForest (int t_trees)
    {
        this->t_trees = t_trees;
    }

    std::vector<MondrianTree> fit()
    {
        for (size_t i = 0; i < t_trees; i++)
        {
            
        }

        return estimators;
        
    };
};

class MondrianTree
{
    public:
    std::vector<Node> nodes;

    void fit()
    {

    }

    void extendTree(int lambda, Node node, std::vector<float> instance)
    {
        
    }
};

int main()
{
    MondrianForest forest = MondrianForest(N_TREES);
    //TODO: Change true to while data stream
    while(true)
    {
        forest.fit();
    }
}
