#include <iostream>

#include "../include/UI.h"
#include "../include/Dataset.h"
#include "../include/FittingEngine.h"
#include "../include/ReportGenerator.h"
#include "../include/BackendClient.h"

using namespace std;

int main()
{
    Dataset dataset;

    vector<Result> results;
    Result best;

    BackendClient backendClient;
    bool useBackend = false;

    cout << "Use backend server? (y/n): ";
    char backendChoice;
    cin >> backendChoice;

    if(backendChoice == 'y' || backendChoice == 'Y')
    {
        useBackend = true;
        if(!backendClient.connect())
        {
            cout << "Failed to connect to backend. Using local analysis.\n";
            useBackend = false;
        }
        else
        {
            cout << "Connected to backend server.\n";
        }
    }

    int choice;

    do
    {
        UI::showMenu();

        cin >> choice;

        switch(choice)
        {
        case 1:
        {
            string filename;

            cout << "File name: ";
            cin >> filename;

            if(dataset.loadFromFile(filename))
                cout << "Loaded successfully.\n";
            else
                cout << "Failed.\n";

            break;
        }

        case 2:
        {
            int n;
            double t;

            cout << "Input Size: ";
            cin >> n;

            cout << "Runtime: ";
            cin >> t;

            dataset.addData(n,t);

            break;
        }

        case 3:
        {
            dataset.display();
            break;
        }

        case 4:
        {
            if(dataset.empty())
            {
                cout << "Load data first.\n";
                break;
            }

            if(useBackend)
            {
                if(!backendClient.createDataset(
                    dataset.getInputSizes(),
                    dataset.getRuntimes()))
                {
                    cout << "Failed to create dataset on backend.\n";
                    break;
                }

                results = backendClient.analyze();
                best = backendClient.getBestModel(results);
            }
            else
            {
                FittingEngine engine;

                results =
                    engine.evaluateAll(
                        dataset.getInputSizes(),
                        dataset.getRuntimes());

                best =
                    engine.findBestModel(
                        results);
            }

            UI::showComparison(results);

            UI::showChart(results);

            UI::showAnalysis(best);

            break;
        }

        case 5:
        {
            if(results.empty())
            {
                cout << "Run analysis first.\n";
                break;
            }

            if(useBackend)
            {
                backendClient.generateReport(results, best);
            }
            else
            {
                ReportGenerator::generate(
                    "reports/report.txt",
                    results,
                    best);

                cout
                    << "Report saved.\n";
            }

            break;
        }

        case 0:
            cout << "Goodbye.\n";
            break;

        default:
            cout << "Invalid choice.\n";
        }

        if(choice != 0)
        {
            cout
                << "\nPress Enter...";
            cin.ignore();
            cin.get();
        }

    }while(choice != 0);

    return 0;
}