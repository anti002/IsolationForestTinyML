#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

struct Tree{
    int child_id_left, child_id_right;
    int feature;
    int n_samples;
    float threshold;
};

struct Data{
    float a, b, c, d, e, f, g, h, i, j ,k, l, m;
};

float c(int size){

    if (size > 2){
        return (2 * (log(size -1) + 0.5772156649)) - (2*(size-1)/size);
    }
    if (size == 2){
        return 1;
    }
    return 0;
}



std::vector<vector<Data>> dataSet;
std::vector<vector<Tree>> iForest;
std::vector<Tree> iTree1;
std::vector<Tree> iTree2;
std::vector<Tree> iTree3;
std::vector<Tree> iTree4;
std::vector<Tree> iTree5;
std::vector<Tree> iTree6;
std::vector<Tree> iTree7;
std::vector<Tree> iTree8;
std::vector<Tree> iTree9;
std::vector<Tree> iTree10;
std::vector<Data> data;

int main(){

    data.push_back({13.2900000000000	,1.97000000000000	,2.68000000000000	,16.8000000000000	,102,	3,	3.23000000000000,	0.310000000000000,	1.66000000000000,	6,	1.07000000000000,	2.84000000000000,	1270});
    data.push_back({14.3000000000000,1.92000000000000,2.72000000000000,20,120,2.80000000000000,3.14000000000000,0.330000000000000,1.97000000000000,6.20000000000000,1.07000000000000,2.65000000000000,1280});
    dataSet.push_back(data);

    float ndimData[1][12];

    ndimData[0][0] = 13.2900000000000;
    ndimData[0][1] = 1.97000000000000;
    ndimData[0][2] = 2.68000000000000;
    ndimData[0][3] = 16.8000000000000;
    ndimData[0][4] = 102;
    ndimData[0][5] = 3;
    ndimData[0][6] = 3.23000000000000;
    ndimData[0][7] = 0.310000000000000;
    ndimData[0][8] = 1.66000000000000;
    ndimData[0][9] = 6;
    ndimData[0][10] = 1.07000000000000;
    ndimData[0][11] = 2.84000000000000;
    ndimData[0][12] = 1270;

    ndimData[1][0] = 14.3000000000000;
    ndimData[1][1] = 1.92000000000000;
    ndimData[1][2] = 2.72000000000000;
    ndimData[1][3] = 20;
    ndimData[1][4] = 120;
    ndimData[1][5] = 2.80000000000000;
    ndimData[1][6] = 3.14000000000000;
    ndimData[1][7] = 0.330000000000000;
    ndimData[1][8] = 1.97000000000000;
    ndimData[1][9] = 6.20000000000000;
    ndimData[1][10] = 1.07000000000000;
    ndimData[1][11] = 2.65000000000000;
    ndimData[1][12] = 1280;
    
    ndimData[2][0] = 13.6800000000000;
    ndimData[2][1] = 1.83000000000000;
    ndimData[2][2] = 2.36000000000000;
    ndimData[2][3] = 17.2000000000000;
    ndimData[2][4] = 104;
    ndimData[2][5] = 2.42000000000000;
    ndimData[2][6] = 2.69000000000000;
    ndimData[2][7] = 0.420000000000000;
    ndimData[2][8] = 1.97000000000000;
    ndimData[2][9] = 3.84000000000000;
    ndimData[2][10] = 1.23000000000000;
    ndimData[2][11] = 2.87000000000000;
    ndimData[2][12] = 990;

    iTree1.push_back({1, 4, 0, 3, 13.771827205440342});
    iTree1.push_back({2, 3, 3, 2, 16.82208635181144});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree1);

    iTree2.push_back({1, 4, 7, 3, 0.34224062699265073});
    iTree2.push_back({2, 3, 8, 2, 1.8118507844859353});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree2);

    iTree3.push_back({1, 2, 5, 3, 2.595825418461203});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({3, 4, 2, 2, 2.7103437290929557});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree3);

    iTree4.push_back({1, 2, 6, 3, 2.952434820332614});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({3, 4, 2, 2, 2.690622230813098});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree4);

    iTree5.push_back({1, 2, 6, 3, 2.809792329299664});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({3, 4, 2, 2, 2.704136673990785});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree5);

    iTree6.push_back({1, 2, 8, 3, 1.7683328147365456});
    iTree6.push_back({0, 0, -2, 1, -2.0});
    iTree6.push_back({3, 4, 12, 2, 1103.5665372775711});
    iTree6.push_back({0, 0, -2, 1, -2.0});
    iTree6.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree6);

    iTree7.push_back({1, 2, 7, 3, 0.3147047230514291});
    iTree7.push_back({0, 0, -2, 1, -2.0});
    iTree7.push_back({3, 4, 4, 2, 114.8050385838398});
    iTree7.push_back({0, 0, -2, 1, -2.0});
    iTree7.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree7);

    iTree8.push_back({1, 4, 0, 3, 13.960032604245313});
    iTree8.push_back({2, 3, 5, 2, 2.707890695242092});
    iTree8.push_back({0, 0, -2, 1, -2.0});
    iTree8.push_back({0, 0, -2, 1, -2.0});
    iTree8.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree8);

    iTree9.push_back({1, 2, 8, 3, 1.688364058857994});
    iTree9.push_back({0, 0, -2, 1, -2.0});
    iTree9.push_back({3, 4, 6, 2, 3.10473297533556});
    iTree9.push_back({0, 0, -2, 1, -2.0});
    iTree9.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree9);

    iTree10.push_back({1, 2, 8, 3, 1.8247949987980339});
    iTree10.push_back({0, 0, -2, 1, -2.0});
    iTree10.push_back({3, 4, 10, 2, 1.149160814050181});
    iTree10.push_back({0, 0, -2, 1, -2.0});
    iTree10.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree10);

    std::vector<float> edges;
    //3 should be dataSet.size()
    for (int i = 0; i < 3; i++)
    {

        std::vector<float> path;

        for (int j = 0; j < iForest.size(); j++)
        {
            std::vector<Tree> tree = iForest[j];
            int current_node_id = 0;
            int length = 0;
            //With the OR j == 0 we ensure that the first iteration will go through since first
            //always has the id of 0
            while (current_node_id != 0 || length == 0)
            {
                if (ndimData[i][tree[current_node_id].feature] < tree[current_node_id].threshold &&
                    tree[current_node_id].threshold != -2)
                {
                    current_node_id = tree[current_node_id].child_id_left;
                    length += 1;
                }
                else if (ndimData[i][tree[current_node_id].feature] >= tree[current_node_id].threshold &&
                    tree[current_node_id].threshold != -2)
                {
                    current_node_id = tree[current_node_id].child_id_right;
                    length += 1;
                }    
                else
                {
                    break;
                }            
            }
            int leaf_size = tree[current_node_id].n_samples;
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
        score = pow(2, -average_length[i]/c(256));
        float scorep =  -1 * score + 0.4775;
        scores.push_back(scorep);
    }

    for (size_t i = 0; i < scores.size(); i++)
    {
        //std::cout << average_length[i] << std::endl;
        //std::cout << c(256) << std::endl;
        //std::cout << i;
        std::cout << scores[i] << std::endl;
    }
    
}