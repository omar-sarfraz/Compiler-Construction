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

void removeUnitProd(vector<tuple<string, vector<string>>> &productions){

    bool foundUnitProd = true;
    while(foundUnitProd){
        foundUnitProd = false;         
        for (int i = 0; i < productions.size(); ++i) {

            tuple<string, vector<string>> currentProduction = productions[i];
            string ruleName = get<0>(currentProduction);

            vector<string> rules = get<1>(currentProduction);            

            for (int j = 0; j < rules.size(); ++j) {                
                if(rules[j].size() == 1 && isNonTerminal(rules[j][0])){                                        
                    foundUnitProd = true;
                    string unitProdName = rules[j];
                    
                    string currentProdName = ruleName;
                    
                    vector<string> newRules = rules;
                    newRules.erase(newRules.begin() + j);
                    
                    for (int k = 0; k < productions.size(); ++k){
                        tuple<string, vector<string>> currentProduction = productions[k];
                        vector<string> rulesForUnit = get<1>(currentProduction);
                        string nameForUnit = get<0>(currentProduction);

                        if(nameForUnit == unitProdName){
                            for(int l = 0; l < rulesForUnit.size(); l++){
                                newRules.push_back(rulesForUnit[l]);
                            }
                        }                                         
                        
                    }
                    productions[i] = make_tuple(currentProdName, newRules);                                                          
                }
            }
        }

    }
}


int main() {

    string filename = "rules.txt";    

    vector <tuple <string, vector<string> > > productions;
    vector<string> alreadyRemoved;

    // Reading the productions in a vector
    readGrammer(productions, filename);

    // Print the Grammer
    cout << "Input Grammer: " << endl;
    printGrammer(productions);

    // Remove Unit Productions
    removeUnitProd(productions);

    cout << "Output Grammer: " << endl;
    printGrammer(productions);

    return 0;
}