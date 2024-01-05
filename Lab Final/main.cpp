#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

# define ROWS 10
# define COLUMNS 13

void createTT(string filename, string data[][COLUMNS]){

    ifstream file(filename);
    if (!file) {
        cout << "Failed to open the CSV file." << endl;
        return;
    }

    // Read and parse the CSV data
    for (int row = 0; row < ROWS; row++) {
        string line;
        if (!getline(file, line)) {
            cout << "Error reading CSV file." << endl;
            return;
        }

        if(row == 0){
            line = line.substr(3);
            cout << line << endl;
        }

        stringstream lineAsStream(line);
        string cell;

        for (int col = 0; col < COLUMNS; col++) {
            if (!getline(lineAsStream, cell, ',')) {
                cout << "Error in CSV cell." << endl;
                return;
            }
    
            data[row][col] = cell;            
        }
    }

    file.close();
}

void printVector(vector<string> &input, bool newLine){    
    for (int i = 0; i < input.size(); ++i) {
        if(newLine){
            cout << input[i] << endl;
        } else {
            cout << input[i] << " ";
        }
    }
    cout << endl;
}

void readTokens(vector<string> &stack, vector<string> &input, string filename){
    ifstream file;  
    string line;
    file.open(filename);

    while (!file.eof()){        
        getline(file, line);
        input.push_back(line);
    }

    // printVector(input);
}

enum Terminals {
    the, a, cat, dog, man, women, chased, caught, in,
    on, with, dot, End_Marker, Empty, ErrorTerm
};

Terminals getTerminal(string word) {
    if (word == "the") {
        return the;
    } else if (word == "a") {
        return a;
    } else if (word == "cat") {
        return cat;
    } else if (word == "dog") {
        return dog;
    } else if (word == "man") {
        return man;
    } else if (word == "women") {
        return women;
    } else if (word == "chased") {
        return caught;
    } else if (word == "in") {
        return in;
    } else if (word == "on") {
        return on;
    } else if (word == "with") {
        return with;
    } else if (word == ".") {
        return dot;
    } else if (word == "$") {
        return End_Marker;
    } else if (word == "#") {
        return Empty;
    } else {
        return ErrorTerm;
    }
}

enum NonTerminals {
    Sentence, NounPhrase, NounPhrasePrime, 
    PrepositionalPhrase, Determiner, Noun, 
    Verb, Preposition, VerbPhrase, VerbPhrasePrime, ErrorNonTerm
};

NonTerminals getNonTerminal(string word) {
    if (word == "Sentence") {
        return Sentence;
    } else if (word == "NounPhrase") {
        return NounPhrase;
    } else if (word == "NounPhrasePrime") {
        return NounPhrasePrime;
    } else if (word == "PrepositionalPhrase") {
        return PrepositionalPhrase;
    } else if (word == "Determiner") {
        return Determiner;
    } else if (word == "Noun") {
        return Noun;
    } else if (word == "Verb") {
        return Verb;
    } else if (word == "Preposition") {
        return Preposition;
    } else if (word == "VerbPhrase") {
        return VerbPhrase;
    } else if (word == "VerbPhrasePrime") {
        return VerbPhrasePrime;
    } else {
        return ErrorNonTerm;
    }
}

bool isNonTerminal(char ch){
    if((ch >= 'A' && ch <= 'Z')){        
        return true;
    }else{
        return false;
    }
}

string convertStackToString(vector<string> stack){
    string result = "";
    for(int i=0; i<stack.size(); ++i){
        result = result + " " + stack[i];
    }
    return result;
}

void processTokens(vector<string> &stack, vector<string> &input, string PPT[][COLUMNS]){
    vector <string> stackTrace;
    vector <string> output;
    int errors = 0;

    stackTrace.push_back(convertStackToString(stack));

    string lastRule = stack.back();
    stack.pop_back();

    int currentInputIndex = 0;
    string currentInput = input[currentInputIndex];

    cout << "----------------------" << endl;
    cout << "Output" << endl;
    cout << "----------------------" << endl;

    while(lastRule != "$"){        
        if(isNonTerminal(lastRule[0])){
            string rules = PPT[getNonTerminal(lastRule)][getTerminal(currentInput)];

            stringstream lineAsStream(rules);
            string rule;

            cout << lastRule << " --> " << rules << endl;

            if(rules == "pop"){
                lastRule = stack.back();
                stack.pop_back();
                ++errors;
                continue;
            } else if (rules == "scan"){
                ++errors;
                currentInputIndex++;
                currentInput = input[currentInputIndex];

                if(currentInput == "$"){
                    break;
                }

                continue;
            }

            vector<string> newRules;
            while (getline(lineAsStream, rule, ' ')) {
                newRules.push_back(rule);            
            }

            for (int j=newRules.size() - 1; j>= 0; --j){
                stack.push_back(newRules[j]);
            }            
        } else {
            if(currentInput == lastRule){
                cout << "Token Matched. " << currentInput << " <=> " << lastRule << endl;
                
                currentInputIndex++;
                currentInput = input[currentInputIndex];
            }
        }

        stackTrace.push_back(convertStackToString(stack));
        lastRule = stack.back();
        stack.pop_back();
    }

    cout << "----------------------" << endl;
    cout << "Stack Trace" << endl;
    cout << "----------------------" << endl;
    printVector(stackTrace, true);

    cout << "----------------------" << endl;
    cout << "Errors = " << errors << endl;
    cout << "----------------------" << endl;
}

int main() {
    string csvfilename = "TableInCSV.csv";

    string tokens = "tokens.txt";

    string TT[ROWS][COLUMNS];
    createTT(csvfilename, TT);

    vector<string> input;
    vector<string> stack;
    stack.push_back("$");
    stack.push_back("Sentence");

    readTokens(stack, input, tokens);
    processTokens(stack, input, TT);
}