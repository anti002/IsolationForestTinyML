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

int c(int size){

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

    iTree1.push_back({1, 72, 9, 129, 8.512631575962718});
    iTree1.push_back({2, 67, 6, 113, 3.354350859158715});
    iTree1.push_back({3, 38, 10, 110, 1.201983565315688});
    iTree1.push_back({4, 11, 7, 91, 0.17383320749673187});
    iTree1.push_back({5, 8, 7, 4, 0.16515764821816675});
    iTree1.push_back({6, 7, 3, 2, 21.37882133614217});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({9, 10, 4, 2, 102.20460538016846});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({12, 25, 5, 87, 1.5453710968757037});
    iTree1.push_back({13, 20, 8, 18, 1.4049271295354115});
    iTree1.push_back({14, 17, 5, 14, 1.430484772958443});
    iTree1.push_back({15, 16, 8, 9, 0.8555552436036673});
    iTree1.push_back({0, 0, -2, 6, -2.0});
    iTree1.push_back({0, 0, -2, 3, -2.0});
    iTree1.push_back({18, 19, 9, 5, 7.54886059458337});
    iTree1.push_back({0, 0, -2, 4, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({21, 24, 12, 4, 813.4082777720914});
    iTree1.push_back({22, 23, 7, 3, 0.545124712087754});
    iTree1.push_back({0, 0, -2, 2, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({26, 31, 3, 69, 20.76299544998902});
    iTree1.push_back({27, 28, 3, 37, 11.141644327911536});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({29, 30, 0, 36, 12.607634286306062});
    iTree1.push_back({0, 0, -2, 18, -2.0});
    iTree1.push_back({0, 0, -2, 18, -2.0});
    iTree1.push_back({32, 35, 8, 32, 1.139899498849706});
    iTree1.push_back({33, 34, 6, 9, 1.9340674896169396});
    iTree1.push_back({0, 0, -2, 8, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({36, 37, 8, 23, 1.3272121962401586});
    iTree1.push_back({0, 0, -2, 4, -2.0});
    iTree1.push_back({0, 0, -2, 19, -2.0});
    iTree1.push_back({39, 52, 4, 19, 90.49656084715228});
    iTree1.push_back({40, 41, 3, 9, 17.149880090332534});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({42, 47, 3, 8, 22.80349611342116});
    iTree1.push_back({43, 44, 5, 5, 2.0622235007786704});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({45, 46, 8, 4, 2.488883140739828});
    iTree1.push_back({0, 0, -2, 3, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({48, 49, 11, 3, 2.8933253066184066});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({50, 51, 6, 2, 2.032116812990321});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({53, 60, 9, 10, 3.2924277764262335});
    iTree1.push_back({54, 55, 9, 4, 2.6218914105314144});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({56, 57, 9, 3, 2.934714524043481});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({58, 59, 4, 2, 102.52299241606285});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({61, 66, 3, 6, 26.00552356709052});
    iTree1.push_back({62, 65, 3, 5, 18.360772474836917});
    iTree1.push_back({63, 64, 9, 4, 3.634684251082052});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 3, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({68, 71, 0, 3, 12.699479131235478});
    iTree1.push_back({69, 70, 9, 2, 5.574424657074001});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({73, 98, 11, 16, 1.8263059122996346});
    iTree1.push_back({74, 93, 11, 15, 1.7496950858969684});
    iTree1.push_back({75, 76, 7, 12, 0.2386231206207518});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({77, 86, 1, 11, 2.8744867291456266});
    iTree1.push_back({78, 81, 2, 5, 2.3629248915894303});
    iTree1.push_back({79, 80, 9, 2, 9.750646242566814});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({82, 83, 9, 3, 9.342436959055457});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({84, 85, 7, 2, 0.33271734046734514});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({87, 92, 4, 6, 115.41209824449015});
    iTree1.push_back({88, 91, 2, 5, 2.728288918511335});
    iTree1.push_back({89, 90, 12, 4, 646.2875324866211});
    iTree1.push_back({0, 0, -2, 2, -2.0});
    iTree1.push_back({0, 0, -2, 2, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({94, 97, 7, 3, 0.45191416518063365});
    iTree1.push_back({95, 96, 4, 2, 104.88875841204485});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree1);

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
            while ((current_node_id != 0 && current_node_id != 0) || length == 0)
            {
                if (ndimData[i][tree[current_node_id].feature] < tree[current_node_id].threshold)
                {
                    current_node_id = tree[current_node_id].child_id_left;
                }
                else
                {
                    current_node_id = tree[current_node_id].child_id_right;
                }
                
                length += 1;
            }
            
            int leaf_size = tree[current_node_id].n_samples;
            float path_length = length + c(leaf_size);

            path.push_back(path_length);
        }

        //mean of elements in path before push into edges
        float average_path = 0;
        for(int k = 0; k < path.size(); k++)
        {
            average_path += path[k];
            average_path = average_path/path.size();
            edges.push_back(average_path);
        }
    }
    
    std::vector<float> scores;
    float score;
    std::vector<float> average_length = edges;
    for (int i = 0; i < average_length.size(); i ++){
        score = pow(2, -average_length[i]/c(256));
        float scorep = -1 * score + 0.4775;
        scores.push_back(scorep);
    }

    for (size_t i = 0; i < scores.size(); i++)
    {
        //std::cout << average_length[i] << std::endl;
        //std::cout << c(256) << std::endl;
    }
    
}