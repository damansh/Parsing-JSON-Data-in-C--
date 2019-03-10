// Daman Sharma

#include <iostream>
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

using namespace std;

int main(int argc, const char * argv[]) {
    
    string path;
    
    // Prompts user to enter the path of the json file
    cout << "Enter the entire path of the test_results.json file " << endl;
    cin >> path;
    
    ifstream i(path);
    
    // Determines if there is an error locating the file
    if(i.fail()) {
        cout << "Cannot locate the test_results.json file" << endl;
        return 0;
    }
    
    // Parses the json file
    json j = json::parse(i);

    // Traverses through the json file to find the tests that have passed, failed, or
    // have been blocked. Saves this in three separate maps (trees) and prints it after
    // traversing through it.
    for(int k = 0; k < j["test_suites"].size(); k++) {
        int testPassed = 0, testFailed = 0, testBlocked = 0, testTenSec = 0;
        map<string, double> pass;
        map<string, double> fail;
        map<string, int> blocked;
        
        json suiteName = j["test_suites"][k]["suite_name"];
        cout << "Suite Name: " << suiteName << endl;
        
        for(int i = 0; i < j["test_suites"][k]["results"].size(); i++) {
            string statusToCheck = j["test_suites"][k]["results"][i]["status"].dump();
            string name = j["test_suites"][k]["results"][i]["test_name"].dump();
            string time = j["test_suites"][k]["results"][i]["time"].dump();
            
            if(statusToCheck == "\"pass\"") {
                double timeDouble = stod(time.substr(1, time.size()-1));
                pass.insert(pair<string, double>(name,timeDouble));
                testPassed++;
                if(timeDouble > 10) testTenSec++;
            } else if(statusToCheck == "\"fail\"") {
                double timeDouble = stod(time.substr(1, time.size()-1));
                fail.insert(pair<string, double>(name,timeDouble));
                testFailed++;
                if(timeDouble > 10) testTenSec++;
            } else {
                blocked.insert(pair<string,int>(name,0));
                testBlocked++;
            }
        }
        
        cout << "Total number of tests that passed: " << testPassed << endl;
        for(auto i=pass.begin(); i != pass.end(); i++) {
            cout << "Test Name: " << i->first << " Time: " << i -> second << endl;
        }
        cout << endl;
        
        cout << "Total number of tests that failed: " << testFailed << endl;
        for(auto i=fail.begin(); i != fail.end(); i++) {
            cout << "Test Name: " << i->first << " Time: " << i -> second << endl;
        }
        cout << endl;
        
        cout << "Total number of tests that are blocked: " << testBlocked << endl;
        for(auto i=blocked.begin(); i != blocked.end(); i++) {
            cout << "Test Name: " << i->first << endl;
        }
        cout << endl;
        
        cout << "Total number of tests that took longer than 10 seconds to execute: " << testTenSec << endl;
        
        cout << endl;
    }
    return 0;
}
