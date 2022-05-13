#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <map>

using namespace std;

/*
Nod ska ha block left och block right

    float l_max, l_min, split_time;
    int id, id_parent, split_threshold, split_feature;
    bool internal;
*/
struct Node
{
    std::vector<std::vector<float>> data, left_block, right_block;
    std::vector<float> l_d, u_d;
    bool leaf = false;
    float split_time;
    int id, parent_id, child_left_id, child_right_id, split_dimension, split_location;
};

std::vector<Node> Tree;

std::vector<std::vector<float>> parseCSV()
{   
    std::vector<std::vector<float>> parsedCsv;
    std::ifstream data("C:\\Users\\anton\\OneDrive\\Skrivbord\\Thesis_Code\\IsolationForestTinyML\\DatSets\\breastw.csv");
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

std::vector<std::vector<float>> random_unique(std::vector<std::vector<float>> data, int sample)
{
    std::vector<std::vector<float>> sub_sample;
    for (size_t i = 0; i < sample; i++)
    {
        int random = rand() % data.size();
        sub_sample.push_back(data[random]);
    }
    return sub_sample;
}

float exponential_dist(float lamda)
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

int main()
{


    std::vector<std::vector<float>> data;
    data.push_back({1,2,3,4,5,6});
    data.push_back({1323,422,533,44,5,436});
    data.push_back({154234,2,35452,4,5,66});
    data.push_back({761,2,31324,4,6435,64674});
    data.push_back({154,652,983,476,564,766});

    std::vector<float> instance;
    instance.push_back(0);
    instance.push_back(532);
    instance.push_back(421);
    instance.push_back(50);
    instance.push_back(103);
    instance.push_back(502);
    float e_l = 0;
    float e_u = 0;
    
    for (size_t i = 0; i < instance.size(); i++)
    {
        for (size_t j = 0; j < data.size(); j++)
        {
            float min = 0;
            float max = 0;
            if (data[j][i] - instance[i] > 0)
            {
                min = data[j][i];
            }

            if (instance[i] - data[j][i] > 0)
            {
                max = data[j][i];
            }
            e_l += min;
            e_u += max;
        }
    }
    
    //Line 2
    float lamda = e_l + e_u;
    float E = exponential_dist(lamda);
    std::vector<int> weights;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    float pick = dis(gen);

    if (0 <= pick < (node.u_d[0] + node.l_d[0]))
    {
        return 0;
    }
    for (size_t i = 1; i < node.u_d.size(); i++)
    {
        if ((node.u_d[i-1] + node.l_d[i-1]) < pick < (node.u_d[i] + node.l_d[i]))
        {
            return i;
        }
    }
    return 0;
}





