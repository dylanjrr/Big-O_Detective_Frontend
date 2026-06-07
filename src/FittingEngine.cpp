#include "../include/FittingEngine.h"
#include "../include/ComplexityModel.h"

using namespace std;

double FittingEngine::computeR2(
    const vector<double>& x,
    const vector<double>& y)
{
    int n = x.size();

    double sumX = 0;
    double sumY = 0;
    double sumXY = 0;
    double sumX2 = 0;

    for(int i=0;i<n;i++)
    {
        sumX += x[i];
        sumY += y[i];
        sumXY += x[i]*y[i];
        sumX2 += x[i]*x[i];
    }

    double denominator =
        n*sumX2 - sumX*sumX;

    if(denominator == 0)
        return 0;

    double slope =
        (n*sumXY - sumX*sumY)
        / denominator;

    double intercept =
        (sumY - slope*sumX)/n;

    double meanY = sumY/n;

    double ssTotal = 0;
    double ssResidual = 0;

    for(int i=0;i<n;i++)
    {
        double predicted =
            slope*x[i] + intercept;

        ssTotal +=
            (y[i]-meanY)*(y[i]-meanY);

        ssResidual +=
            (y[i]-predicted)*(y[i]-predicted);
    }

    if(ssTotal == 0)
        return 0;

    return 1.0 - (ssResidual/ssTotal);
}

vector<Result> FittingEngine::evaluateAll(
    const vector<int>& sizes,
    const vector<double>& times)
{
    vector<Result> results;
    vector<double> transformed;

    transformed.clear();
    for(int n:sizes)
        transformed.push_back(
            ComplexityModel::constant(n));

    results.push_back(
        {"O(1)",
         computeR2(transformed,times)});

    transformed.clear();
    for(int n:sizes)
        transformed.push_back(
            ComplexityModel::logarithmic(n));

    results.push_back(
        {"O(log n)",
         computeR2(transformed,times)});

    transformed.clear();
    for(int n:sizes)
        transformed.push_back(
            ComplexityModel::linear(n));

    results.push_back(
        {"O(n)",
         computeR2(transformed,times)});

    transformed.clear();
    for(int n:sizes)
        transformed.push_back(
            ComplexityModel::linearithmic(n));

    results.push_back(
        {"O(n log n)",
         computeR2(transformed,times)});

    transformed.clear();
    for(int n:sizes)
        transformed.push_back(
            ComplexityModel::quadratic(n));

    results.push_back(
        {"O(n²)",
         computeR2(transformed,times)});

    transformed.clear();
    for(int n:sizes)
        transformed.push_back(
            ComplexityModel::cubic(n));

    results.push_back(
        {"O(n³)",
         computeR2(transformed,times)});

    return results;
}

Result FittingEngine::findBestModel(
    const vector<Result>& results)
{
    Result best = results[0];

    for(const auto& r:results)
    {
        if(r.score > best.score)
            best = r;
    }

    return best;
}