#include <cassert>
#include "../include/Dataset.h"

int main()
{
    Dataset d;

    d.addData(100,0.2);

    assert(
        d.getInputSizes().size() == 1);

    assert(
        d.getRuntimes().size() == 1);

    return 0;
}