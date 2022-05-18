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
    std::ifstream data("C:\\Users\\anton\\OneDrive\\Skrivbord\\Thesis_Code\\IsolationForestTinyML\\DatSets\\glass.csv");
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


int main()
{
    std::vector<std::vector<float>> hej;
    std::vector<std::vector<float>> test = parseCSV();
    std::cout << test.size() << std::endl;

    test = cleanDataset(test);
    std::cout << test.size() << std::endl;
    hej.push_back({1.0,1.0,2.0});
    hej.push_back({1.0,1.0,2.0});
    hej.push_back({1.0,1.0,2.0});
    if(hej[0] == hej[1])
    {
        std::cout << "a" << std::endl;
    }
    return 0;
}





