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

void addRule(vector<string> &rules, string rule){
    vector<string>::iterator i;

    bool ruleExists = false;
    for(i = rules.begin(); i != rules.end(); ++i){
        if(*i == rule){
            ruleExists = true;
        }
    }

    if(!ruleExists){
        rules.push_back(rule);
    }
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
                                addRule(newRules, currentRule);
                                addRule(newRules, "$");                                                             
                            }else{
                                // newRules.push_back(rulesForNull[l]);
                                addRule(newRules, rulesForNull[l]);
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
                                        addRule(newRules, newRule);
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
                                addRule(newRules, rulesForUnit[l]);
                                // newRules.push_back(rulesForUnit[l]);
                            }
                        }                                         
                        
                    }
                    productions[i] = make_tuple(currentProdName, newRules);                                                          
                }
            }
        }

    }
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

        if(alphaRules.size() != 0){
            newProductions.push_back(make_tuple(ruleName, betaRules));
            alphaRules.push_back("$");
            newProductions.push_back(make_tuple(ruleName + "'", alphaRules));
        }else{
            newProductions.push_back(make_tuple(ruleName, rules));
        }
    }

    productions = newProductions;
    
}

int main() {

    string filename = "rules.txt";    

    vector <tuple <string, vector<string> > > productions;
    vector<string> alreadyRemoved;

    readGrammer(productions, filename);

    cout << "Input Grammer: " << endl;
    printGrammer(productions);

    removeUnitProd(productions);
    cout << "Output Grammer after Unit Removal: " << endl;
    printGrammer(productions);

    removeNull(productions, alreadyRemoved);
    cout << "Output Grammer after Null Removal: " << endl;
    printGrammer(productions);

    removeDirectRecursion(productions);
    cout << "Output Grammer after Direct LR Removal: " << endl;
    printGrammer(productions);

    return 0;
}