#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

# define ROWS 25
# define COLUMNS 21

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
    Identifier, LB, RB, Comma, Semicolon, Int, Float, While, If,
    ElseT, LCB, RCB, LT, GT, Equal, Plus, Minus, MUL, Div, Number,
    End_Marker, Empty, ErrorTerm
};

Terminals getTerminal(string word) {
    if (word == "identifier") {
        return Identifier;
    } else if (word == "(") {
        return LB;
    } else if (word == ")") {
        return RB;
    } else if (word == ",") {
        return Comma;
    } else if (word == ";") {
        return Semicolon;
    } else if (word == "int") {
        return Int;
    } else if (word == "float") {
        return Float;
    } else if (word == "while") {
        return While;
    } else if (word == "if") {
        return If;
    } else if (word == "else") {
        return ElseT;
    } else if (word == "{") {
        return LCB;
    } else if (word == "}") {
        return RCB;
    } else if (word == "<") {
        return LT;
    } else if (word == ">") {
        return GT;
    } else if (word == "=") {
        return Equal;
    } else if (word == "+") {
        return Plus;
    } else if (word == "-") {
        return Minus;
    } else if (word == "*") {
        return MUL;
    } else if (word == "/") {
        return Div;
    } else if (word == "number") {
        return Number;
    } else if (word == "$") {
        return End_Marker;
    } else if (word == "#") {
        return Empty;
    } else {
        return ErrorTerm;
    }
}

enum NonTerminals {
    Function, ArgList, ArgListPrime, Arg, Stmt, Declaration,
    Type, WhileStmt, IfStmt, Else, ElsePart, CompoundStmt, CompoundStmtPrime,
    StmtList, StmtListPrime, BoolExpr, Compare, ComparePrime, CompareDoublePrime, 
    Expr, Mag, MagPrime, Term, TermPrime, Factor, ErrorNonTerm
};

NonTerminals getNonTerminal(string word) {
    if (word == "Function") {
        return Function;
    } else if (word == "ArgList") {
        return ArgList;
    } else if (word == "ArgListPrime") {
        return ArgListPrime;
    } else if (word == "Arg") {
        return Arg;
    } else if (word == "Stmt") {
        return Stmt;
    } else if (word == "Declaration") {
        return Declaration;
    } else if (word == "Type") {
        return Type;
    } else if (word == "WhileStmt") {
        return WhileStmt;
    } else if (word == "IfStmt") {
        return IfStmt;
    } else if (word == "Else") {
        return Else;
    } else if (word == "ElsePart") {
        return ElsePart;
    } else if (word == "CompoundStmt") {
        return CompoundStmt;
    } else if (word == "CompoundStmtPrime") {
        return CompoundStmtPrime;
    } else if (word == "StmtList") {
        return StmtList;
    } else if (word == "StmtListPrime") {
        return StmtListPrime;
    } else if (word == "BoolExpr") {
        return BoolExpr;
    } else if (word == "Compare") {
        return Compare;
    } else if (word == "ComparePrime") {
        return ComparePrime;
    } else if (word == "CompareDoublePrime") {
        return CompareDoublePrime;
    } else if (word == "Expr") {
        return Expr;
    } else if (word == "Mag") {
        return Mag;
    } else if (word == "MagPrime") {
        return MagPrime;
    } else if (word == "Term") {
        return Term;
    } else if (word == "TermPrime") {
        return TermPrime;
    } else if (word == "Factor") {
        return Factor;
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
    string csvfilename = "PPTinCSV.csv";

    // Use tokens.txt for Function example. Use dec.txt for error example.

    string tokens = "tokens.txt";

    string TT[ROWS][COLUMNS];
    createTT(csvfilename, TT);

    vector<string> input;
    vector<string> stack;
    stack.push_back("$");
    stack.push_back("Function");

    readTokens(stack, input, tokens);
    processTokens(stack, input, TT);
}