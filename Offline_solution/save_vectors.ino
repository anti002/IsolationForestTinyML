#include <vector>
using namespace std;

struct Tree{
    int child_id_left, child_id_right, feature, n_samples;
    float threshold;
};

vector<vector<Tree>> iForest;
std::vector<Tree> iTree1;

void setup() {
    Serial.begin(9600);
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

}
void loop() {

}