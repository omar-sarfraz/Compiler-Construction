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
                                if(nameForNull == nullRuleName && rulesForNull[l] == "$"){

                                }else{
                                    addRule(newRules, rulesForNull[l]);
                                }
                                
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

void addRootElement(vector<tuple<string, vector<string>>> &productions){
    vector<tuple<string, vector<string>>> newProductions;

    tuple<string, vector<string>> firstProduction = productions[0];
    string rootNonTerminal = get<0>(firstProduction);

    bool shouldAddNewRoot = false;
    for(int i=0; i<productions.size(); ++i){
        tuple<string, vector<string>> currentProduction = productions[i];        
        vector<string> rules = get<1>(currentProduction);        

        for (int j = 0; j < rules.size(); ++j) {
            for (int k=0; k<rules[j].size(); ++k){
                if(rules[j][k] == rootNonTerminal[0]){
                    shouldAddNewRoot = true;
                }
            }
        }
    }

    if(!shouldAddNewRoot){
        return;
    }

    string newRoot = rootNonTerminal+"'";
    vector<string> newRootRules;
    newRootRules.push_back(rootNonTerminal);

    newProductions.push_back(make_tuple(newRoot, newRootRules));
    for(int i=0; i<productions.size(); ++i){
        newProductions.push_back(productions[i]);
    }
    productions=newProductions;
}

void handleTerminalsConversion(vector<tuple<string, vector<string>>> &productions, char &nextRuleToAdd){
    vector<tuple<char, char>> addedTerminalRules;

    // Looping over all productions in grammer
    for(int i=0; i<productions.size(); ++i){
        tuple<string, vector<string>> currentProduction = productions[i];   
        string currentProductionName = get<0>(currentProduction);     
        vector<string> rules = get<1>(currentProduction);     

        // Looping over rules of 1 productio
        for (int j = 0; j < rules.size(); ++j) {
            if(rules[j].size() >= 2){   
                // Looping over 1 rule string      
                for (int k=0; k<rules[j].size(); ++k){
                    char currentLetter = rules[j][k];                    
                    if(!isNonTerminal(currentLetter)){
                        bool alreadyAddedTerminalRule = false;
                        char alreadyAddedRule;
                        for(int l=0; l<addedTerminalRules.size(); ++l){
                            if(get<0>(addedTerminalRules[l]) == currentLetter){
                                alreadyAddedTerminalRule = true;
                                alreadyAddedRule = get<1>(addedTerminalRules[l]);
                            }
                        }

                        if(alreadyAddedTerminalRule){
                            string newRule = rules[j];
                            newRule[k] = alreadyAddedRule;

                            rules[j] = newRule;
                        }else{
                            // Adding new production for each terminal
                            vector<string> temp;
                            temp.push_back(string(1, currentLetter));

                            productions.push_back(make_tuple(string(1, nextRuleToAdd), temp));

                            string newRule = rules[j];
                            newRule[k] = nextRuleToAdd;

                            rules[j] = newRule;

                            addedTerminalRules.push_back(make_tuple(currentLetter, nextRuleToAdd));
                            ++nextRuleToAdd;
                        }                        
                    }
                }

                productions[i] = make_tuple(currentProductionName, rules);
            }            
        }        
    }
}

void handleNonTerminalsConversion(vector<tuple<string, vector<string>>> &productions, char &nextRuleToAdd){
    vector<tuple<string, char>> addedNonTerminalRules;

    for(int i=0; i<productions.size(); ++i){
        tuple<string, vector<string>> currentProduction = productions[i];   
        string currentProductionName = get<0>(currentProduction);     
        vector<string> rules = get<1>(currentProduction);

        vector<string> newRules;
        newRules = rules;      

        // Handling non terminals longer than 2
        for (int k=0; k<rules.size(); ++k){
            if(rules[k].size() > 2){
                string newRule = rules[k];

                string firstHalf = newRule.substr(0, 1);
                string secondHalf = newRule.substr(1);

                bool alreadyAddedNonTerminalRule = false;
                string alreadyAddedRule;
                for(int l=0; l<addedNonTerminalRules.size(); ++l){
                    if(get<0>(addedNonTerminalRules[l]) == secondHalf){
                        alreadyAddedNonTerminalRule = true;
                        alreadyAddedRule = get<1>(addedNonTerminalRules[l]);
                    }
                }

                if(alreadyAddedNonTerminalRule){
                    newRules[k] = firstHalf + alreadyAddedRule;
                }else{                    
                    vector<string> temp;
                    temp.push_back(secondHalf);

                    productions.push_back(make_tuple(string(1, nextRuleToAdd), temp));

                    newRules[k] = firstHalf + nextRuleToAdd;
                    addedNonTerminalRules.push_back(make_tuple(secondHalf, nextRuleToAdd));
                    ++nextRuleToAdd;
                }
                productions[i] = make_tuple(currentProductionName, newRules);
            }
        }
    }

}

void convertToCNF(vector<tuple<string, vector<string>>> &productions){
    tuple<string, vector<string>> lastProduction = productions.back();
    string ruleName = get<0>(lastProduction);
    char nextRuleToAdd = ruleName[0] + 1;

    handleTerminalsConversion(productions, nextRuleToAdd);
    handleNonTerminalsConversion(productions, nextRuleToAdd);
}

int main() {

    string filename = "rules.txt";    

    vector <tuple <string, vector<string> > > productions;
    vector<string> alreadyRemoved;

    readGrammer(productions, filename);

    cout << "Input Grammer: " << endl;
    printGrammer(productions);

    addRootElement(productions);

    removeNull(productions, alreadyRemoved);
    cout << "Output Grammer after Null Removal: " << endl;
    printGrammer(productions);
    
    removeUnitProd(productions);
    cout << "Output Grammer after Unit Removal: " << endl;
    printGrammer(productions);

    convertToCNF(productions);
    cout << "Output Grammer after conversion: " << endl;
    printGrammer(productions);

    return 0;
}