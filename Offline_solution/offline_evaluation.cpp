#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct Tree{
    int child_id_left, child_id_right;
    int feature;
    int n_samples;
    float threshold;
};

std::vector<vector<Tree>> iForest;
std::vector<Tree> iTree1;
std::vector<Tree> iTree2;
std::vector<Tree> iTree3;
std::vector<Tree> iTree4;
std::vector<Tree> iTree5;

std::vector<std::vector<float>> parseCSV()
{   
    std::vector<std::vector<float>> parsedCsv;
    std::ifstream data("C:\\Users\\anton\\OneDrive\\Skrivbord\\Thesis_Code\\IsolationForestTinyML\\DatSets\\test.csv");
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

float c(float size)
{
    if (size > 2)
    {
        float temp = (2 * (log(size -1) + 0.5772156649)) - (2*(size-1)/size);
        return temp;
    }
    if (size == 2)
    {
        return 1;
    }
    return 0;
}

std::vector<float> path_length(std::vector<vector<Tree>> forest, std::vector<std::vector<float>> parsedCsv)
{
    std::vector<float> edges;
    for (size_t i = 0; i < parsedCsv.size(); i++)
    {
        float avg = 0;
        float avg_depth = 0;
        for (size_t j = 0; j < forest.size(); j++)
        {
            std::vector<Tree> tree = forest[j];
            int current_node_id = 0;
            int length = 0;
            
            while (length == 0 || tree[current_node_id].child_id_left != 0)
            {
                float splitValue_attribute = parsedCsv[i][tree[current_node_id].feature];
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
            avg += path_length;
        }

        float average_path = avg/forest.size();
        edges.push_back(average_path);
    }
    return edges;
}


std::vector<float> decision_function(std::vector<vector<Tree>> forest, std::vector<std::vector<float>> parsedCsv)
{
    std::vector<float> scores;
    float score = 0;
    std::vector<float> average_length = path_length(forest, parsedCsv);
    for (size_t i = 0; i < average_length.size(); i ++)
    {
        std::cout << average_length[i] << std::endl;
        float scorep =  0.5 - pow(2, (-1 * average_length[i])/c(parsedCsv.size()));
        scores.push_back(scorep);
    }
    return scores;
}


int main(){
    std::vector<std::vector<float>> parsedCsv = parseCSV();

    iTree1.push_back({1, 2, 9, 129, 1.6405781368961765});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({3, 54, 12, 128, 471.40192911652935});
    iTree1.push_back({4, 17, 10, 35, 0.7816606168045506});
    iTree1.push_back({5, 16, 4, 7, 99.34769586070799});
    iTree1.push_back({6, 9, 3, 6, 19.717726556685626});
    iTree1.push_back({7, 8, 1, 2, 3.2803963471365005});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({10, 13, 4, 4, 82.15621677234593});
    iTree1.push_back({11, 12, 3, 2, 20.014752697206454});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({14, 15, 0, 2, 12.77063790292305});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({18, 35, 2, 28, 2.267621363630143});
    iTree1.push_back({19, 20, 7, 15, 0.21215908051075968});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({21, 28, 2, 14, 1.958125998491533});
    iTree1.push_back({22, 25, 10, 4, 0.9830296932384182});
    iTree1.push_back({23, 24, 0, 2, 12.104659039134585});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({26, 27, 11, 2, 2.1433962733029728});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({29, 32, 2, 10, 2.2139185295292814});
    iTree1.push_back({30, 31, 1, 8, 2.601289930906148});
    iTree1.push_back({0, 0, -2, 7, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({33, 34, 1, 2, 0.9966685021395899});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({36, 43, 9, 13, 2.752268103757097});
    iTree1.push_back({37, 38, 4, 4, 89.11115835565661});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({39, 42, 7, 3, 0.5319080856956571});
    iTree1.push_back({40, 41, 6, 2, 2.322996860168792});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({44, 49, 8, 9, 1.494220103918421});
    iTree1.push_back({45, 46, 7, 5, 0.25772851356477555});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({47, 48, 8, 4, 1.076231359019901});
    iTree1.push_back({0, 0, -2, 2, -2.0});
    iTree1.push_back({0, 0, -2, 2, -2.0});
    iTree1.push_back({50, 51, 7, 4, 0.21574310207180036});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({52, 53, 9, 3, 5.426890474477267});
    iTree1.push_back({0, 0, -2, 2, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({55, 76, 3, 93, 17.109535100605655});
    iTree1.push_back({56, 67, 1, 11, 1.2348377357738336});
    iTree1.push_back({57, 62, 3, 6, 15.801317855079025});
    iTree1.push_back({58, 61, 10, 3, 1.1158768514688775});
    iTree1.push_back({59, 60, 10, 2, 1.0581741431203744});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({63, 64, 6, 3, 1.270563615906003});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({65, 66, 6, 2, 1.296742663144915});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({68, 73, 2, 5, 2.532936249016224});
    iTree1.push_back({69, 70, 1, 3, 1.5254966363187645});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({71, 72, 7, 2, 0.3012374797076133});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({74, 75, 9, 2, 6.232270004161656});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({77, 94, 7, 82, 0.24243538948355028});
    iTree1.push_back({78, 87, 9, 11, 4.5393843935252045});
    iTree1.push_back({79, 84, 1, 7, 1.7961084694993996});
    iTree1.push_back({80, 81, 1, 5, 1.0360002622779787});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({82, 83, 6, 4, 1.9685709084004959});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 3, -2.0});
    iTree1.push_back({85, 86, 0, 2, 11.696381776223015});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({88, 91, 5, 4, 1.466135939223571});
    iTree1.push_back({89, 90, 5, 2, 1.3736115563953035});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({92, 93, 4, 2, 109.94235999693272});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({95, 110, 1, 71, 5.364607428953512});
    iTree1.push_back({96, 103, 5, 69, 1.6226871598164183});
    iTree1.push_back({97, 100, 10, 23, 0.6453655110225959});
    iTree1.push_back({98, 99, 11, 4, 1.4279973646418884});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 3, -2.0});
    iTree1.push_back({101, 102, 0, 19, 12.500847890144788});
    iTree1.push_back({0, 0, -2, 5, -2.0});
    iTree1.push_back({0, 0, -2, 14, -2.0});
    iTree1.push_back({104, 107, 0, 46, 11.721508699332956});
    iTree1.push_back({105, 106, 10, 4, 0.8320143882579716});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 3, -2.0});
    iTree1.push_back({108, 109, 6, 42, 3.0356839066250414});
    iTree1.push_back({0, 0, -2, 41, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({111, 112, 4, 2, 95.32100799601572});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree1);

    iTree2.push_back({1, 34, 11, 129, 1.5601811689772487});
    iTree2.push_back({2, 33, 0, 17, 13.649031793365836});
    iTree2.push_back({3, 16, 11, 16, 1.3869262958258641});
    iTree2.push_back({4, 5, 5, 7, 1.2565960754175118});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({6, 11, 1, 6, 3.161190568829251});
    iTree2.push_back({7, 8, 9, 3, 4.198623093562608});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({9, 10, 2, 2, 2.631421653769825});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({12, 15, 12, 3, 646.4988212643651});
    iTree2.push_back({13, 14, 8, 2, 1.0188550760229846});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({17, 26, 3, 9, 21.474219297745368});
    iTree2.push_back({18, 25, 4, 5, 113.04058962410343});
    iTree2.push_back({19, 20, 3, 4, 17.810461656335026});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({21, 24, 1, 3, 3.086613766998433});
    iTree2.push_back({22, 23, 7, 2, 0.28956446417031745});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({27, 30, 10, 4, 0.6538067018975902});
    iTree2.push_back({28, 29, 11, 2, 1.472665395971382});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({31, 32, 4, 2, 108.39742641495421});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({35, 110, 11, 112, 3.5846136449837442});
    iTree2.push_back({36, 73, 10, 110, 0.8912500530951233});
    iTree2.push_back({37, 54, 8, 43, 1.4327442696514958});
    iTree2.push_back({38, 45, 1, 29, 2.485680265928076});
    iTree2.push_back({39, 42, 11, 4, 1.6425703273163648});
    iTree2.push_back({40, 41, 2, 2, 2.239582756727057});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({43, 44, 6, 2, 2.4828419297098643});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({46, 53, 11, 25, 3.134410773592649});
    iTree2.push_back({47, 50, 11, 24, 2.4334341031835933});
    iTree2.push_back({48, 49, 4, 22, 81.80301934564626});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 21, -2.0});
    iTree2.push_back({51, 52, 8, 2, 0.7921152015604129});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({55, 64, 5, 14, 2.41837078111526});
    iTree2.push_back({56, 61, 11, 8, 2.9641195269026204});
    iTree2.push_back({57, 60, 5, 6, 1.862577485899684});
    iTree2.push_back({58, 59, 2, 5, 2.5538858813756504});
    iTree2.push_back({0, 0, -2, 4, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({62, 63, 10, 2, 0.8409455946423253});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({65, 70, 2, 6, 2.54570337673703});
    iTree2.push_back({66, 67, 3, 4, 19.97349136624182});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({68, 69, 0, 3, 11.969901907215693});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 2, -2.0});
    iTree2.push_back({71, 72, 9, 2, 11.82086660075973});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({74, 97, 6, 67, 2.4366473563642237});
    iTree2.push_back({75, 88, 9, 47, 4.133607332420044});
    iTree2.push_back({76, 81, 9, 41, 1.944076122618375});
    iTree2.push_back({77, 80, 5, 3, 2.4562243993411923});
    iTree2.push_back({78, 79, 8, 2, 1.7573627004211692});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({82, 85, 7, 38, 0.599498159005073});
    iTree2.push_back({83, 84, 10, 36, 1.0277359266303938});
    iTree2.push_back({0, 0, -2, 15, -2.0});
    iTree2.push_back({0, 0, -2, 21, -2.0});
    iTree2.push_back({86, 87, 7, 2, 0.6253882123391743});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({89, 92, 8, 6, 0.8285624831197287});
    iTree2.push_back({90, 91, 9, 2, 5.84604598729272});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({93, 96, 1, 4, 1.7562295739702845});
    iTree2.push_back({94, 95, 11, 3, 2.5244024126065905});
    iTree2.push_back({0, 0, -2, 2, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({98, 103, 5, 20, 2.4767327938516988});
    iTree2.push_back({99, 102, 7, 3, 0.40162667724469003});
    iTree2.push_back({100, 101, 10, 2, 1.1696489782056383});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({104, 109, 1, 17, 3.674099374858212});
    iTree2.push_back({105, 108, 1, 16, 2.947280459651665});
    iTree2.push_back({106, 107, 7, 15, 0.27651675847003737});
    iTree2.push_back({0, 0, -2, 7, -2.0});
    iTree2.push_back({0, 0, -2, 8, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({111, 112, 11, 2, 3.6779090851080807});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree2);

    iTree3.push_back({1, 6, 8, 129, 0.5024201283602208});
    iTree3.push_back({2, 3, 9, 3, 2.3025119055563517});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({4, 5, 9, 2, 3.2207828029280807});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({7, 58, 9, 126, 7.464036132601433});
    iTree3.push_back({8, 33, 10, 101, 1.0375714310920718});
    iTree3.push_back({9, 28, 6, 58, 3.070733626148361});
    iTree3.push_back({10, 23, 3, 55, 25.407638663848694});
    iTree3.push_back({11, 18, 5, 52, 2.946360520349094});
    iTree3.push_back({12, 15, 5, 49, 1.289350318530221});
    iTree3.push_back({13, 14, 7, 5, 0.5179236077375129});
    iTree3.push_back({0, 0, -2, 4, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({16, 17, 3, 44, 23.162180837784977});
    iTree3.push_back({0, 0, -2, 39, -2.0});
    iTree3.push_back({0, 0, -2, 5, -2.0});
    iTree3.push_back({19, 22, 8, 3, 2.8089991214017775});
    iTree3.push_back({20, 21, 9, 2, 3.5678163200017936});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({24, 25, 8, 3, 1.6898263053041456});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({26, 27, 12, 2, 393.5263074052177});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({29, 30, 3, 3, 17.781385090100294});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({31, 32, 11, 2, 2.9192921533651814});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({34, 47, 11, 43, 2.3416373517898683});
    iTree3.push_back({35, 40, 11, 7, 2.265533502573562});
    iTree3.push_back({36, 39, 2, 3, 2.319511228450183});
    iTree3.push_back({37, 38, 0, 2, 11.911169952597083});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({41, 46, 8, 4, 2.693015482317628});
    iTree3.push_back({42, 45, 1, 3, 1.321738392169451});
    iTree3.push_back({43, 44, 12, 2, 492.4636982579081});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({48, 57, 7, 36, 0.5673685477516354});
    iTree3.push_back({49, 56, 3, 35, 28.361705748067102});
    iTree3.push_back({50, 53, 12, 34, 501.84740606502044});
    iTree3.push_back({51, 52, 2, 13, 2.236966251756413});
    iTree3.push_back({0, 0, -2, 8, -2.0});
    iTree3.push_back({0, 0, -2, 5, -2.0});
    iTree3.push_back({54, 55, 9, 21, 5.463936217702525});
    iTree3.push_back({0, 0, -2, 16, -2.0});
    iTree3.push_back({0, 0, -2, 5, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({59, 80, 5, 25, 1.6664484470359693});
    iTree3.push_back({60, 75, 6, 11, 1.2724885833820054});
    iTree3.push_back({61, 68, 9, 8, 9.30268987530998});
    iTree3.push_back({62, 63, 12, 4, 664.2234324264449});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({64, 67, 9, 3, 8.80761602667403});
    iTree3.push_back({65, 66, 5, 2, 1.4487140030495955});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({69, 70, 6, 4, 0.658541522428433});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({71, 74, 3, 3, 20.434631070557344});
    iTree3.push_back({72, 73, 5, 2, 1.5803107482494592});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({76, 77, 6, 3, 1.3626857194259605});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({78, 79, 1, 2, 3.1692028754161954});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({81, 94, 5, 14, 1.8490391064968612});
    iTree3.push_back({82, 83, 0, 7, 13.162199714256626});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({84, 89, 2, 6, 2.415975141385145});
    iTree3.push_back({85, 86, 12, 3, 572.5301235087821});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({87, 88, 8, 2, 1.6526803157134737});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({90, 93, 4, 3, 97.86663071013365});
    iTree3.push_back({91, 92, 8, 2, 1.0968711029564453});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({95, 102, 7, 7, 0.550600345125728});
    iTree3.push_back({96, 97, 5, 5, 1.9592139773457622});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({98, 101, 10, 4, 0.6540770597044518});
    iTree3.push_back({99, 100, 3, 3, 24.937058057606713});
    iTree3.push_back({0, 0, -2, 2, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({103, 104, 6, 2, 0.6834457306055426});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree3);

    iTree4.push_back({1, 20, 9, 129, 2.4703708269776206});
    iTree4.push_back({2, 3, 11, 16, 2.129241880633525});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({4, 17, 4, 15, 94.37988477404224});
    iTree4.push_back({5, 6, 4, 13, 70.72941087220302});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({7, 16, 2, 12, 2.4129735352657913});
    iTree4.push_back({8, 15, 6, 11, 2.5866225582224893});
    iTree4.push_back({9, 12, 9, 10, 2.2341864076922184});
    iTree4.push_back({10, 11, 10, 7, 1.164515923691741});
    iTree4.push_back({0, 0, -2, 6, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({13, 14, 8, 3, 1.4841500170478061});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({0, 0, -2, 2, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({18, 19, 10, 2, 0.9833603074452167});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({21, 88, 9, 113, 10.750839577173274});
    iTree4.push_back({22, 57, 12, 110, 813.4985346531023});
    iTree4.push_back({23, 36, 10, 91, 0.712152824949803});
    iTree4.push_back({24, 33, 11, 24, 2.3563039505791});
    iTree4.push_back({25, 26, 0, 22, 12.23552538212405});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({27, 30, 2, 21, 2.625659203577964});
    iTree4.push_back({28, 29, 7, 18, 0.3586648434360501});
    iTree4.push_back({0, 0, -2, 3, -2.0});
    iTree4.push_back({0, 0, -2, 15, -2.0});
    iTree4.push_back({31, 32, 11, 3, 1.6160351638201211});
    iTree4.push_back({0, 0, -2, 2, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({34, 35, 3, 2, 20.542717385824172});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({37, 48, 12, 67, 587.6530043397345});
    iTree4.push_back({38, 45, 1, 41, 5.31275792980718});
    iTree4.push_back({39, 42, 5, 39, 1.474251984455766});
    iTree4.push_back({40, 41, 5, 5, 1.4225153167469955});
    iTree4.push_back({0, 0, -2, 4, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({43, 44, 10, 34, 0.8166291310049214});
    iTree4.push_back({0, 0, -2, 6, -2.0});
    iTree4.push_back({0, 0, -2, 28, -2.0});
    iTree4.push_back({46, 47, 9, 2, 2.613876838794929});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({49, 56, 9, 26, 10.415487595611918});
    iTree4.push_back({50, 53, 5, 25, 2.6980366389814});
    iTree4.push_back({51, 52, 8, 22, 2.0578024918318434});
    iTree4.push_back({0, 0, -2, 19, -2.0});
    iTree4.push_back({0, 0, -2, 3, -2.0});
    iTree4.push_back({54, 55, 6, 3, 2.9824565787498933});
    iTree4.push_back({0, 0, -2, 2, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({58, 75, 12, 19, 1007.5513418659341});
    iTree4.push_back({59, 68, 11, 11, 2.053945341753097});
    iTree4.push_back({60, 67, 10, 6, 0.7980571312334519});
    iTree4.push_back({61, 64, 2, 5, 2.3646523529972137});
    iTree4.push_back({62, 63, 8, 3, 1.2621513659101047});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({0, 0, -2, 2, -2.0});
    iTree4.push_back({65, 66, 2, 2, 2.3971451237446106});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({69, 70, 1, 5, 1.3354304929254037});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({71, 74, 2, 4, 2.3674308741374452});
    iTree4.push_back({72, 73, 12, 3, 954.8034025415794});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({0, 0, -2, 2, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({76, 79, 7, 8, 0.2910257422485479});
    iTree4.push_back({77, 78, 8, 2, 1.6616259734495198});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({80, 83, 6, 6, 2.852435476023385});
    iTree4.push_back({81, 82, 6, 2, 2.676992947107164});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({84, 87, 7, 4, 0.33661804241501914});
    iTree4.push_back({85, 86, 6, 3, 3.0437707247737205});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({0, 0, -2, 2, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({89, 92, 1, 3, 1.7348821818245248});
    iTree4.push_back({90, 91, 6, 2, 1.1614980281715102});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree4);

    iTree5.push_back({1, 84, 1, 129, 2.730697426506171});
    iTree5.push_back({2, 7, 8, 83, 0.4613010051615283});
    iTree5.push_back({3, 4, 7, 3, 0.524922819453173});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({5, 6, 0, 2, 13.212935861883066});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({8, 61, 7, 80, 0.4170474445707501});
    iTree5.push_back({9, 36, 5, 55, 2.4430078397680624});
    iTree5.push_back({10, 23, 3, 30, 18.760191059825175});
    iTree5.push_back({11, 16, 6, 11, 1.4915603697199324});
    iTree5.push_back({12, 15, 6, 3, 1.3354625100189856});
    iTree5.push_back({13, 14, 11, 2, 2.968119839976868});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({17, 20, 8, 8, 1.6938820888665584});
    iTree5.push_back({18, 19, 9, 5, 3.4562737141438578});
    iTree5.push_back({0, 0, -2, 2, -2.0});
    iTree5.push_back({0, 0, -2, 3, -2.0});
    iTree5.push_back({21, 22, 6, 3, 2.5565928222483016});
    iTree5.push_back({0, 0, -2, 2, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({24, 29, 10, 19, 0.6998551380106639});
    iTree5.push_back({25, 28, 0, 3, 12.99938493975836});
    iTree5.push_back({26, 27, 10, 2, 0.5912795878728889});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({30, 33, 10, 16, 1.0999734140721462});
    iTree5.push_back({31, 32, 0, 11, 12.023175219749922});
    iTree5.push_back({0, 0, -2, 2, -2.0});
    iTree5.push_back({0, 0, -2, 9, -2.0});
    iTree5.push_back({34, 35, 0, 5, 12.466398664035749});
    iTree5.push_back({0, 0, -2, 4, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({37, 46, 8, 25, 1.748708954756597});
    iTree5.push_back({38, 39, 2, 10, 1.9827486778751453});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({40, 43, 8, 9, 1.507677018456477});
    iTree5.push_back({41, 42, 11, 6, 2.6835982291189384});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 5, -2.0});
    iTree5.push_back({44, 45, 2, 3, 2.4316082752903614});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 2, -2.0});
    iTree5.push_back({47, 54, 11, 15, 2.998428901816782});
    iTree5.push_back({48, 51, 0, 8, 13.602977106484978});
    iTree5.push_back({49, 50, 2, 5, 1.9861840011761211});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 4, -2.0});
    iTree5.push_back({52, 53, 1, 3, 1.8853743652206525});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 2, -2.0});
    iTree5.push_back({55, 58, 10, 7, 1.006725339785584});
    iTree5.push_back({56, 57, 9, 3, 6.28486322185497});
    iTree5.push_back({0, 0, -2, 2, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({59, 60, 12, 4, 916.5987728706555});
    iTree5.push_back({0, 0, -2, 2, -2.0});
    iTree5.push_back({0, 0, -2, 2, -2.0});
    iTree5.push_back({62, 81, 12, 25, 817.419346851492});
    iTree5.push_back({63, 78, 12, 23, 662.9056381005355});
    iTree5.push_back({64, 71, 7, 21, 0.5273052736552246});
    iTree5.push_back({65, 68, 10, 13, 0.7211700460272907});
    iTree5.push_back({66, 67, 10, 3, 0.5962362193079577});
    iTree5.push_back({0, 0, -2, 2, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({69, 70, 7, 10, 0.45740361066988644});
    iTree5.push_back({0, 0, -2, 5, -2.0});
    iTree5.push_back({0, 0, -2, 5, -2.0});
    iTree5.push_back({72, 75, 2, 8, 2.531059400367298});
    iTree5.push_back({73, 74, 5, 5, 1.5434255293007006});
    iTree5.push_back({0, 0, -2, 2, -2.0});
    iTree5.push_back({0, 0, -2, 3, -2.0});
    iTree5.push_back({76, 77, 1, 3, 2.605597543473613});
    iTree5.push_back({0, 0, -2, 2, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({79, 80, 11, 2, 2.092934892233205});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({82, 83, 9, 2, 6.73601803295076});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({85, 120, 0, 46, 13.47466056779533});
    iTree5.push_back({86, 113, 5, 35, 2.811567400444516});
    iTree5.push_back({87, 108, 0, 31, 13.329792906109756});
    iTree5.push_back({88, 97, 7, 28, 0.5007433321918926});
    iTree5.push_back({89, 96, 0, 19, 13.279839971863938});
    iTree5.push_back({90, 93, 2, 18, 2.0207559364106302});
    iTree5.push_back({91, 92, 8, 2, 1.2925009239068488});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({94, 95, 2, 16, 2.2798276207450296});
    iTree5.push_back({0, 0, -2, 6, -2.0});
    iTree5.push_back({0, 0, -2, 10, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({98, 103, 10, 9, 0.7712030544519752});
    iTree5.push_back({99, 102, 11, 5, 2.2795774414231698});
    iTree5.push_back({100, 101, 1, 4, 4.516214481401186});
    iTree5.push_back({0, 0, -2, 2, -2.0});
    iTree5.push_back({0, 0, -2, 2, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({104, 105, 7, 4, 0.5634827187176074});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({106, 107, 9, 3, 4.8550540451615});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 2, -2.0});
    iTree5.push_back({109, 112, 5, 3, 1.8560466552874813});
    iTree5.push_back({110, 111, 6, 2, 0.8617731700587017});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({114, 115, 1, 4, 3.2033284547137786});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({116, 119, 6, 3, 3.003599994387851});
    iTree5.push_back({117, 118, 10, 2, 1.0832973259566996});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({121, 140, 12, 11, 703.0549269556324});
    iTree5.push_back({122, 123, 5, 10, 0.9997161643617418});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({124, 129, 1, 9, 3.200188926037984});
    iTree5.push_back({125, 128, 12, 3, 584.3129328402285});
    iTree5.push_back({126, 127, 11, 2, 1.5648907395856366});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({130, 133, 5, 6, 1.710988037057529});
    iTree5.push_back({131, 132, 3, 2, 20.454647837651265});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({134, 137, 10, 4, 0.8141079814899288});
    iTree5.push_back({135, 136, 2, 2, 2.4187200381647074});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({138, 139, 9, 2, 4.664125943115907});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree5);


    std::vector<float> scores_pred = decision_function(iForest, parsedCsv);
    float avg = 0;
    for (size_t i = 0; i < scores_pred.size(); i++)
    {
        std::cout << scores_pred[i] << std::endl;
        avg += scores_pred[i];
    }
    //std::cout << avg/scores_pred.size() << std::endl;
}
