#include "../include/BackendClient.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
#else
#include <curl/curl.h>
#endif

using namespace std;

BackendClient::BackendClient(const string& url)
    : baseUrl(url), currentDatasetId("")
{
}

bool BackendClient::connect()
{
#ifdef _WIN32
    HINTERNET hInternet = InternetOpenA("Big-O Detective", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet)
    {
        cout << "Failed to initialize WinINet.\n";
        return false;
    }
    InternetCloseHandle(hInternet);
#else
    curl_global_init(CURL_GLOBAL_DEFAULT);
#endif
    return true;
}

bool BackendClient::createDataset(const vector<int>& sizes, const vector<double>& times)
{
    if (sizes.size() != times.size())
    {
        cout << "Error: Size mismatch between input sizes and runtimes.\n";
        return false;
    }

    // Build JSON payload
    stringstream json;
    json << "{\"points\":[";
    for (size_t i = 0; i < sizes.size(); i++)
    {
        json << "{\"inputSize\":" << sizes[i] << ",\"runtime\":" << times[i] << "}";
        if (i < sizes.size() - 1)
            json << ",";
    }
    json << "]}";

    string url = baseUrl + "/api/dataset";
    string response;

#ifdef _WIN32
    HINTERNET hInternet = InternetOpenA("Big-O Detective", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet)
    {
        cout << "Failed to initialize WinINet.\n";
        return false;
    }

    HINTERNET hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect)
    {
        cout << "Failed to connect to backend.\n";
        InternetCloseHandle(hInternet);
        return false;
    }

    string headers = "Content-Type: application/json";
    HttpSendRequestA(hConnect, headers.c_str(), headers.length(), (LPVOID)json.str().c_str(), json.str().length());

    char buffer[1024];
    DWORD bytesRead;
    while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0)
    {
        buffer[bytesRead] = '\0';
        response += buffer;
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
#else
    CURL* curl = curl_easy_init();
    if (!curl)
    {
        cout << "Failed to initialize CURL.\n";
        return false;
    }

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.str().c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        cout << "Failed to send request: " << curl_easy_strerror(res) << "\n";
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return false;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
#endif

    // Parse response to get dataset ID
    size_t idPos = response.find("\"id\":");
    if (idPos != string::npos)
    {
        size_t start = response.find("\"", idPos + 5) + 1;
        size_t end = response.find("\"", start);
        currentDatasetId = response.substr(start, end - start);
        return true;
    }

    cout << "Failed to parse dataset ID from response.\n";
    return false;
}

vector<Result> BackendClient::analyze()
{
    vector<Result> results;

    if (currentDatasetId.empty())
    {
        cout << "Error: No dataset loaded. Create a dataset first.\n";
        return results;
    }

    string url = baseUrl + "/api/analyze";
    string response;

#ifdef _WIN32
    stringstream json;
    json << "{\"datasetId\":\"" << currentDatasetId << "\"}";

    HINTERNET hInternet = InternetOpenA("Big-O Detective", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet)
    {
        cout << "Failed to initialize WinINet.\n";
        return results;
    }

    HINTERNET hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect)
    {
        cout << "Failed to connect to backend.\n";
        InternetCloseHandle(hInternet);
        return results;
    }

    string headers = "Content-Type: application/json";
    HttpSendRequestA(hConnect, headers.c_str(), headers.length(), (LPVOID)json.str().c_str(), json.str().length());

    char buffer[1024];
    DWORD bytesRead;
    while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0)
    {
        buffer[bytesRead] = '\0';
        response += buffer;
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
#else
    CURL* curl = curl_easy_init();
    if (!curl)
    {
        cout << "Failed to initialize CURL.\n";
        return results;
    }

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    stringstream json;
    json << "{\"datasetId\":\"" << currentDatasetId << "\"}";

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.str().c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        cout << "Failed to send request: " << curl_easy_strerror(res) << "\n";
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return results;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
#endif

    // Parse JSON response (simple parsing)
    size_t resultsPos = response.find("\"results\":");
    if (resultsPos != string::npos)
    {
        size_t start = response.find("[", resultsPos);
        size_t end = response.find("]", start);
        string resultsStr = response.substr(start, end - start + 1);

        // Simple parsing - extract modelName and score
        size_t pos = 0;
        while ((pos = resultsStr.find("\"modelName\":", pos)) != string::npos)
        {
            size_t nameStart = resultsStr.find("\"", pos + 12) + 1;
            size_t nameEnd = resultsStr.find("\"", nameStart);
            string modelName = resultsStr.substr(nameStart, nameEnd - nameStart);

            size_t scorePos = resultsStr.find("\"score\":", nameEnd);
            size_t scoreStart = scorePos + 8;
            size_t scoreEnd = resultsStr.find(",", scoreStart);
            if (scoreEnd == string::npos)
                scoreEnd = resultsStr.find("}", scoreStart);
            string scoreStr = resultsStr.substr(scoreStart, scoreEnd - scoreStart);
            double score = stod(scoreStr);

            results.push_back({modelName, score});
            pos = scoreEnd;
        }
    }

    return results;
}

Result BackendClient::getBestModel(const vector<Result>& results)
{
    if (results.empty())
    {
        return {"Unknown", 0.0};
    }

    Result best = results[0];
    for (const auto& r : results)
    {
        if (r.score > best.score)
            best = r;
    }
    return best;
}

bool BackendClient::generateReport(const vector<Result>& results, const Result& best)
{
    string url = baseUrl + "/api/report";
    string response;

    // Build JSON payload
    stringstream json;
    json << "{\"results\":[";
    for (size_t i = 0; i < results.size(); i++)
    {
        json << "{\"modelName\":\"" << results[i].modelName << "\",\"score\":" << results[i].score << "}";
        if (i < results.size() - 1)
            json << ",";
    }
    json << "],\"bestModel\":{\"modelName\":\"" << best.modelName << "\",\"score\":" << best.score << "}}";

#ifdef _WIN32
    HINTERNET hInternet = InternetOpenA("Big-O Detective", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet)
    {
        cout << "Failed to initialize WinINet.\n";
        return false;
    }

    HINTERNET hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect)
    {
        cout << "Failed to connect to backend.\n";
        InternetCloseHandle(hInternet);
        return false;
    }

    string headers = "Content-Type: application/json";
    HttpSendRequestA(hConnect, headers.c_str(), headers.length(), (LPVOID)json.str().c_str(), json.str().length());

    char buffer[1024];
    DWORD bytesRead;
    while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0)
    {
        buffer[bytesRead] = '\0';
        response += buffer;
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
#else
    CURL* curl = curl_easy_init();
    if (!curl)
    {
        cout << "Failed to initialize CURL.\n";
        return false;
    }

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.str().c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        cout << "Failed to send request: " << curl_easy_strerror(res) << "\n";
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return false;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
#endif

    cout << "Report generated by backend.\n";
    return true;
}

void BackendClient::setBaseUrl(const string& url)
{
    baseUrl = url;
}

string BackendClient::getCurrentDatasetId() const
{
    return currentDatasetId;
}

#ifndef _WIN32
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t totalSize = size * nmemb;
    string* response = static_cast<string*>(userp);
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}
#endif
