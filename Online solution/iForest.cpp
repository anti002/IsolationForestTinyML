#include <vector>

class iForest {
    public:
    int t_trees, sample_size;
    iForest (int t_trees, int sample_size){
        t_trees = t_trees;
        sample_size = sample_size;
    }

    void fit(std::vector<std::vector<float>> dataSet){
    std::vector<iTree> forest;
    height_limit = 
};
};

