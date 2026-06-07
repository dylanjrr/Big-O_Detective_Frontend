#include "../include/ReportGenerator.h"

#include <fstream>
#include <filesystem>

using namespace std;

void ReportGenerator::generate(
    const string& filename,
    const vector<Result>& results,
    const Result& best)
{
    filesystem::path filePath(filename);
    filesystem::path parentDir = filePath.parent_path();
    
    if(!parentDir.empty() && !filesystem::exists(parentDir))
    {
        filesystem::create_directories(parentDir);
    }

    ofstream file(filename);

    file << "BIG-O DETECTIVE REPORT\n";
    file << "======================\n\n";

    for(const auto& r:results)
    {
        file
            << r.modelName
            << " : "
            << r.score
            << "\n";
    }

    file << "\n";
    file << "BEST MODEL : "
         << best.modelName
         << "\n";

    file << "CONFIDENCE : "
         << best.score*100
         << "%\n";
}