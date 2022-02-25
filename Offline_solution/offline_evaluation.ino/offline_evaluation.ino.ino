#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

struct Tree{
    int child_id_left, child_id_right, feature, n_samples;
    float threshold;
};

vector<vector<Tree>> iForest;
std::vector<Tree> iTree1;
std::vector<std::vector<std::string>> parsedCsv;

int i = 0;
void setup() 
{
    Serial.begin(9600);
    
    parsedCsv.push_back({"13.29", "1.97", "2.68", "16.8", "102.0", "3.0", "3.23", "0.31", "1.66", "6.0", "1.07", "2.84", "1270.0"});
    parsedCsv.push_back({"14.3", "1.92", "2.72", "20.0", "120.0", "2.8", "3.14", "0.33", "1.97", "6.2", "1.07", "2.65", "1280.0"});
    parsedCsv.push_back({"13.68", "1.83", "2.36", "17.2", "104.0", "2.42", "2.69", "0.42", "1.97", "3.84", "1.23", "2.87", "990.0"});
    parsedCsv.push_back({"14.06", "2.15", "2.61", "17.6", "121.0", "2.6", "2.51", "0.31", "1.25", "5.05", "1.06", "3.58", "1295.0"});
    parsedCsv.push_back({"14.22", "1.7", "2.3", "16.3", "118.0", "3.2", "3.0", "0.26", "2.03", "6.38", "0.94", "3.31", "970.0"});
    parsedCsv.push_back({"14.1", "2.02", "2.4", "18.8", "103.0", "2.75", "2.92", "0.32", "2.38", "6.2", "1.07", "2.75", "1060.0"});
    parsedCsv.push_back({"13.86", "1.35", "2.27", "16.0", "98.0", "2.98", "3.15", "0.22", "1.85", "7.22", "1.01", "3.55", "1045.0"});
    parsedCsv.push_back({"13.05", "1.65", "2.55", "18.0", "98.0", "2.45", "2.43", "0.29", "1.44", "4.25", "1.12", "2.51", "1105.0"});
    parsedCsv.push_back({"14.2", "1.76", "2.45", "15.2", "112.0", "3.27", "3.39", "0.34", "1.97", "6.75", "1.05", "2.85", "1450.0"});
    parsedCsv.push_back({"13.77", "1.9", "2.68", "17.1", "115.0", "3.0", "2.79", "0.39", "1.68", "6.3", "1.13", "2.93", "1375.0"});
    parsedCsv.push_back({"12.37", "0.94", "1.36", "10.6", "88.0", "1.98", "0.57", "0.28", "0.42", "1.95", "1.05", "1.82", "520.0"});
    parsedCsv.push_back({"12.33", "1.1", "2.28", "16.0", "101.0", "2.05", "1.09", "0.63", "0.41", "3.27", "1.25", "1.67", "680.0"});
    parsedCsv.push_back({"12.64", "1.36", "2.02", "16.8", "100.0", "2.02", "1.41", "0.53", "0.62", "5.75", "0.98", "1.59", "450.0"});
    parsedCsv.push_back({"13.67", "1.25", "1.92", "18.0", "94.0", "2.1", "1.79", "0.32", "0.73", "3.8", "1.23", "2.46", "630.0"});
    parsedCsv.push_back({"12.37", "1.13", "2.16", "19.0", "87.0", "3.5", "3.1", "0.19", "1.87", "4.45", "1.22", "2.87", "420.0"});
    parsedCsv.push_back({"12.17", "1.45", "2.53", "19.0", "104.0", "1.89", "1.75", "0.45", "1.03", "2.95", "1.45", "2.23", "355.0"});
    parsedCsv.push_back({"12.37", "1.21", "2.56", "18.1", "98.0", "2.42", "2.65", "0.37", "2.08", "4.6", "1.19", "2.3", "678.0"});
    parsedCsv.push_back({"13.11", "1.01", "1.7", "15.0", "78.0", "2.98", "3.18", "0.26", "2.28", "5.3", "1.12", "3.18", "502.0"});
    parsedCsv.push_back({"12.37", "1.17", "1.92", "19.6", "78.0", "2.11", "2.0", "0.27", "1.04", "4.68", "1.12", "3.48", "510.0"});
    parsedCsv.push_back({"13.34", "0.94", "2.36", "17.0", "110.0", "2.53", "1.3", "0.55", "0.42", "3.17", "1.02", "1.93", "750.0"});
    parsedCsv.push_back({"12.21", "1.19", "1.75", "16.8", "151.0", "1.85", "1.28", "0.14", "2.5", "2.85", "1.28", "3.07", "718.0"});
    parsedCsv.push_back({"12.29", "1.61", "2.21", "20.4", "103.0", "1.1", "1.02", "0.37", "1.46", "3.05", "0.906", "1.82", "870.0"});
    parsedCsv.push_back({"13.86", "1.51", "2.67", "25.0", "86.0", "2.95", "2.86", "0.21", "1.87", "3.38", "1.36", "3.16", "410.0"});
    parsedCsv.push_back({"13.49", "1.66", "2.24", "24.0", "87.0", "1.88", "1.84", "0.27", "1.03", "3.74", "0.98", "2.78", "472.0"});
    parsedCsv.push_back({"12.99", "1.67", "2.6", "30.0", "139.0", "3.3", "2.89", "0.21", "1.96", "3.35", "1.31", "3.5", "985.0"});
    parsedCsv.push_back({"11.96", "1.09", "2.3", "21.0", "101.0", "3.38", "2.14", "0.13", "1.65", "3.21", "0.99", "3.13", "886.0"});
    parsedCsv.push_back({"11.66", "1.88", "1.92", "16.0", "97.0", "1.61", "1.57", "0.34", "1.15", "3.8", "1.23", "2.14", "428.0"});
    parsedCsv.push_back({"13.03", "0.9", "1.71", "16.0", "86.0", "1.95", "2.03", "0.24", "1.46", "4.6", "1.19", "2.48", "392.0"});
    parsedCsv.push_back({"11.84", "2.89", "2.23", "18.0", "112.0", "1.72", "1.32", "0.43", "0.95", "2.65", "0.96", "2.52", "500.0"});
    parsedCsv.push_back({"12.33", "0.99", "1.95", "14.8", "136.0", "1.9", "1.85", "0.35", "2.76", "3.4", "1.06", "2.31", "750.0"});
    parsedCsv.push_back({"12.7", "3.87", "2.4", "23.0", "101.0", "2.83", "2.55", "0.43", "1.95", "2.57", "1.19", "3.13", "463.0"});
    parsedCsv.push_back({"12.0", "0.92", "2.0", "19.0", "86.0", "2.42", "2.26", "0.3", "1.43", "2.5", "1.38", "3.12", "278.0"});
    parsedCsv.push_back({"12.72", "1.81", "2.2", "18.8", "86.0", "2.2", "2.53", "0.26", "1.77", "3.9", "1.16", "3.14", "714.0"});
    parsedCsv.push_back({"12.08", "1.13", "2.51", "24.0", "78.0", "2.0", "1.58", "0.4", "1.4", "2.2", "1.31", "2.72", "630.0"});
    parsedCsv.push_back({"13.05", "3.86", "2.32", "22.5", "85.0", "1.65", "1.59", "0.61", "1.62", "4.8", "0.84", "2.01", "515.0"});
    parsedCsv.push_back({"11.84", "0.89", "2.58", "18.0", "94.0", "2.2", "2.21", "0.22", "2.35", "3.05", "0.79", "3.08", "520.0"});
    parsedCsv.push_back({"12.67", "0.98", "2.24", "18.0", "99.0", "2.2", "1.94", "0.3", "1.46", "2.62", "1.23", "3.16", "450.0"});
    parsedCsv.push_back({"12.16", "1.61", "2.31", "22.8", "90.0", "1.78", "1.69", "0.43", "1.56", "2.45", "1.33", "2.26", "495.0"});
    parsedCsv.push_back({"11.65", "1.67", "2.62", "26.0", "88.0", "1.92", "1.61", "0.4", "1.34", "2.6", "1.36", "3.21", "562.0"});
    parsedCsv.push_back({"11.64", "2.06", "2.46", "21.6", "84.0", "1.95", "1.69", "0.48", "1.35", "2.8", "1.0", "2.75", "680.0"});
    parsedCsv.push_back({"12.08", "1.33", "2.3", "23.6", "70.0", "2.2", "1.59", "0.42", "1.38", "1.74", "1.07", "3.21", "625.0"});
    parsedCsv.push_back({"12.08", "1.83", "2.32", "18.5", "81.0", "1.6", "1.5", "0.52", "1.64", "2.4", "1.08", "2.27", "480.0"});
    parsedCsv.push_back({"12.0", "1.51", "2.42", "22.0", "86.0", "1.45", "1.25", "0.5", "1.63", "3.6", "1.05", "2.65", "450.0"});
    parsedCsv.push_back({"12.69", "1.53", "2.26", "20.7", "80.0", "1.38", "1.46", "0.58", "1.62", "3.05", "0.96", "2.06", "495.0"});
    parsedCsv.push_back({"12.29", "2.83", "2.22", "18.0", "88.0", "2.45", "2.25", "0.25", "1.99", "2.15", "1.15", "3.3", "290.0"});
    parsedCsv.push_back({"11.62", "1.99", "2.28", "18.0", "98.0", "3.02", "2.26", "0.17", "1.35", "3.25", "1.16", "2.96", "345.0"});
    parsedCsv.push_back({"12.47", "1.52", "2.2", "19.0", "162.0", "2.5", "2.27", "0.32", "3.28", "2.6", "1.16", "2.63", "937.0"});
    parsedCsv.push_back({"11.81", "2.12", "2.74", "21.5", "134.0", "1.6", "0.99", "0.14", "1.56", "2.5", "0.95", "2.26", "625.0"});
    parsedCsv.push_back({"12.29", "1.41", "1.98", "16.0", "85.0", "2.55", "2.5", "0.29", "1.77", "2.9", "1.23", "2.74", "428.0"});
    parsedCsv.push_back({"12.37", "1.07", "2.1", "18.5", "88.0", "3.52", "3.75", "0.24", "1.95", "4.5", "1.04", "2.77", "660.0"});
    parsedCsv.push_back({"12.29", "3.17", "2.21", "18.0", "88.0", "2.85", "2.99", "0.45", "2.81", "2.3", "1.42", "2.83", "406.0"});
    parsedCsv.push_back({"12.08", "2.08", "1.7", "17.5", "97.0", "2.23", "2.17", "0.26", "1.4", "3.3", "1.27", "2.96", "710.0"});
    parsedCsv.push_back({"12.6", "1.34", "1.9", "18.5", "88.0", "1.45", "1.36", "0.29", "1.35", "2.45", "1.04", "2.77", "562.0"});
    parsedCsv.push_back({"12.34", "2.45", "2.46", "21.0", "98.0", "2.56", "2.11", "0.34", "1.31", "2.8", "0.8", "3.38", "438.0"});
    parsedCsv.push_back({"11.82", "1.72", "1.88", "19.5", "86.0", "2.5", "1.64", "0.37", "1.42", "2.06", "0.94", "2.44", "415.0"});
    parsedCsv.push_back({"12.51", "1.73", "1.98", "20.5", "85.0", "2.2", "1.92", "0.32", "1.48", "2.94", "1.04", "3.57", "672.0"});
    parsedCsv.push_back({"12.42", "2.55", "2.27", "22.0", "90.0", "1.68", "1.84", "0.66", "1.42", "2.7", "0.86", "3.3", "315.0"});
    parsedCsv.push_back({"12.25", "1.73", "2.12", "19.0", "80.0", "1.65", "2.03", "0.37", "1.63", "3.4", "1.0", "3.17", "510.0"});
    parsedCsv.push_back({"12.72", "1.75", "2.28", "22.5", "84.0", "1.38", "1.76", "0.48", "1.63", "3.3", "0.88", "2.42", "488.0"});
    parsedCsv.push_back({"12.22", "1.29", "1.94", "19.0", "92.0", "2.36", "2.04", "0.39", "2.08", "2.7", "0.86", "3.02", "312.0"});
    parsedCsv.push_back({"11.61", "1.35", "2.7", "20.0", "94.0", "2.74", "2.92", "0.29", "2.49", "2.65", "0.96", "3.26", "680.0"});
    parsedCsv.push_back({"11.46", "3.74", "1.82", "19.5", "107.0", "3.18", "2.58", "0.24", "3.58", "2.9", "0.75", "2.81", "562.0"});
    parsedCsv.push_back({"12.52", "2.43", "2.17", "21.0", "88.0", "2.55", "2.27", "0.26", "1.22", "2.0", "0.9", "2.78", "325.0"});
    parsedCsv.push_back({"11.76", "2.68", "2.92", "20.0", "103.0", "1.75", "2.03", "0.6", "1.05", "3.8", "1.23", "2.5", "607.0"});
    parsedCsv.push_back({"11.41", "0.74", "2.5", "21.0", "88.0", "2.48", "2.01", "0.42", "1.44", "3.08", "1.1", "2.31", "434.0"});
    parsedCsv.push_back({"12.08", "1.39", "2.5", "22.5", "84.0", "2.56", "2.29", "0.43", "1.04", "2.9", "0.93", "3.19", "385.0"});
    parsedCsv.push_back({"11.03", "1.51", "2.2", "21.5", "85.0", "2.46", "2.17", "0.52", "2.01", "1.9", "1.71", "2.87", "407.0"});
    parsedCsv.push_back({"11.82", "1.47", "1.99", "20.8", "86.0", "1.98", "1.6", "0.3", "1.53", "1.95", "0.95", "3.33", "495.0"});
    parsedCsv.push_back({"12.42", "1.61", "2.19", "22.5", "108.0", "2.0", "2.09", "0.34", "1.61", "2.06", "1.06", "2.96", "345.0"});
    parsedCsv.push_back({"12.77", "3.43", "1.98", "16.0", "80.0", "1.63", "1.25", "0.43", "0.83", "3.4", "0.7", "2.12", "372.0"});
    parsedCsv.push_back({"12.0", "3.43", "2.0", "19.0", "87.0", "2.0", "1.64", "0.37", "1.87", "1.28", "0.93", "3.05", "564.0"});
    parsedCsv.push_back({"11.45", "2.4", "2.42", "20.0", "96.0", "2.9", "2.79", "0.32", "1.83", "3.25", "0.8", "3.39", "625.0"});
    parsedCsv.push_back({"11.56", "2.05", "3.23", "28.5", "119.0", "3.18", "5.08", "0.47", "1.87", "6.0", "0.93", "3.69", "465.0"});
    parsedCsv.push_back({"12.42", "4.43", "2.73", "26.5", "102.0", "2.2", "2.13", "0.43", "1.71", "2.08", "0.92", "3.12", "365.0"});
    parsedCsv.push_back({"13.05", "5.8", "2.13", "21.5", "86.0", "2.62", "2.65", "0.3", "2.01", "2.6", "0.73", "3.1", "380.0"});
    parsedCsv.push_back({"11.87", "4.31", "2.39", "21.0", "82.0", "2.86", "3.03", "0.21", "2.91", "2.8", "0.75", "3.64", "380.0"});
    parsedCsv.push_back({"12.07", "2.16", "2.17", "21.0", "85.0", "2.6", "2.65", "0.37", "1.35", "2.76", "0.86", "3.28", "378.0"});
    parsedCsv.push_back({"12.43", "1.53", "2.29", "21.5", "86.0", "2.74", "3.15", "0.39", "1.77", "3.94", "0.69", "2.84", "352.0"});
    parsedCsv.push_back({"11.79", "2.13", "2.78", "28.5", "92.0", "2.13", "2.24", "0.58", "1.76", "3.0", "0.97", "2.44", "466.0"});
    parsedCsv.push_back({"12.37", "1.63", "2.3", "24.5", "88.0", "2.22", "2.45", "0.4", "1.9", "2.12", "0.89", "2.78", "342.0"});
    parsedCsv.push_back({"12.04", "4.3", "2.38", "22.0", "80.0", "2.1", "1.75", "0.42", "1.35", "2.6", "0.79", "2.57", "580.0"});
    parsedCsv.push_back({"12.86", "1.35", "2.32", "18.0", "122.0", "1.51", "1.25", "0.21", "0.94", "4.1", "0.76", "1.29", "630.0"});
    parsedCsv.push_back({"12.88", "2.99", "2.4", "20.0", "104.0", "1.3", "1.22", "0.24", "0.83", "5.4", "0.74", "1.42", "530.0"});
    parsedCsv.push_back({"12.81", "2.31", "2.4", "24.0", "98.0", "1.15", "1.09", "0.27", "0.83", "5.7", "0.66", "1.36", "560.0"});
    parsedCsv.push_back({"12.7", "3.55", "2.36", "21.5", "106.0", "1.7", "1.2", "0.17", "0.84", "5.0", "0.78", "1.29", "600.0"});
    parsedCsv.push_back({"12.51", "1.24", "2.25", "17.5", "85.0", "2.0", "0.58", "0.6", "1.25", "5.45", "0.75", "1.51", "650.0"});
    parsedCsv.push_back({"12.6", "2.46", "2.2", "18.5", "94.0", "1.62", "0.66", "0.63", "0.94", "7.1", "0.73", "1.58", "695.0"});
    parsedCsv.push_back({"12.25", "4.72", "2.54", "21.0", "89.0", "1.38", "0.47", "0.53", "0.8", "3.85", "0.75", "1.27", "720.0"});
    parsedCsv.push_back({"12.53", "5.51", "2.64", "25.0", "96.0", "1.79", "0.6", "0.63", "1.1", "5.0", "0.82", "1.69", "515.0"});
    parsedCsv.push_back({"13.49", "3.59", "2.19", "19.5", "88.0", "1.62", "0.48", "0.58", "0.88", "5.7", "0.81", "1.82", "580.0"});
    parsedCsv.push_back({"12.84", "2.96", "2.61", "24.0", "101.0", "2.32", "0.6", "0.53", "0.81", "4.92", "0.89", "2.15", "590.0"});
    parsedCsv.push_back({"12.93", "2.81", "2.7", "21.0", "96.0", "1.54", "0.5", "0.53", "0.75", "4.6", "0.77", "2.31", "600.0"});
    parsedCsv.push_back({"13.36", "2.56", "2.35", "20.0", "89.0", "1.4", "0.5", "0.37", "0.64", "5.6", "0.7", "2.47", "780.0"});
    parsedCsv.push_back({"13.52", "3.17", "2.72", "23.5", "97.0", "1.55", "0.52", "0.5", "0.55", "4.35", "0.89", "2.06", "520.0"});
    parsedCsv.push_back({"13.62", "4.95", "2.35", "20.0", "92.0", "2.0", "0.8", "0.47", "1.02", "4.4", "0.91", "2.05", "550.0"});
    parsedCsv.push_back({"12.25", "3.88", "2.2", "18.5", "112.0", "1.38", "0.78", "0.29", "1.14", "8.21", "0.65", "2.0", "855.0"});
    parsedCsv.push_back({"13.16", "3.57", "2.15", "21.0", "102.0", "1.5", "0.55", "0.43", "1.3", "4.0", "0.6", "1.68", "830.0"});
    parsedCsv.push_back({"13.88", "5.04", "2.23", "20.0", "80.0", "0.98", "0.34", "0.4", "0.68", "4.9", "0.58", "1.33", "415.0"});
    parsedCsv.push_back({"12.87", "4.61", "2.48", "21.5", "86.0", "1.7", "0.65", "0.47", "0.86", "7.65", "0.54", "1.86", "625.0"});
    parsedCsv.push_back({"13.32", "3.24", "2.38", "21.5", "92.0", "1.93", "0.76", "0.45", "1.25", "8.42", "0.55", "1.62", "650.0"});
    parsedCsv.push_back({"13.08", "3.9", "2.36", "21.5", "113.0", "1.41", "1.39", "0.34", "1.14", "9.4", "0.57", "1.33", "550.0"});
    parsedCsv.push_back({"13.5", "3.12", "2.62", "24.0", "123.0", "1.4", "1.57", "0.22", "1.25", "8.6", "0.59", "1.3", "500.0"});
    parsedCsv.push_back({"12.79", "2.67", "2.48", "22.0", "112.0", "1.48", "1.36", "0.24", "1.26", "10.8", "0.48", "1.47", "480.0"});
    parsedCsv.push_back({"13.11", "1.9", "2.75", "25.5", "116.0", "2.2", "1.28", "0.26", "1.56", "7.1", "0.61", "1.33", "425.0"});
    parsedCsv.push_back({"13.23", "3.3", "2.28", "18.5", "98.0", "1.8", "0.83", "0.61", "1.87", "10.52", "0.56", "1.51", "675.0"});
    parsedCsv.push_back({"12.58", "1.29", "2.1", "20.0", "103.0", "1.48", "0.58", "0.53", "1.4", "7.6", "0.58", "1.55", "640.0"});
    parsedCsv.push_back({"13.17", "5.19", "2.32", "22.0", "93.0", "1.74", "0.63", "0.61", "1.55", "7.9", "0.6", "1.48", "725.0"});
    parsedCsv.push_back({"13.84", "4.12", "2.38", "19.5", "89.0", "1.8", "0.83", "0.48", "1.56", "9.01", "0.57", "1.64", "480.0"});
    parsedCsv.push_back({"12.45", "3.03", "2.64", "27.0", "97.0", "1.9", "0.58", "0.63", "1.14", "7.5", "0.67", "1.73", "880.0"});
    parsedCsv.push_back({"14.34", "1.68", "2.7", "25.0", "98.0", "2.8", "1.31", "0.53", "2.7", "13.0", "0.57", "1.96", "660.0"});
    parsedCsv.push_back({"13.48", "1.67", "2.64", "22.5", "89.0", "2.6", "1.1", "0.52", "2.29", "11.75", "0.57", "1.78", "620.0"});
    parsedCsv.push_back({"12.36", "3.83", "2.38", "21.0", "88.0", "2.3", "0.92", "0.5", "1.04", "7.65", "0.56", "1.58", "520.0"});
    parsedCsv.push_back({"13.69", "3.26", "2.54", "20.0", "107.0", "1.83", "0.56", "0.5", "0.8", "5.88", "0.96", "1.82", "680.0"});
    parsedCsv.push_back({"12.85", "3.27", "2.58", "22.0", "106.0", "1.65", "0.6", "0.6", "0.96", "5.58", "0.87", "2.11", "570.0"});
    parsedCsv.push_back({"12.96", "3.45", "2.35", "18.5", "106.0", "1.39", "0.7", "0.4", "0.94", "5.28", "0.68", "1.75", "675.0"});
    parsedCsv.push_back({"13.78", "2.76", "2.3", "22.0", "90.0", "1.35", "0.68", "0.41", "1.03", "9.58", "0.7", "1.68", "615.0"});
    parsedCsv.push_back({"13.73", "4.36", "2.26", "22.5", "88.0", "1.28", "0.47", "0.52", "1.15", "6.62", "0.78", "1.75", "520.0"});
    parsedCsv.push_back({"13.45", "3.7", "2.6", "23.0", "111.0", "1.7", "0.92", "0.43", "1.46", "10.68", "0.85", "1.56", "695.0"});
    parsedCsv.push_back({"12.82", "3.37", "2.3", "19.5", "88.0", "1.48", "0.66", "0.4", "0.97", "10.26", "0.72", "1.75", "685.0"});
    parsedCsv.push_back({"13.58", "2.58", "2.69", "24.5", "105.0", "1.55", "0.84", "0.39", "1.54", "8.66", "0.74", "1.8", "750.0"});
    parsedCsv.push_back({"13.4", "4.6", "2.86", "25.0", "112.0", "1.98", "0.96", "0.27", "1.11", "8.5", "0.67", "1.92", "630.0"});
    parsedCsv.push_back({"12.2", "3.03", "2.32", "19.0", "96.0", "1.25", "0.49", "0.4", "0.73", "5.5", "0.66", "1.83", "510.0"});
    parsedCsv.push_back({"12.77", "2.39", "2.28", "19.5", "86.0", "1.39", "0.51", "0.48", "0.64", "9.899999", "0.57", "1.63", "470.0"});
    parsedCsv.push_back({"14.16", "2.51", "2.48", "20.0", "91.0", "1.68", "0.7", "0.44", "1.24", "9.7", "0.62", "1.71", "660.0"});
    parsedCsv.push_back({"13.71", "5.65", "2.45", "20.5", "95.0", "1.68", "0.61", "0.52", "1.06", "7.7", "0.64", "1.74", "740.0"});
    parsedCsv.push_back({"13.4", "3.91", "2.48", "23.0", "102.0", "1.8", "0.75", "0.43", "1.41", "7.3", "0.7", "1.56", "750.0"});
    parsedCsv.push_back({"13.27", "4.28", "2.26", "20.0", "120.0", "1.59", "0.69", "0.43", "1.35", "10.2", "0.59", "1.56", "835.0"});
    parsedCsv.push_back({"13.17", "2.59", "2.37", "20.0", "120.0", "1.65", "0.68", "0.53", "1.46", "9.3", "0.6", "1.62", "840.0"});
    parsedCsv.push_back({"14.13", "4.1", "2.74", "24.5", "96.0", "2.05", "0.76", "0.56", "1.35", "9.2", "0.61", "1.6", "560.0"});

    
    
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

std::vector<float> path_length(std::vector<vector<Tree>> forest, std::vector<std::vector<std::string>> parsedCsv)
{
    std::vector<float> edges;
    for (size_t i = 0; i < parsedCsv.size(); i++)
    {
        std::vector<float> path;

        for (size_t j = 0; j < iForest.size(); j++)
        {
            std::vector<Tree> tree = iForest[j];
            int current_node_id = 0;
            int length = 0;
            
            while (length == 0 || tree[current_node_id].child_id_left != 0)
            {
                float splitValue_attribute = std::stof(parsedCsv[i][tree[current_node_id].feature]);
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
            path.push_back(path_length);
        }

        float average_path = 0;
        for(int k = 0; k < iForest.size(); k++)
        {
            average_path += path[k];
        }
        average_path = average_path/path.size();
        edges.push_back(average_path);
        path.clear();
    }
    return edges;
}

std::vector<float> decision_function(std::vector<vector<Tree>> forest, std::vector<std::vector<std::string>> parsedCsv)
{
    std::vector<float> scores;
    float score = 0;
    std::vector<float> average_length = path_length(forest, parsedCsv);
    for (size_t i = 0; i < average_length.size(); i ++)
    {
        float scorep =  0.5 - pow(2, (-1 * average_length[i])/c(129));
        scores.push_back(scorep);
    }
    return scores;
}

void loop() 
{
    std::vector<float> scores_pred = decision_function(iForest, parsedCsv);
    for (size_t i = 0; i < scores_pred.size(); i++)
    {
        Serial.println(scores_pred[i], 8);
    }  
}
