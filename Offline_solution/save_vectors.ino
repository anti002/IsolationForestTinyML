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
    iTree1.push_back({1, 92, 4, 129, 140.76942206675625});
    iTree1.push_back({2, 43, 4, 127, 90.76112199563585});
    iTree1.push_back({3, 18, 6, 54, 0.8489511963225397});
    iTree1.push_back({4, 5, 3, 12, 13.537867511899258});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({6, 9, 1, 11, 2.476501037700126});
    iTree1.push_back({7, 8, 7, 2, 0.5455851492290039});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({10, 13, 0, 9, 12.837502419160215});
    iTree1.push_back({11, 12, 9, 2, 6.573685979083372});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({14, 17, 6, 7, 0.7135073439171333});
    iTree1.push_back({15, 16, 0, 6, 13.621330615025457});
    iTree1.push_back({0, 0, -2, 3, -2.0});
    iTree1.push_back({0, 0, -2, 3, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({19, 42, 10, 42, 1.6199856722022368});
    iTree1.push_back({20, 29, 8, 41, 1.306085388580287});
    iTree1.push_back({21, 22, 3, 6, 16.642311424316052});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({23, 26, 5, 5, 2.125494032511546});
    iTree1.push_back({24, 25, 5, 2, 1.9350272501571635});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({27, 28, 6, 3, 1.9860975224284452});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 2, -2.0});
    iTree1.push_back({30, 37, 3, 35, 18.847464016102006});
    iTree1.push_back({31, 34, 1, 9, 1.3631390551044111});
    iTree1.push_back({32, 33, 8, 4, 1.8730474828800352});
    iTree1.push_back({0, 0, -2, 2, -2.0});
    iTree1.push_back({0, 0, -2, 2, -2.0});
    iTree1.push_back({35, 36, 1, 5, 1.4687860365609782});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 4, -2.0});
    iTree1.push_back({38, 39, 11, 26, 1.890215280977992});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({40, 41, 0, 25, 12.575581406219564});
    iTree1.push_back({0, 0, -2, 20, -2.0});
    iTree1.push_back({0, 0, -2, 5, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({44, 53, 2, 73, 1.9412826931393372});
    iTree1.push_back({45, 48, 10, 5, 1.1360814102807755});
    iTree1.push_back({46, 47, 4, 2, 97.20512599740417});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({49, 52, 5, 3, 2.1613230639030436});
    iTree1.push_back({50, 51, 5, 2, 1.7041187211664692});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({54, 81, 6, 68, 2.985762086712655});
    iTree1.push_back({55, 66, 4, 62, 96.0752850422055});
    iTree1.push_back({56, 63, 12, 14, 713.1590439626757});
    iTree1.push_back({57, 60, 10, 12, 0.6174441616249634});
    iTree1.push_back({58, 59, 8, 2, 1.2527153101716906});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({61, 62, 6, 10, 0.7309903702598203});
    iTree1.push_back({0, 0, -2, 5, -2.0});
    iTree1.push_back({0, 0, -2, 5, -2.0});
    iTree1.push_back({64, 65, 11, 2, 1.6234693625022187});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({67, 74, 8, 48, 1.2372903239313504});
    iTree1.push_back({68, 71, 11, 19, 2.0126747956771096});
    iTree1.push_back({69, 70, 2, 13, 2.2014979868734468});
    iTree1.push_back({0, 0, -2, 2, -2.0});
    iTree1.push_back({0, 0, -2, 11, -2.0});
    iTree1.push_back({72, 73, 3, 6, 21.562002563645134});
    iTree1.push_back({0, 0, -2, 3, -2.0});
    iTree1.push_back({0, 0, -2, 3, -2.0});
    iTree1.push_back({75, 78, 3, 29, 18.9790093885575});
    iTree1.push_back({76, 77, 2, 10, 2.0492380706255524});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 9, -2.0});
    iTree1.push_back({79, 80, 6, 19, 2.8185074995494475});
    iTree1.push_back({0, 0, -2, 18, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({82, 91, 2, 6, 2.7447860646386926});
    iTree1.push_back({83, 88, 9, 5, 6.422476482026762});
    iTree1.push_back({84, 85, 3, 3, 16.65358340452632});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({86, 87, 0, 2, 13.311816107057368});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({89, 90, 5, 2, 2.995063489647532});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({93, 94, 12, 2, 758.5235508352162});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree1);

}
void loop() {

}