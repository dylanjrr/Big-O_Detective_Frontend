#ifndef COMPLEXITYMODEL_H
#define COMPLEXITYMODEL_H

class ComplexityModel
{
public:

    static double constant(int n);

    static double logarithmic(int n);

    static double linear(int n);

    static double linearithmic(int n);

    static double quadratic(int n);

    static double cubic(int n);
};

#endif