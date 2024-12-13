#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include <vector>
#include <ctime>
#include <iomanip>

using namespace std;

// Function to parse the log file
struct LogEntry {
    string timestamp;
    string logLevel;
    string message;
};

vector<LogEntry> parseLogFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        exit(1);
    }

    vector<LogEntry> logs;
    string line;
    regex logPattern(R"(\[(.?)\] \[(.?)\] (.*))");
    smatch matches;

    while (getline(file, line)) {
        if (regex_match(line, matches, logPattern)) {
            LogEntry log = {matches[1], matches[2], matches[3]};
            logs.push_back(log);
        }
    }
    file.close();
    return logs;
}

// Function to count logs by log level
map<string, int> countLogsByLevel(const vector<LogEntry>& logs) {
    map<string, int> logLevelCounts;
    for (const auto& log : logs) {
        logLevelCounts[log.logLevel]++;
    }
    return logLevelCounts;
}

// Function to count logs by hour
map<int, int> countLogsByHour(const vector<LogEntry>& logs) {
    map<int, int> hourlyCounts;
    for (const auto& log : logs) {
        struct tm timeStruct = {};
        strptime(log.timestamp.c_str(), "%a %b %d %H:%M:%S %Y", &timeStruct);
        hourlyCounts[timeStruct.tm_hour]++;
    }
    return hourlyCounts;
}

// Function to export results to a CSV file
void exportToCSV(const string& filename, const map<string, int>& data) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file for writing!" << endl;
        exit(1);
    }

    file << "Category,Count\n";
    for (const auto& pair : data) {
        file << pair.first << "," << pair.second << "\n";
    }
    file.close();
}

void exportToCSV(const string& filename, const map<int, int>& data) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file for writing!" << endl;
        exit(1);
    }

    file << "Hour,Count\n";
    for (const auto& pair : data) {
        file << pair.first << "," << pair.second << "\n";
    }
    file.close();
}

int main() {
    string logFilename = "log.txt";
    vector<LogEntry> logs = parseLogFile(logFilename);

    // Count logs by level
    map<string, int> logLevelCounts = countLogsByLevel(logs);

    // Count logs by hour
    map<int, int> hourlyCounts = countLogsByHour(logs);

    // Export results to CSV
    exportToCSV("log_level_counts.csv", logLevelCounts);
    exportToCSV("hourly_counts.csv", hourlyCounts);

    // Print summary to console
    cout << "Log Level Counts:" << endl;
    for (const auto& pair : logLevelCounts) {
        cout << pair.first << ": " << pair.second << endl;
    }

    cout << "\nHourly Log Counts:" << endl;
    for (const auto& pair : hourlyCounts) {
        cout << pair.first << ":00 - " << pair.second << endl;
    }

    return 0;
}
