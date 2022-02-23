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
std::vector<Tree> iTree11;
std::vector<Tree> iTree12;
std::vector<Tree> iTree13;
std::vector<Tree> iTree14;
std::vector<Tree> iTree15;
std::vector<Tree> iTree16;
std::vector<Tree> iTree17;
std::vector<Tree> iTree18;
std::vector<Tree> iTree19;
std::vector<Tree> iTree20;
std::vector<Tree> iTree21;
std::vector<Tree> iTree22;
std::vector<Tree> iTree23;
std::vector<Tree> iTree24;
std::vector<Tree> iTree25;
std::vector<Tree> iTree26;
std::vector<Tree> iTree27;
std::vector<Tree> iTree28;
std::vector<Tree> iTree29;
std::vector<Tree> iTree30;
std::vector<Tree> iTree31;
std::vector<Tree> iTree32;
std::vector<Tree> iTree33;
std::vector<Tree> iTree34;
std::vector<Tree> iTree35;
std::vector<Tree> iTree36;
std::vector<Tree> iTree37;
std::vector<Tree> iTree38;
std::vector<Tree> iTree39;
std::vector<Tree> iTree40;
std::vector<Tree> iTree41;
std::vector<Tree> iTree42;
std::vector<Tree> iTree43;
std::vector<Tree> iTree44;
std::vector<Tree> iTree45;
std::vector<Tree> iTree46;
std::vector<Tree> iTree47;
std::vector<Tree> iTree48;
std::vector<Tree> iTree49;
std::vector<Tree> iTree50;
std::vector<Tree> iTree51;
std::vector<Tree> iTree52;
std::vector<Tree> iTree53;
std::vector<Tree> iTree54;
std::vector<Tree> iTree55;
std::vector<Tree> iTree56;
std::vector<Tree> iTree57;
std::vector<Tree> iTree58;
std::vector<Tree> iTree59;
std::vector<Tree> iTree60;
std::vector<Tree> iTree61;
std::vector<Tree> iTree62;
std::vector<Tree> iTree63;
std::vector<Tree> iTree64;
std::vector<Tree> iTree65;
std::vector<Tree> iTree66;
std::vector<Tree> iTree67;
std::vector<Tree> iTree68;
std::vector<Tree> iTree69;
std::vector<Tree> iTree70;
std::vector<Tree> iTree71;
std::vector<Tree> iTree72;
std::vector<Tree> iTree73;
std::vector<Tree> iTree74;
std::vector<Tree> iTree75;
std::vector<Tree> iTree76;
std::vector<Tree> iTree77;
std::vector<Tree> iTree78;
std::vector<Tree> iTree79;
std::vector<Tree> iTree80;
std::vector<Tree> iTree81;
std::vector<Tree> iTree82;
std::vector<Tree> iTree83;
std::vector<Tree> iTree84;
std::vector<Tree> iTree85;
std::vector<Tree> iTree86;
std::vector<Tree> iTree87;
std::vector<Tree> iTree88;
std::vector<Tree> iTree89;
std::vector<Tree> iTree90;
std::vector<Tree> iTree91;
std::vector<Tree> iTree92;
std::vector<Tree> iTree93;
std::vector<Tree> iTree94;
std::vector<Tree> iTree95;
std::vector<Tree> iTree96;
std::vector<Tree> iTree97;
std::vector<Tree> iTree98;
std::vector<Tree> iTree99;
std::vector<Tree> iTree100;

int main(){

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

    iTree1.push_back({1, 2, 0, 3, 13.591396885699229});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({3, 4, 7, 2, 0.40825807308596046});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iTree1.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree1);

    iTree2.push_back({1, 2, 5, 3, 2.4795425823888784});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({3, 4, 5, 2, 2.8737883377137745});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iTree2.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree2);

    iTree3.push_back({1, 2, 8, 3, 1.927202066373514});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({3, 4, 5, 2, 2.6282953826663915});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iTree3.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree3);

    iTree4.push_back({1, 2, 8, 3, 1.8032303351345986});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({3, 4, 5, 2, 2.7804172896689985});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iTree4.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree4);

    iTree5.push_back({1, 2, 11, 3, 2.6654207308748408});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({3, 4, 10, 2, 1.1239146324527347});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iTree5.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree5);

    iTree6.push_back({1, 2, 9, 3, 4.5766496543855455});
    iTree6.push_back({0, 0, -2, 1, -2.0});
    iTree6.push_back({3, 4, 1, 2, 1.9680012927893054});
    iTree6.push_back({0, 0, -2, 1, -2.0});
    iTree6.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree6);

    iTree7.push_back({1, 2, 6, 3, 2.7683196983285896});
    iTree7.push_back({0, 0, -2, 1, -2.0});
    iTree7.push_back({3, 4, 3, 2, 19.551891891900727});
    iTree7.push_back({0, 0, -2, 1, -2.0});
    iTree7.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree7);

    iTree8.push_back({1, 2, 1, 3, 1.8450004090291872});
    iTree8.push_back({0, 0, -2, 1, -2.0});
    iTree8.push_back({3, 4, 6, 2, 3.2137664559996386});
    iTree8.push_back({0, 0, -2, 1, -2.0});
    iTree8.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree8);

    iTree9.push_back({1, 2, 8, 3, 1.808178766597533});
    iTree9.push_back({0, 0, -2, 1, -2.0});
    iTree9.push_back({3, 4, 10, 2, 1.1964653052901943});
    iTree9.push_back({0, 0, -2, 1, -2.0});
    iTree9.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree9);

    iTree10.push_back({1, 4, 10, 3, 1.0775514145868552});
    iTree10.push_back({2, 3, 9, 2, 6.12006599978761});
    iTree10.push_back({0, 0, -2, 1, -2.0});
    iTree10.push_back({0, 0, -2, 1, -2.0});
    iTree10.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree10);

    iTree11.push_back({1, 4, 5, 3, 2.934106658663527});
    iTree11.push_back({2, 3, 9, 2, 6.095481269970803});
    iTree11.push_back({0, 0, -2, 1, -2.0});
    iTree11.push_back({0, 0, -2, 1, -2.0});
    iTree11.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree11);

    iTree12.push_back({1, 2, 2, 3, 2.5797357874489864});
    iTree12.push_back({0, 0, -2, 1, -2.0});
    iTree12.push_back({3, 4, 0, 2, 13.920842244952151});
    iTree12.push_back({0, 0, -2, 1, -2.0});
    iTree12.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree12);

    iTree13.push_back({1, 2, 9, 3, 4.4203797641483575});
    iTree13.push_back({0, 0, -2, 1, -2.0});
    iTree13.push_back({3, 4, 4, 2, 103.44478155926093});
    iTree13.push_back({0, 0, -2, 1, -2.0});
    iTree13.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree13);

    iTree14.push_back({1, 2, 12, 3, 1022.5379810680347});
    iTree14.push_back({0, 0, -2, 1, -2.0});
    iTree14.push_back({3, 4, 3, 2, 19.94691067261863});
    iTree14.push_back({0, 0, -2, 1, -2.0});
    iTree14.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree14);

    iTree15.push_back({1, 2, 12, 3, 1226.3896385004696});
    iTree15.push_back({0, 0, -2, 1, -2.0});
    iTree15.push_back({3, 4, 11, 2, 2.8025404812626227});
    iTree15.push_back({0, 0, -2, 1, -2.0});
    iTree15.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree15);

    iTree16.push_back({1, 4, 3, 3, 17.68912455668728});
    iTree16.push_back({2, 3, 8, 2, 1.8793657611446757});
    iTree16.push_back({0, 0, -2, 1, -2.0});
    iTree16.push_back({0, 0, -2, 1, -2.0});
    iTree16.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree16);

    iTree17.push_back({1, 2, 9, 3, 5.830735660288397});
    iTree17.push_back({0, 0, -2, 1, -2.0});
    iTree17.push_back({3, 4, 4, 2, 102.47506854425886});
    iTree17.push_back({0, 0, -2, 1, -2.0});
    iTree17.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree17);

    iTree18.push_back({1, 2, 0, 3, 13.434723317569508});
    iTree18.push_back({0, 0, -2, 1, -2.0});
    iTree18.push_back({3, 4, 3, 2, 19.693241248834404});
    iTree18.push_back({0, 0, -2, 1, -2.0});
    iTree18.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree18);

    iTree19.push_back({1, 4, 2, 3, 2.685401070289902});
    iTree19.push_back({2, 3, 11, 2, 2.8434722029148687});
    iTree19.push_back({0, 0, -2, 1, -2.0});
    iTree19.push_back({0, 0, -2, 1, -2.0});
    iTree19.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree19);

    iTree20.push_back({1, 2, 5, 3, 2.5264411186420723});
    iTree20.push_back({0, 0, -2, 1, -2.0});
    iTree20.push_back({3, 4, 12, 2, 1274.9918913119434});
    iTree20.push_back({0, 0, -2, 1, -2.0});
    iTree20.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree20);

    iTree21.push_back({1, 4, 5, 3, 2.8041457110874415});
    iTree21.push_back({2, 3, 7, 2, 0.40024933285535});
    iTree21.push_back({0, 0, -2, 1, -2.0});
    iTree21.push_back({0, 0, -2, 1, -2.0});
    iTree21.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree21);

    iTree22.push_back({1, 4, 7, 3, 0.36513198151029647});
    iTree22.push_back({2, 3, 5, 2, 2.819693189441519});
    iTree22.push_back({0, 0, -2, 1, -2.0});
    iTree22.push_back({0, 0, -2, 1, -2.0});
    iTree22.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree22);

    iTree23.push_back({1, 4, 7, 3, 0.4048160728060831});
    iTree23.push_back({2, 3, 5, 2, 2.8500298106415713});
    iTree23.push_back({0, 0, -2, 1, -2.0});
    iTree23.push_back({0, 0, -2, 1, -2.0});
    iTree23.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree23);

    iTree24.push_back({1, 2, 6, 3, 3.0394755015861024});
    iTree24.push_back({0, 0, -2, 1, -2.0});
    iTree24.push_back({3, 4, 6, 2, 3.1483061041206137});
    iTree24.push_back({0, 0, -2, 1, -2.0});
    iTree24.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree24);

    iTree25.push_back({1, 4, 10, 3, 1.0830718522876988});
    iTree25.push_back({2, 3, 4, 2, 107.4647306629758});
    iTree25.push_back({0, 0, -2, 1, -2.0});
    iTree25.push_back({0, 0, -2, 1, -2.0});
    iTree25.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree25);

    iTree26.push_back({1, 4, 7, 3, 0.3614858481076896});
    iTree26.push_back({2, 3, 7, 2, 0.31556293183660794});
    iTree26.push_back({0, 0, -2, 1, -2.0});
    iTree26.push_back({0, 0, -2, 1, -2.0});
    iTree26.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree26);

    iTree27.push_back({1, 4, 3, 3, 18.422173802636262});
    iTree27.push_back({2, 3, 2, 2, 2.6206383504141018});
    iTree27.push_back({0, 0, -2, 1, -2.0});
    iTree27.push_back({0, 0, -2, 1, -2.0});
    iTree27.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree27);

    iTree28.push_back({1, 4, 3, 3, 17.97842431502972});
    iTree28.push_back({2, 3, 0, 2, 13.657574922891003});
    iTree28.push_back({0, 0, -2, 1, -2.0});
    iTree28.push_back({0, 0, -2, 1, -2.0});
    iTree28.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree28);

    iTree29.push_back({1, 2, 5, 3, 2.588283531140118});
    iTree29.push_back({0, 0, -2, 1, -2.0});
    iTree29.push_back({3, 4, 11, 2, 2.686976298210044});
    iTree29.push_back({0, 0, -2, 1, -2.0});
    iTree29.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree29);

    iTree30.push_back({1, 4, 7, 3, 0.37313368400666985});
    iTree30.push_back({2, 3, 2, 2, 2.6830935768725253});
    iTree30.push_back({0, 0, -2, 1, -2.0});
    iTree30.push_back({0, 0, -2, 1, -2.0});
    iTree30.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree30);

    iTree31.push_back({1, 2, 1, 3, 1.8718934943290764});
    iTree31.push_back({0, 0, -2, 1, -2.0});
    iTree31.push_back({3, 4, 4, 2, 105.23897066490677});
    iTree31.push_back({0, 0, -2, 1, -2.0});
    iTree31.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree31);

    iTree32.push_back({1, 2, 8, 3, 1.845113700002673});
    iTree32.push_back({0, 0, -2, 1, -2.0});
    iTree32.push_back({3, 4, 12, 2, 1033.5579368302404});
    iTree32.push_back({0, 0, -2, 1, -2.0});
    iTree32.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree32);

    iTree33.push_back({1, 4, 5, 3, 2.953902180332571});
    iTree33.push_back({2, 3, 1, 2, 1.8745042740785294});
    iTree33.push_back({0, 0, -2, 1, -2.0});
    iTree33.push_back({0, 0, -2, 1, -2.0});
    iTree33.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree33);

    iTree34.push_back({1, 4, 9, 3, 6.1105709066089435});
    iTree34.push_back({2, 3, 8, 2, 1.8703326723683855});
    iTree34.push_back({0, 0, -2, 1, -2.0});
    iTree34.push_back({0, 0, -2, 1, -2.0});
    iTree34.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree34);

    iTree35.push_back({1, 4, 3, 3, 18.373240720048866});
    iTree35.push_back({2, 3, 7, 2, 0.4182921275426621});
    iTree35.push_back({0, 0, -2, 1, -2.0});
    iTree35.push_back({0, 0, -2, 1, -2.0});
    iTree35.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree35);

    iTree36.push_back({1, 2, 12, 3, 1151.8160099172107});
    iTree36.push_back({0, 0, -2, 1, -2.0});
    iTree36.push_back({3, 4, 9, 2, 6.096697506615899});
    iTree36.push_back({0, 0, -2, 1, -2.0});
    iTree36.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree36);

    iTree37.push_back({1, 2, 9, 3, 5.462234902087378});
    iTree37.push_back({0, 0, -2, 1, -2.0});
    iTree37.push_back({3, 4, 5, 2, 2.931041927181425});
    iTree37.push_back({0, 0, -2, 1, -2.0});
    iTree37.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree37);

    iTree38.push_back({1, 4, 0, 3, 13.966077275658664});
    iTree38.push_back({2, 3, 6, 2, 2.728311631580025});
    iTree38.push_back({0, 0, -2, 1, -2.0});
    iTree38.push_back({0, 0, -2, 1, -2.0});
    iTree38.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree38);

    iTree39.push_back({1, 4, 10, 3, 1.1833794217740239});
    iTree39.push_back({2, 3, 8, 2, 1.9675386194794262});
    iTree39.push_back({0, 0, -2, 1, -2.0});
    iTree39.push_back({0, 0, -2, 1, -2.0});
    iTree39.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree39);

    iTree40.push_back({1, 4, 10, 3, 1.1596016406026117});
    iTree40.push_back({2, 3, 1, 2, 1.9441194118787435});
    iTree40.push_back({0, 0, -2, 1, -2.0});
    iTree40.push_back({0, 0, -2, 1, -2.0});
    iTree40.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree40);

    iTree41.push_back({1, 4, 5, 3, 2.9156265584797003});
    iTree41.push_back({2, 3, 6, 2, 2.925561595656393});
    iTree41.push_back({0, 0, -2, 1, -2.0});
    iTree41.push_back({0, 0, -2, 1, -2.0});
    iTree41.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree41);

    iTree42.push_back({1, 2, 6, 3, 2.9104925276390117});
    iTree42.push_back({0, 0, -2, 1, -2.0});
    iTree42.push_back({3, 4, 5, 2, 2.9933424117903495});
    iTree42.push_back({0, 0, -2, 1, -2.0});
    iTree42.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree42);

    iTree43.push_back({1, 2, 6, 3, 2.8707085423388334});
    iTree43.push_back({0, 0, -2, 1, -2.0});
    iTree43.push_back({3, 4, 1, 2, 1.948435401512308});
    iTree43.push_back({0, 0, -2, 1, -2.0});
    iTree43.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree43);

    iTree44.push_back({1, 4, 7, 3, 0.36222675504687146});
    iTree44.push_back({2, 3, 0, 2, 13.402699504158605});
    iTree44.push_back({0, 0, -2, 1, -2.0});
    iTree44.push_back({0, 0, -2, 1, -2.0});
    iTree44.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree44);

    iTree45.push_back({1, 2, 5, 3, 2.612757452894987});
    iTree45.push_back({0, 0, -2, 1, -2.0});
    iTree45.push_back({3, 4, 0, 2, 13.776948704217649});
    iTree45.push_back({0, 0, -2, 1, -2.0});
    iTree45.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree45);

    iTree46.push_back({1, 2, 8, 3, 1.8294006930260625});
    iTree46.push_back({0, 0, -2, 1, -2.0});
    iTree46.push_back({3, 4, 4, 2, 107.12936069217015});
    iTree46.push_back({0, 0, -2, 1, -2.0});
    iTree46.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree46);

    iTree47.push_back({1, 4, 3, 3, 18.324257922705506});
    iTree47.push_back({2, 3, 7, 2, 0.34911967410875444});
    iTree47.push_back({0, 0, -2, 1, -2.0});
    iTree47.push_back({0, 0, -2, 1, -2.0});
    iTree47.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree47);

    iTree48.push_back({1, 2, 11, 3, 2.6915644232359432});
    iTree48.push_back({0, 0, -2, 1, -2.0});
    iTree48.push_back({3, 4, 9, 2, 5.253895622224698});
    iTree48.push_back({0, 0, -2, 1, -2.0});
    iTree48.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree48);

    iTree49.push_back({1, 2, 2, 3, 2.5190493975733848});
    iTree49.push_back({0, 0, -2, 1, -2.0});
    iTree49.push_back({3, 4, 9, 2, 6.190707631046468});
    iTree49.push_back({0, 0, -2, 1, -2.0});
    iTree49.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree49);

    iTree50.push_back({1, 2, 2, 3, 2.3715961176004914});
    iTree50.push_back({0, 0, -2, 1, -2.0});
    iTree50.push_back({3, 4, 6, 2, 3.1412229499438933});
    iTree50.push_back({0, 0, -2, 1, -2.0});
    iTree50.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree50);

    iTree51.push_back({1, 4, 0, 3, 13.921398692295751});
    iTree51.push_back({2, 3, 11, 2, 2.8540613807300805});
    iTree51.push_back({0, 0, -2, 1, -2.0});
    iTree51.push_back({0, 0, -2, 1, -2.0});
    iTree51.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree51);

    iTree52.push_back({1, 4, 10, 3, 1.1426018101102708});
    iTree52.push_back({2, 3, 2, 2, 2.686732836187821});
    iTree52.push_back({0, 0, -2, 1, -2.0});
    iTree52.push_back({0, 0, -2, 1, -2.0});
    iTree52.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree52);

    iTree53.push_back({1, 4, 7, 3, 0.41674026626765454});
    iTree53.push_back({2, 3, 8, 2, 1.793334076045812});
    iTree53.push_back({0, 0, -2, 1, -2.0});
    iTree53.push_back({0, 0, -2, 1, -2.0});
    iTree53.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree53);

    iTree54.push_back({1, 4, 3, 3, 19.49333798851932});
    iTree54.push_back({2, 3, 10, 2, 1.110154255242584});
    iTree54.push_back({0, 0, -2, 1, -2.0});
    iTree54.push_back({0, 0, -2, 1, -2.0});
    iTree54.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree54);

    iTree55.push_back({1, 2, 6, 3, 3.037842431952647});
    iTree55.push_back({0, 0, -2, 1, -2.0});
    iTree55.push_back({3, 4, 12, 2, 1278.6243068513575});
    iTree55.push_back({0, 0, -2, 1, -2.0});
    iTree55.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree55);

    iTree56.push_back({1, 4, 5, 3, 2.807436270035513});
    iTree56.push_back({2, 3, 0, 2, 13.783741750447994});
    iTree56.push_back({0, 0, -2, 1, -2.0});
    iTree56.push_back({0, 0, -2, 1, -2.0});
    iTree56.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree56);

    iTree57.push_back({1, 2, 6, 3, 2.8437582551353});
    iTree57.push_back({0, 0, -2, 1, -2.0});
    iTree57.push_back({3, 4, 9, 2, 6.124929667668756});
    iTree57.push_back({0, 0, -2, 1, -2.0});
    iTree57.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree57);

    iTree58.push_back({1, 2, 2, 3, 2.412820946686826});
    iTree58.push_back({0, 0, -2, 1, -2.0});
    iTree58.push_back({3, 4, 5, 2, 2.8577422387068094});
    iTree58.push_back({0, 0, -2, 1, -2.0});
    iTree58.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree58);

    iTree59.push_back({1, 2, 0, 3, 13.569309105978366});
    iTree59.push_back({0, 0, -2, 1, -2.0});
    iTree59.push_back({3, 4, 4, 2, 112.66744175956931});
    iTree59.push_back({0, 0, -2, 1, -2.0});
    iTree59.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree59);

    iTree60.push_back({1, 4, 3, 3, 19.17038696408673});
    iTree60.push_back({2, 3, 1, 2, 1.9281598267513775});
    iTree60.push_back({0, 0, -2, 1, -2.0});
    iTree60.push_back({0, 0, -2, 1, -2.0});
    iTree60.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree60);

    iTree61.push_back({1, 2, 9, 3, 5.52344789269636});
    iTree61.push_back({0, 0, -2, 1, -2.0});
    iTree61.push_back({3, 4, 0, 2, 14.06133776771625});
    iTree61.push_back({0, 0, -2, 1, -2.0});
    iTree61.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree61);

    iTree62.push_back({1, 4, 10, 3, 1.1080773953811378});
    iTree62.push_back({2, 3, 5, 2, 2.976002149418511});
    iTree62.push_back({0, 0, -2, 1, -2.0});
    iTree62.push_back({0, 0, -2, 1, -2.0});
    iTree62.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree62);

    iTree63.push_back({1, 4, 7, 3, 0.3859857418047462});
    iTree63.push_back({2, 3, 4, 2, 103.50339486706228});
    iTree63.push_back({0, 0, -2, 1, -2.0});
    iTree63.push_back({0, 0, -2, 1, -2.0});
    iTree63.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree63);

    iTree64.push_back({1, 4, 0, 3, 14.29764718410212});
    iTree64.push_back({2, 3, 8, 2, 1.947300968426227});
    iTree64.push_back({0, 0, -2, 1, -2.0});
    iTree64.push_back({0, 0, -2, 1, -2.0});
    iTree64.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree64);

    iTree65.push_back({1, 4, 1, 3, 1.953255343138016});
    iTree65.push_back({2, 3, 10, 2, 1.1935889872702465});
    iTree65.push_back({0, 0, -2, 1, -2.0});
    iTree65.push_back({0, 0, -2, 1, -2.0});
    iTree65.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree65);

    iTree66.push_back({1, 4, 10, 3, 1.09971867374879});
    iTree66.push_back({2, 3, 1, 2, 1.9628739678774247});
    iTree66.push_back({0, 0, -2, 1, -2.0});
    iTree66.push_back({0, 0, -2, 1, -2.0});
    iTree66.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree66);

    iTree67.push_back({1, 2, 8, 3, 1.7360540588224058});
    iTree67.push_back({0, 0, -2, 1, -2.0});
    iTree67.push_back({3, 4, 3, 2, 18.47084974872062});
    iTree67.push_back({0, 0, -2, 1, -2.0});
    iTree67.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree67);

    iTree68.push_back({1, 2, 1, 3, 1.8997946204500593});
    iTree68.push_back({0, 0, -2, 1, -2.0});
    iTree68.push_back({3, 4, 2, 2, 2.6816509524642647});
    iTree68.push_back({0, 0, -2, 1, -2.0});
    iTree68.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree68);

    iTree69.push_back({1, 2, 6, 3, 2.8362411633138183});
    iTree69.push_back({0, 0, -2, 1, -2.0});
    iTree69.push_back({3, 4, 3, 2, 16.85077093562385});
    iTree69.push_back({0, 0, -2, 1, -2.0});
    iTree69.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree69);

    iTree70.push_back({1, 2, 12, 3, 1077.2350041369139});
    iTree70.push_back({0, 0, -2, 1, -2.0});
    iTree70.push_back({3, 4, 2, 2, 2.685824442970329});
    iTree70.push_back({0, 0, -2, 1, -2.0});
    iTree70.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree70);

    iTree71.push_back({1, 4, 0, 3, 14.07943320882496});
    iTree71.push_back({2, 3, 1, 2, 1.9378133608356531});
    iTree71.push_back({0, 0, -2, 1, -2.0});
    iTree71.push_back({0, 0, -2, 1, -2.0});
    iTree71.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree71);

    iTree72.push_back({1, 2, 11, 3, 2.768243782061591});
    iTree72.push_back({0, 0, -2, 1, -2.0});
    iTree72.push_back({3, 4, 11, 2, 2.84860043559443});
    iTree72.push_back({0, 0, -2, 1, -2.0});
    iTree72.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree72);

    iTree73.push_back({1, 2, 8, 3, 1.8822938748447138});
    iTree73.push_back({0, 0, -2, 1, -2.0});
    iTree73.push_back({3, 4, 10, 2, 1.1286614238556296});
    iTree73.push_back({0, 0, -2, 1, -2.0});
    iTree73.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree73);

    iTree74.push_back({1, 4, 5, 3, 2.880011589254566});
    iTree74.push_back({2, 3, 7, 2, 0.3888781665331812});
    iTree74.push_back({0, 0, -2, 1, -2.0});
    iTree74.push_back({0, 0, -2, 1, -2.0});
    iTree74.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree74);

    iTree75.push_back({1, 4, 7, 3, 0.3626207454141471});
    iTree75.push_back({2, 3, 2, 2, 2.713850614068966});
    iTree75.push_back({0, 0, -2, 1, -2.0});
    iTree75.push_back({0, 0, -2, 1, -2.0});
    iTree75.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree75);

    iTree76.push_back({1, 2, 5, 3, 2.4209641083277025});
    iTree76.push_back({0, 0, -2, 1, -2.0});
    iTree76.push_back({3, 4, 0, 2, 13.819569684395777});
    iTree76.push_back({0, 0, -2, 1, -2.0});
    iTree76.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree76);

    iTree77.push_back({1, 4, 2, 3, 2.713289871406287});
    iTree77.push_back({2, 3, 0, 2, 13.312581559397367});
    iTree77.push_back({0, 0, -2, 1, -2.0});
    iTree77.push_back({0, 0, -2, 1, -2.0});
    iTree77.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree77);

    iTree78.push_back({1, 4, 7, 3, 0.3753586954536757});
    iTree78.push_back({2, 3, 0, 2, 14.11568020562443});
    iTree78.push_back({0, 0, -2, 1, -2.0});
    iTree78.push_back({0, 0, -2, 1, -2.0});
    iTree78.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree78);

    iTree79.push_back({1, 4, 4, 3, 110.3029652695651});
    iTree79.push_back({2, 3, 0, 2, 13.679842110569952});
    iTree79.push_back({0, 0, -2, 1, -2.0});
    iTree79.push_back({0, 0, -2, 1, -2.0});
    iTree79.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree79);

    iTree80.push_back({1, 4, 10, 3, 1.2058493162297488});
    iTree80.push_back({2, 3, 8, 2, 1.9365138375837812});
    iTree80.push_back({0, 0, -2, 1, -2.0});
    iTree80.push_back({0, 0, -2, 1, -2.0});
    iTree80.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree80);

    iTree81.push_back({1, 2, 12, 3, 1218.0289129158616});
    iTree81.push_back({0, 0, -2, 1, -2.0});
    iTree81.push_back({3, 4, 6, 2, 3.2109501613886544});
    iTree81.push_back({0, 0, -2, 1, -2.0});
    iTree81.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree81);

    iTree82.push_back({1, 2, 9, 3, 4.4911895731697635});
    iTree82.push_back({0, 0, -2, 1, -2.0});
    iTree82.push_back({3, 4, 3, 2, 18.951976300759227});
    iTree82.push_back({0, 0, -2, 1, -2.0});
    iTree82.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree82);

    iTree83.push_back({1, 2, 8, 3, 1.8741550005071113});
    iTree83.push_back({0, 0, -2, 1, -2.0});
    iTree83.push_back({3, 4, 12, 2, 1182.9729490182237});
    iTree83.push_back({0, 0, -2, 1, -2.0});
    iTree83.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree83);

    iTree84.push_back({1, 4, 4, 3, 107.56474507393537});
    iTree84.push_back({2, 3, 11, 2, 2.8608807434080643});
    iTree84.push_back({0, 0, -2, 1, -2.0});
    iTree84.push_back({0, 0, -2, 1, -2.0});
    iTree84.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree84);

    iTree85.push_back({1, 2, 2, 3, 2.49305080282533});
    iTree85.push_back({0, 0, -2, 1, -2.0});
    iTree85.push_back({3, 4, 6, 2, 3.1795139769581273});
    iTree85.push_back({0, 0, -2, 1, -2.0});
    iTree85.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree85);

    iTree86.push_back({1, 4, 10, 3, 1.2177239238691588});
    iTree86.push_back({2, 3, 5, 2, 2.850018185591021});
    iTree86.push_back({0, 0, -2, 1, -2.0});
    iTree86.push_back({0, 0, -2, 1, -2.0});
    iTree86.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree86);

    iTree87.push_back({1, 2, 9, 3, 3.9214140302081666});
    iTree87.push_back({0, 0, -2, 1, -2.0});
    iTree87.push_back({3, 4, 6, 2, 3.1736169222632});
    iTree87.push_back({0, 0, -2, 1, -2.0});
    iTree87.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree87);

    iTree88.push_back({1, 4, 10, 3, 1.2256610949428284});
    iTree88.push_back({2, 3, 5, 2, 2.859608907429216});
    iTree88.push_back({0, 0, -2, 1, -2.0});
    iTree88.push_back({0, 0, -2, 1, -2.0});
    iTree88.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree88);

    iTree89.push_back({1, 2, 2, 3, 2.4776801173963605});
    iTree89.push_back({0, 0, -2, 1, -2.0});
    iTree89.push_back({3, 4, 7, 2, 0.31960884996643074});
    iTree89.push_back({0, 0, -2, 1, -2.0});
    iTree89.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree89);

    iTree90.push_back({1, 2, 12, 3, 1117.7254858555857});
    iTree90.push_back({0, 0, -2, 1, -2.0});
    iTree90.push_back({3, 4, 12, 2, 1270.6486092278028});
    iTree90.push_back({0, 0, -2, 1, -2.0});
    iTree90.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree90);

    iTree91.push_back({1, 2, 8, 3, 1.8111349766940905});
    iTree91.push_back({0, 0, -2, 1, -2.0});
    iTree91.push_back({3, 4, 1, 2, 1.8835081524023765});
    iTree91.push_back({0, 0, -2, 1, -2.0});
    iTree91.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree91);

    iTree92.push_back({1, 2, 8, 3, 1.685540597933894});
    iTree92.push_back({0, 0, -2, 1, -2.0});
    iTree92.push_back({3, 4, 10, 2, 1.1772198427807277});
    iTree92.push_back({0, 0, -2, 1, -2.0});
    iTree92.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree92);

    iTree93.push_back({1, 2, 1, 3, 1.8458556000593551});
    iTree93.push_back({0, 0, -2, 1, -2.0});
    iTree93.push_back({3, 4, 4, 2, 115.51630308456546});
    iTree93.push_back({0, 0, -2, 1, -2.0});
    iTree93.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree93);

    iTree94.push_back({1, 2, 2, 3, 2.5651842787969326});
    iTree94.push_back({0, 0, -2, 1, -2.0});
    iTree94.push_back({3, 4, 11, 2, 2.6960690014329085});
    iTree94.push_back({0, 0, -2, 1, -2.0});
    iTree94.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree94);

    iTree95.push_back({1, 2, 0, 3, 13.473795765316412});
    iTree95.push_back({0, 0, -2, 1, -2.0});
    iTree95.push_back({3, 4, 4, 2, 116.0785091501095});
    iTree95.push_back({0, 0, -2, 1, -2.0});
    iTree95.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree95);

    iTree96.push_back({1, 4, 7, 3, 0.4002205760657751});
    iTree96.push_back({2, 3, 0, 2, 14.25284369227196});
    iTree96.push_back({0, 0, -2, 1, -2.0});
    iTree96.push_back({0, 0, -2, 1, -2.0});
    iTree96.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree96);

    iTree97.push_back({1, 4, 4, 3, 110.61295400774709});
    iTree97.push_back({2, 3, 6, 2, 2.9602796442098276});
    iTree97.push_back({0, 0, -2, 1, -2.0});
    iTree97.push_back({0, 0, -2, 1, -2.0});
    iTree97.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree97);

    iTree98.push_back({1, 2, 5, 3, 2.4321563436455187});
    iTree98.push_back({0, 0, -2, 1, -2.0});
    iTree98.push_back({3, 4, 7, 2, 0.3296889896138525});
    iTree98.push_back({0, 0, -2, 1, -2.0});
    iTree98.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree98);

    iTree99.push_back({1, 4, 3, 3, 19.619137062047308});
    iTree99.push_back({2, 3, 11, 2, 2.8637697685896706});
    iTree99.push_back({0, 0, -2, 1, -2.0});
    iTree99.push_back({0, 0, -2, 1, -2.0});
    iTree99.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree99);

    iTree100.push_back({1, 4, 4, 3, 106.87412326637381});
    iTree100.push_back({2, 3, 3, 2, 17.138821954449607});
    iTree100.push_back({0, 0, -2, 1, -2.0});
    iTree100.push_back({0, 0, -2, 1, -2.0});
    iTree100.push_back({0, 0, -2, 1, -2.0});
    iForest.push_back(iTree100);

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
            while (current_node_id != 0 || length == 0 && tree[current_node_id].child_id_left != 0)
            {
                if (ndimData[i][tree[current_node_id].feature] < tree[current_node_id].threshold)
                {
                    current_node_id = tree[current_node_id].child_id_left;
                }
                else if (ndimData[i][tree[current_node_id].feature] >= tree[current_node_id].threshold)
                {
                    current_node_id = tree[current_node_id].child_id_right;
                }    
                else
                {
                    break;
                }
                    length += 1;
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
    }
    
    std::vector<float> scores;
    float score = 0;
    std::vector<float> average_length = edges;
    for (int i = 0; i < average_length.size(); i ++){
        score = pow(2, -average_length[i]/c(256));
        float scorep =  1 * score;// + 0.4775;
        scores.push_back(scorep);
    }

    for (size_t i = 0; i < scores.size(); i++)
    {
        std::cout << scores[i] << std::endl;
    }
    
}