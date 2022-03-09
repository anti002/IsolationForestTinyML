#include <vector>

class iTree
{
    public:
    int current_height, height_limit;
    iForest (int current_height, int height_limit){
        this.current_height = current_height;
        this.height_limit = height_limit;
    }
    
    void fit(std::vector<std::vector<float>> sub_sample)
    {
    
        if (current_height >= height_limit || sub_sample.size() <= 2 )
        {
            
        }
    };

};

