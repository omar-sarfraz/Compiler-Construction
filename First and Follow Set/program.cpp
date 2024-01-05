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

void findFirstSet(vector<tuple<string, vector<string>>> &productions, vector<string> &firstSet, string currentNonTerminal){
    for (int i=0; i<productions.size(); i++){
        tuple<string, vector<string>> currentProduction = productions[i];
        
        if(get<0>(currentProduction) == currentNonTerminal){            
            vector<string> rules = get<1>(currentProduction);

            for(int j=0; j<rules.size(); ++j){
                string currentRule = rules[j];

                if(isNonTerminal(currentRule[0])){
                    vector<string> foundFirstSet;
                    findFirstSet(productions, foundFirstSet, string(1, currentRule[0]));

                    for(int k=0; k<foundFirstSet.size(); ++k){
                        if(foundFirstSet[k] == "$"){
                            if(currentRule.size() > 1){
                                if(isNonTerminal(currentRule[1])){
                                    currentRule = currentRule.substr(1); 
                                    findFirstSet(productions, foundFirstSet, string(1, currentRule[0]));
                                }else{
                                    addRule(firstSet, string(1, currentRule[1]));                                
                                }
                            }else{
                                addRule(firstSet, "$");
                            }
                        }else{
                            addRule(firstSet, foundFirstSet[k]);
                        }
                    }
                }else{
                    addRule(firstSet, string(1, currentRule[0]));
                }
            }
        }else{
            continue;
        }

    }
}

void findFollowSet(vector<tuple<string, vector<string>>> &productions, vector<string> &followSet, string currentNonTerminal, vector<tuple<string, vector<string>>> firstSets, string rootElement){
    if(currentNonTerminal == rootElement){
        followSet.push_back("$");
    }
    for (int i=0; i<productions.size(); i++){
        tuple<string, vector<string>> currentProduction = productions[i];
        vector<string> rules = get<1>(currentProduction);
        string currentRuleName = get<0>(currentProduction);

        for(int j=0; j<rules.size(); ++j){
            for(int k=0; k<rules[j].size(); ++k){
                if(rules[j][k] == currentNonTerminal[0]){
                    if(rules[j][k+1]){
                        // Follow element exist
                        if(isNonTerminal(rules[j][k+1])){
                            for(int l=0; l<firstSets.size(); ++l){
                                tuple<string, vector<string>> temp = firstSets[l];
                                if(get<0>(temp)[0] == rules[j][k+1]){
                                    vector<string> tempFirstSet = get<1>(temp);
                                    for(int m=0; m<tempFirstSet.size(); ++m){
                                        if(tempFirstSet[m] != "$"){
                                            addRule(followSet, tempFirstSet[m]);
                                        }
                                    }
                                }
                            }
                        }else{
                            addRule(followSet, string(1, rules[j][k+1]));
                        }
                    }else{
                        // Follow element does not exist
                        findFollowSet(productions, followSet, currentRuleName, firstSets, rootElement);
                    }
                }
            }
        }
    }
}


void findSets(vector<tuple<string, vector<string>>> &productions){
    vector<tuple<string, vector<string>>> firstSets;
    vector<tuple<string, vector<string>>> followSets;

    for (int i=0; i<productions.size(); i++){
        tuple<string, vector<string>> currentProduction = productions[i];
        string currentNonTerminal = get<0>(currentProduction);
        vector<string> firstSet;
        
        findFirstSet(productions, firstSet, currentNonTerminal);
        firstSets.push_back(make_tuple(currentNonTerminal, firstSet));
    }
    cout << "First Sets" << endl;
    printGrammer(firstSets);

    for (int i=0; i<productions.size(); i++){
        tuple<string, vector<string>> currentProduction = productions[i];
        string currentNonTerminal = get<0>(currentProduction);
        vector<string> followSet;

        string rootElement = get<0>(productions[0]);

        findFollowSet(productions, followSet, currentNonTerminal, firstSets, rootElement);
        followSets.push_back(make_tuple(currentNonTerminal, followSet));
    }
    cout << "Follow Sets" << endl;
    printGrammer(followSets);
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

    findSets(productions);
    return 0;
}