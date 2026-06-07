#ifndef BACKENDCLIENT_H
#define BACKENDCLIENT_H

#include <string>
#include <vector>
#include "FittingEngine.h"

class BackendClient
{
private:
    std::string baseUrl;
    std::string currentDatasetId;

public:
    BackendClient(const std::string& url = "http://localhost:5000");

    bool connect();

    bool createDataset(const std::vector<int>& sizes, const std::vector<double>& times);

    std::vector<Result> analyze();

    Result getBestModel(const std::vector<Result>& results);

    bool generateReport(const std::vector<Result>& results, const Result& best);

    void setBaseUrl(const std::string& url);

    std::string getCurrentDatasetId() const;
};

#endif
