#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <string>
#include <vector>

#include "FittingEngine.h"

class ReportGenerator
{
public:

    static void generate(
        const std::string& filename,
        const std::vector<Result>& results,
        const Result& best);
};

#endif