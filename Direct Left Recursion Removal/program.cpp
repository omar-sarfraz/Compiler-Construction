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

void removeDirectRecursion(vector<tuple<string, vector<string>>> &productions){
    
    vector <tuple <string, vector<string> > > newProductions;

    for(int i=0; i<productions.size(); ++i){
        tuple<string, vector<string>> currentProduction = productions[i];
        string ruleName = get<0>(currentProduction);
        vector<string> rules = get<1>(currentProduction);

        vector<string> alphaRules;
        vector<string> betaRules;

        for (int j = 0; j < rules.size(); ++j){
            if(ruleName[0] == rules[j][0]){
                //Current Rule has left recursion            
                alphaRules.push_back(rules[j].substr(1, rules[j].size() - 1) + ruleName + "'");                
            }else{
                betaRules.push_back(rules[j] + ruleName + "'");
            }
        }        

        newProductions.push_back(make_tuple(ruleName, betaRules));
        if(alphaRules.size() != 0){
            alphaRules.push_back("$");
            newProductions.push_back(make_tuple(ruleName + "'", alphaRules));
        }
    }

    productions = newProductions;
    
}


int main() {

    string filename = "rules.txt";    

    vector <tuple <string, vector<string> > > productions;

    // Reading the productions in a vector
    readGrammer(productions, filename);

    // Print the Grammer
    cout << "Input Grammer: " << endl;
    printGrammer(productions);

    // Remove Null Productions
    removeDirectRecursion(productions);

    cout << "Output Grammer: " << endl;
    printGrammer(productions);

    return 0;
}