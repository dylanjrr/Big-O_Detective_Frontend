#include "../include/ComplexityModel.h"

#include <cmath>

double ComplexityModel::constant(int)
{
    return 1.0;
}

double ComplexityModel::logarithmic(int n)
{
    return log2(n);
}

double ComplexityModel::linear(int n)
{
    return n;
}

double ComplexityModel::linearithmic(int n)
{
    return n * log2(n);
}

double ComplexityModel::quadratic(int n)
{
    return static_cast<double>(n) * n;
}

double ComplexityModel::cubic(int n)
{
    return static_cast<double>(n) * n * n;
}