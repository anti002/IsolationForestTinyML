#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

std::vector<std::vector<std::string> > parsedCsv;

struct Tree{
    int child_id_left, child_id_right;
    int feature;
    int n_samples;
    float threshold;
};


void parseCSV()
{
    std::ifstream  data("C:\\Users\\anton\\OneDrive\\Skrivbord\\Thesis_Code\\IsolationForestTinyML\\DatSets\\test.csv");
    std::string line;
    while(std::getline(data,line))
    {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<std::string> parsedRow;
        while(std::getline(lineStream,cell,','))
        {
            parsedRow.push_back(cell);
        }

        parsedCsv.push_back(parsedRow);
    }
};


float c(float size){

    if (size > 2){
        float temp = (2 * (log(size -1) + 0.5772156649)) - (2*(size-1)/size);
        return temp;
    }
    if (size == 2){
        return 1;
    }
    return 0;
}

std::vector<vector<Tree>> iForest;
std::vector<Tree> iTree1;
int main(){

    parseCSV();

    
    iTree1.push_back({1, 70, 2, 129, 3.001993747746314});
    iTree1.push_back({2, 3, 5, 128, 1.0984851564639742});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({4, 23, 7, 127, 0.28393284182323786});
    iTree1.push_back({5, 22, 12, 30, 996.0485382965061});
    iTree1.push_back({6, 9, 0, 29, 11.801234189826335});
    iTree1.push_back({7, 8, 6, 2, 2.462179098513871});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({10, 17, 12, 27, 661.2775011599424});
    iTree1.push_back({11, 14, 0, 21, 13.339808253514153});
    iTree1.push_back({12, 13, 1, 17, 1.3756862994552208});
    iTree1.push_back({0, 0, -2, 8, -2.0});
    iTree1.push_back({0, 0, -2, 9, -2.0});
    iTree1.push_back({15, 16, 4, 4, 94.37649366556504});
    iTree1.push_back({0, 0, -2, 2, -2.0});
    iTree1.push_back({0, 0, -2, 2, -2.0});
    iTree1.push_back({18, 21, 2, 6, 2.405038238620369});
    iTree1.push_back({19, 20, 4, 5, 112.58449735100591});
    iTree1.push_back({0, 0, -2, 3, -2.0});
    iTree1.push_back({0, 0, -2, 2, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({24, 43, 2, 97, 2.1954620702932384});
    iTree1.push_back({25, 34, 12, 20, 580.7731478655586});
    iTree1.push_back({26, 27, 6, 15, 1.1482157041960317});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({28, 31, 4, 14, 96.8548106070863});
    iTree1.push_back({29, 30, 12, 11, 287.63854161726243});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 10, -2.0});
    iTree1.push_back({32, 33, 6, 3, 1.672542772913862});
    iTree1.push_back({0, 0, -2, 2, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({35, 38, 6, 5, 1.7471065887242392});
    iTree1.push_back({36, 37, 12, 2, 663.7050281947968});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({39, 40, 5, 3, 1.926022271183056});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({41, 42, 5, 2, 2.108021920431228});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({44, 59, 1, 77, 3.7524134812585794});
    iTree1.push_back({45, 52, 9, 59, 9.823295001416081});
    iTree1.push_back({46, 49, 5, 53, 2.734010337929947});
    iTree1.push_back({47, 48, 8, 44, 1.286949449167675});
    iTree1.push_back({0, 0, -2, 21, -2.0});
    iTree1.push_back({0, 0, -2, 23, -2.0});
    iTree1.push_back({50, 51, 0, 9, 13.288019892552803});
    iTree1.push_back({0, 0, -2, 4, -2.0});
    iTree1.push_back({0, 0, -2, 5, -2.0});
    iTree1.push_back({53, 56, 9, 6, 10.725589347545405});
    iTree1.push_back({54, 55, 8, 4, 1.3203436166589797});
    iTree1.push_back({0, 0, -2, 2, -2.0});
    iTree1.push_back({0, 0, -2, 2, -2.0});
    iTree1.push_back({57, 58, 7, 2, 0.5241137351697017});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({60, 67, 2, 18, 2.6440716025218216});
    iTree1.push_back({61, 64, 4, 16, 97.1213274342573});
    iTree1.push_back({62, 63, 8, 11, 0.918828054691188});
    iTree1.push_back({0, 0, -2, 2, -2.0});
    iTree1.push_back({0, 0, -2, 9, -2.0});
    iTree1.push_back({65, 66, 7, 5, 0.3868779005231979});
    iTree1.push_back({0, 0, -2, 2, -2.0});
    iTree1.push_back({0, 0, -2, 3, -2.0});
    iTree1.push_back({68, 69, 8, 2, 1.478487676481142});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree1);

    std::vector<float> edges;

    for (int i = 0; i < parsedCsv.size(); i++)
    {
        std::vector<float> path;

        for (int j = 0; j < iForest.size(); j++)
        {
            std::vector<Tree> tree = iForest[j];
            int current_node_id = 0;
            int length = 0;
            //With the OR j == 0 we ensure that the first iteration will go through since first
            //always has the id of 0
            while (length == 0 || tree[current_node_id].child_id_left != 0)
            {
                float splitValue_attribute = std::stof(parsedCsv[i][tree[current_node_id].feature]);
                float splitValue_node = tree[current_node_id].threshold;
                if (splitValue_attribute < splitValue_node)
                {
                    current_node_id = tree[current_node_id].child_id_left;
                    length += 1;
                }
                else
                {
                    current_node_id = tree[current_node_id].child_id_right;
                    length += 1;
                }
            }
            float leaf_size = tree[current_node_id].n_samples;
            
            float path_length = length + c(leaf_size);
            path.push_back(path_length);
        }

        //mean of elements in path before push into edges
        float average_path = 0;
        for(int k = 0; k < iForest.size(); k++)
        {
            average_path += path[k];
        }
        average_path = average_path/path.size();
        edges.push_back(average_path);
        path.clear();
    }

    std::vector<float> scores;
    float score = 0;
    std::vector<float> average_length = edges;
    for (int i = 0; i < average_length.size(); i ++){
        score = pow(2, (-1 * average_length[i])/c(parsedCsv.size()));
        float scorep =  0.5 - score;
        scores.push_back(scorep);
    }
    edges.clear();

    for (size_t i = 0; i < scores.size(); i++)
    {
        std::cout << scores[i] << std::endl;
    }
    
}