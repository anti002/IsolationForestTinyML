#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

std::vector<std::vector<float> > parsedCsv;

void parseCSV()
{
    std::ifstream  data("test.csv");
    std::string line;
    while(std::getline(data,line))
    {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<std::string> parsedRow;
        while(std::getline(lineStream,cell,','))
        {
            parsedRow.push_back(float(cell));
        }

        parsedCsv.push_back(parsedRow);
    }
};

int main()
{
    parseCSV();

    std::cout << parsedCsv[0][1] << std::endl;
}