#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>

using namespace std;

void readGrammer(vector<tuple<string, vector<string>>> &productions, string filename){

    ifstream file;
    string line;
    file.open(filename);

    while (!file.eof()){        
        getline(file, line);
        
        stringstream lineAsStream(line);
    
        string head;
        getline(lineAsStream, head, '=');

        vector<string> tempRules;
        string rule;
        while(getline(lineAsStream, rule, '|')){
            tempRules.push_back(rule);    
        }

        productions.push_back(make_tuple(head, tempRules));
    }
}

void printGrammer(vector<tuple<string, vector<string>>> &productions){    
    for (int i = 0; i < productions.size(); ++i) {

        tuple<string, vector<string>> currentProduction = productions[i];

        cout << get<0>(currentProduction) << "=";

        vector<string> rules = get<1>(currentProduction);
        for (int j = 0; j < rules.size(); ++j) {
            cout << rules[j];
            if (j != rules.size() - 1) {
                cout << "|";
            }
        }

        cout << endl;
    }
    cout << endl;
}

void removeNull(vector<tuple<string, vector<string>>> &productions, vector<string> &alreadyRemoved){

    bool foundNull = true;
    while(foundNull){
        for (int i = productions.size() - 1; i >= 0; --i) {

            tuple<string, vector<string>> currentProduction = productions[i];
            string ruleName = get<0>(currentProduction);

            vector<string> rules = get<1>(currentProduction);            

            foundNull = false;

            bool alreadyRemovedNull = false;
            for (int ai=0; ai < alreadyRemoved.size(); ++ai){
                if(alreadyRemoved[ai] == ruleName){
                    alreadyRemovedNull = true;
                }
            }

            for (int j = 0; j < rules.size(); ++j) {
                // cout << "Rulse k " << rules[j] << endl;
                if(rules[j] == "$"){

                    if(alreadyRemovedNull){
                        get<1>(productions[i]).erase(get<1>(productions[i]).begin() + j);
                        continue;
                    }

                    // Current Rule has null production
                    foundNull = true;
                    alreadyRemoved.push_back(ruleName);
                    // cout << ruleName << " -> " << "Null" << endl;
                    // cout << endl;
                    string nullRuleName = ruleName;

                    for (int k = 0; k < productions.size(); ++k){
                        tuple<string, vector<string>> currentProductionForNull = productions[k];
                        vector<string> rulesForNull = get<1>(currentProductionForNull);
                        string nameForNull = get<0>(currentProductionForNull);

                        vector<string> newRules;                        

                        for (int l=0; l < rulesForNull.size(); ++l){                        
                            string newRule = "";
                            string currentRule = rulesForNull[l];

                            bool removed = false;

                            if(currentRule == nullRuleName && rulesForNull[l] != "$"){                                
                                newRules.push_back(currentRule);
                                newRules.push_back("$");
                            }else{
                                newRules.push_back(rulesForNull[l]);
                                for(int m = 0; m < currentRule.size(); ++m){
                                    if(currentRule[m] != nullRuleName[0]){
                                        newRule += currentRule[m];
                                    }                                                             

                                    if(currentRule[m] == nullRuleName[0]){
                                        removed = true;
                                    }                                 
                                }

                        
                                if(removed){
                                    bool alreadyHasRule = false;
                                    for(int hi = 0; hi < rulesForNull.size(); ++hi){
                                        if(newRule == rulesForNull[hi]){
                                            alreadyHasRule = true;
                                        }
                                    }

                                    if(!alreadyHasRule){
                                        newRules.push_back(newRule);
                                    }
                                }

                            }

                        }                        
                        
                        productions[k] = make_tuple(nameForNull, newRules);                        
                    }
                    // printGrammer(productions);
                }
            }
        }

    }
}


int main() {

    string filename = "wasiq.txt";    

    vector <tuple <string, vector<string> > > productions;
    vector<string> alreadyRemoved;

    // Reading the productions in a vector
    readGrammer(productions, filename);

    // Print the Grammer
    cout << "Input Grammer: " << endl;
    printGrammer(productions);

    // Remove Null Productions
    removeNull(productions, alreadyRemoved);

    cout << "Output Grammer: " << endl;
    printGrammer(productions);

    return 0;
}