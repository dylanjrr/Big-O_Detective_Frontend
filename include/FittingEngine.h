#ifndef FITTINGENGINE_H
#define FITTINGENGINE_H

#include <string>
#include <vector>

struct Result
{
    std::string modelName;
    double score;
};

class FittingEngine
{
public:

    std::vector<Result> evaluateAll(
        const std::vector<int>& sizes,
        const std::vector<double>& times);

    Result findBestModel(
        const std::vector<Result>& results);

private:

    double computeR2(
        const std::vector<double>& x,
        const std::vector<double>& y);
};

#endif