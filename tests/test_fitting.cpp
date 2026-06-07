#include <cassert>

#include "../include/FittingEngine.h"

using namespace std;

int main()
{
    vector<int> n =
    {
        100,
        200,
        400,
        800
    };

    vector<double> t =
    {
        100,
        200,
        400,
        800
    };

    FittingEngine engine;

    auto results =
        engine.evaluateAll(n,t);

    Result best =
        engine.findBestModel(results);

    assert(
        best.modelName == "O(n)");

    return 0;
}