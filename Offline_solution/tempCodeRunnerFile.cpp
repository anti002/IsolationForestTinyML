std::vector<float> decision_function(std::vector<vector<Tree>> forest, std::vector<std::vector<std::string>> parsedCsv)
{
    std::vector<float> scores;
    float score = 0;
    std::vector<float> average_length = path_length(forest, parsedCsv);
    for (size_t i = 0; i < average_length.size(); i ++)
    {
        float scorep =  0.5 - pow(2, (-1 * average_length[i])/c(parsedCsv.size()));
        scores.push_back(scorep);
    }
    return scores;
}