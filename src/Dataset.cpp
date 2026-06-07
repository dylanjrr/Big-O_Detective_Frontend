#include "../include/Dataset.h"

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

void Dataset::addData(int n, double t)
{
    if(n <= 0)
    {
        cout << "Error: Input size must be positive.\n";
        return;
    }
    if(t < 0)
    {
        cout << "Error: Runtime cannot be negative.\n";
        return;
    }
    inputSizes.push_back(n);
    runtimes.push_back(t);
}

bool Dataset::loadFromFile(const string& filename)
{
    ifstream file(filename);

    if(!file)
    {
        cout << "Error: Could not open file '" << filename << "'\n";
        return false;
    }

    inputSizes.clear();
    runtimes.clear();

    int n;
    double t;

    while(file >> n >> t)
    {
        addData(n,t);
    }

    return true;
}

void Dataset::display() const
{
    cout << "\n---------------------------------\n";
    cout << left
         << setw(15) << "Input Size"
         << setw(15) << "Runtime(ms)"
         << endl;

    cout << "---------------------------------\n";

    for(size_t i=0;i<inputSizes.size();i++)
    {
        cout << setw(15)
             << inputSizes[i]
             << setw(15)
             << runtimes[i]
             << endl;
    }
}

const vector<int>& Dataset::getInputSizes() const
{
    return inputSizes;
}

const vector<double>& Dataset::getRuntimes() const
{
    return runtimes;
}

bool Dataset::empty() const
{
    return inputSizes.empty();
}