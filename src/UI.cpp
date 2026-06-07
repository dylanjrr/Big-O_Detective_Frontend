#include "../include/UI.h"

#include <iostream>
#include <iomanip>

using namespace std;

void UI::showMenu()
{
    cout << "\n";
    cout << "=================================\n";
    cout << "      BIG-O DETECTIVE\n";
    cout << "=================================\n";

    cout << "1. Load Dataset\n";
    cout << "2. Enter Dataset Manually\n";
    cout << "3. View Dataset\n";
    cout << "4. Run Analysis\n";
    cout << "5. Save Report\n";
    cout << "0. Exit\n\n";

    cout << "Choice: ";
}

void UI::showComparison(
    const vector<Result>& results)
{
    cout << "\nMODEL COMPARISON\n\n";

    for(const auto& r:results)
    {
        cout
            << setw(15)
            << r.modelName
            << r.score
            << endl;
    }
}

void UI::showChart(
    const vector<Result>& results)
{
    cout << "\nVISUALIZATION\n\n";

    for(const auto& r:results)
    {
        cout
            << setw(12)
            << r.modelName
            << " ";

        int bars =
            static_cast<int>(
                r.score * 30);

        for(int i=0;i<bars;i++)
            cout << '#';

        cout << endl;
    }
}

void UI::showAnalysis(
    const Result& best)
{
    cout << "\n";
    cout << "=========================\n";
    cout << "BEST MATCH FOUND\n";
    cout << "=========================\n";

    cout
        << "Complexity : "
        << best.modelName
        << endl;

    cout
        << "Confidence : "
        << best.score*100
        << "%"
        << endl;
}