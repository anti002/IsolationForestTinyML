#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

std::vector<std::vector<float>> parsedCsv;

std::string list = "13.29,1.97,2.68,16.8,102,3,3.23,0.31,1.66,6,1.07,2.84,1270";



int main(){

    char tab2[1024];
    strcpy(tab2, list.c_str());
    char * strtokIndx;
    strtokIndx = strtok(tab2,",");
    int i = 1;
    std::vector<float> parsedRow;
    parsedRow.push_back(std::stof(strtokIndx));
    while (i < 13){
    strtokIndx = strtok(NULL,",");
    parsedRow.push_back(std::stof(strtokIndx));
    i++;
    }
    parsedCsv.push_back(parsedRow);
    std::cout << parsedCsv[0][0];
}