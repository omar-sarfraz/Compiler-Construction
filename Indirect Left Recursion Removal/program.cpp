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

bool isNonTerminal(char ch){
    if((ch >= 'A' && ch <= 'Z')){        
        return true;
    }else{
        return false;
    }
}

void checkLR(vector<tuple<string, vector<string>>> &productions, char nonTerminal, bool &foundILR){
    for(int i=0; i<productions.size(); ++i){
        tuple<string, vector<string>> currentProduction = productions[i];
        string ruleName = get<0>(currentProduction);
        vector<string> rules = get<1>(currentProduction);        

        for (int j = 0; j < rules.size(); ++j){
            if(isNonTerminal(rules[j][0]) && rules[j][0] == nonTerminal){
                foundILR = true;
                return;
            } else if(isNonTerminal(rules[j][0])){
                checkLR(productions, rules[j][0], foundILR);
            } else {
                foundILR = false;
            }
        }
    }
}

void removeIndirectLR(vector<tuple<string, vector<string>>> &productions){
        
    for(int i=0; i<productions.size(); ++i){
        tuple<string, vector<string>> currentProduction = productions[i];
        string ruleName = get<0>(currentProduction);
        vector<string> rules = get<1>(currentProduction);
        
        bool hasILR = false;

        for (int j = 0; j < rules.size(); ++j){
            if(isNonTerminal(rules[j][0]) && rules[j][0] != ruleName[0]){
                checkLR(productions, rules[j][0], hasILR);
            }
        }

        if(hasILR){
            cout << "Current Rule " + ruleName + " has Indirect Left Recursion";
        }else{
            cout << "Current Rule " + ruleName + " does not have Indirect Left Recursion";
        }
        cout << endl;
    }
    
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
    removeIndirectLR(productions);

    cout << "Output Grammer: " << endl;
    printGrammer(productions);

    return 0;
}