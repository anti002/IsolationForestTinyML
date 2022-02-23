#include <vector>
using namespace std;

struct Tree{
    int child_id_left, child_id_right, feature, n_samples;
    float threshold;
};

vector<vector<Tree>> iForest;
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

void setup() {
    Serial.begin(9600);
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

}
void loop() {

}