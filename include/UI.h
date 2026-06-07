#ifndef UI_H
#define UI_H

#include <vector>

#include "FittingEngine.h"

class UI
{
public:

    static void showMenu();

    static void showComparison(
        const std::vector<Result>& results);

    static void showChart(
        const std::vector<Result>& results);

    static void showAnalysis(
        const Result& best);
};

#endif