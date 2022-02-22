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

std::vector<float> dataSet;
std::vector<vector<Tree>> iForest;
std::vector<Tree> iTree1;

int main(){
    iTree1.push_back({1, 2, 4, 129, 72.02171203215686});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({3, 36, 12, 128, 1088.404219455274});
    iTree1.push_back({4, 35, 6, 122, 3.912145246030638});
    iTree1.push_back({5, 26, 12, 121, 950.0623462926886});
    iTree1.push_back({6, 15, 5, 116, 2.4736709211765993});
    iTree1.push_back({7, 14, 4, 91, 149.0737372041092});
    iTree1.push_back({8, 11, 12, 90, 431.6030703334152});
    iTree1.push_back({9, 10, 9, 14, 3.2211440678204317});
    iTree1.push_back({0, 0, -2, 9, -2.0});
    iTree1.push_back({0, 0, -2, 5, -2.0});
    iTree1.push_back({12, 13, 4, 76, 111.08964769779223});
    iTree1.push_back({0, 0, -2, 65, -2.0});
    iTree1.push_back({0, 0, -2, 11, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({16, 21, 7, 25, 0.2552607374518071});
    iTree1.push_back({17, 18, 2, 7, 1.954436201640515});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({19, 20, 11, 6, 3.456564754783706});
    iTree1.push_back({0, 0, -2, 5, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({22, 23, 3, 18, 15.21732546399223});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({24, 25, 11, 17, 2.8823696604929943});
    iTree1.push_back({0, 0, -2, 10, -2.0});
    iTree1.push_back({0, 0, -2, 7, -2.0});
    iTree1.push_back({27, 32, 0, 5, 14.073755481150922});
    iTree1.push_back({28, 31, 2, 3, 2.4516489567168356});
    iTree1.push_back({29, 30, 2, 2, 2.303378063680138});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({33, 34, 3, 2, 17.836402147326805});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({37, 38, 6, 6, 2.430061976614103});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({39, 40, 2, 5, 2.4726853648578926});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({41, 46, 7, 4, 0.3495693537185417});
    iTree1.push_back({42, 45, 11, 3, 3.4595430489988543});
    iTree1.push_back({43, 44, 11, 2, 2.7296275542315263});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree1);

    std::vector<float> scores = decision_function(iTree1, data);
}

int c(int size){

    if (size > 2){
        return 2 * (log(size -1) + 0.5772156649) - 2*(size-1)/size;
    }
    if (size == 2){
        return 1;
    }
    return 0;
}

//TODO Write code for traversing nodes AND calculate mean path
std::vector<float> path_length(std::vector<vector<Tree>> forest, std::vector<vector<float>> data)
{

    std::vector<float> edges;
    
    for (int i = 0; i < dataSet.size(); i++)
    {

        std::vector<float> path;

        for (int j = 0; j < forest.size(); j++)
        {
            std::vector<Tree> tree = forest[j];
            int current_node_id = 0;
            int length = 0;
            
            //With the OR j == 0 we ensure that the first iteration will go through since first
            //always has the id of 0
            while ((current_node_id != 0 && current_node_id != 0) || j == 0)
            {
                if (data[i][tree[current_node_id].feature] < tree[current_node_id].threshold)
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
        float average_path;
        for(int k = 0; k < path.size(); k++)
        {
            average_path += path[k];
            average_path = average_path/path.size();
            edges.push_back(average_path);
        }
    }
    return edges;
}


//TODO: Remove the extra step of storing the score in a float varaible
std::vector<float> decision_function(std::vector<Tree> tree, std::vector<vector<float>> data){

    std::vector<float> scores;
    float score;
    std::vector<float> average_length = path_length(tree, data);
    for (int i = 0; i < average_length.size(); i ++){
        score = pow(2, -average_length[i]/c(256));
        scores.push_back(-score + 0.4775);
    }
    return scores;
}
