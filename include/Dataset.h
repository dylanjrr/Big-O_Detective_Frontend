#ifndef DATASET_H
#define DATASET_H

#include <vector>
#include <string>

class Dataset
{
private:
    std::vector<int> inputSizes;
    std::vector<double> runtimes;

public:
    void addData(int n, double t);

    bool loadFromFile(const std::string& filename);

    void display() const;

    const std::vector<int>& getInputSizes() const;
    const std::vector<double>& getRuntimes() const;

    bool empty() const;
};

#endif