#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

# define BUFFER_LENGTH 4096
# define ROWS 61
# define COLUMNS 130

class Buffer{
    public:
        char buff1[BUFFER_LENGTH];
        char buff2[BUFFER_LENGTH];
        char* currentBuffer = buff1;

        int currentChar = 0;
        bool retracting = false;
        char prevCharacter;

        ifstream file;

        Buffer(string fileName){
            file.open(fileName);
            readInBuffer();
        }

        void readInBuffer(){
            switchBuffer();
            file.read(currentBuffer, BUFFER_LENGTH);
            if(file.eof() && file.gcount() < BUFFER_LENGTH){
                currentBuffer[file.gcount()] = '\0';
            }
        }

        void switchBuffer(){
            currentBuffer = currentBuffer == buff1 ? buff2 : buff1;
            currentChar = 0;
        }

        char getCharFromBuffer(){
            if (currentChar == BUFFER_LENGTH){
                readInBuffer();                
            }

            if(retracting){
                retracting = false;
                return prevCharacter;
            }

            char returnChar = currentBuffer[currentChar];
            prevCharacter = returnChar;

            currentChar++;
            if(returnChar == '\0'){
                return EOF;
            }else{
                return returnChar;
            }
        }

        void retract(){
            retracting = true;
        }

        ~Buffer(){
            file.close();
        }
};

string generateToken(char tokenArr[], string type){
    if(type == "COMMENT"){
        return "";
    }

    const int reserveWordsLength = 14;

    string castToken = string(tokenArr);
    if(type == "RESERVE WORD"){
        string reserveWord[reserveWordsLength] = {"CONST" , "FLOAT" , "INT" , "BREAK" , "CONTINUE" , "ELSE" , "FOR" , "SWITCH" , "VOID" , "CASE" , "CHAR" , "DO" , "IF" , "WHILE"};

        bool reserverWord = false;
        for (int i = 0; i < reserveWordsLength; i++) {
            if(castToken == reserveWord[i]){
                reserverWord = true;
                break;    
            }
        }

        if(reserverWord){
            type = "RESERVE WORD";
        }else {
            type = "WORD";
        }

    }
    string token = "<" + string(tokenArr) + ", " + type + ">";
    cout << token << endl;
    return token;
}


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

int main() {
    string csvfilename = "TT in csv.csv";

    string outputFileName = "token.txt";
    ofstream outputFile(outputFileName);

    string TT[ROWS][COLUMNS];
    createTT(csvfilename, TT);

    int state = 0, li = 0;
    char lex[1024];
    
    Buffer buff = Buffer("code.txt");
    char character;

    bool retract = false;
    
    while((character = buff.getCharFromBuffer()) != EOF){
        if(state == -1){
            state = 0;
            li = 0;
        }

        // 127 is accepting state index
        if(!stoi(TT[state][127])){
            state = stoi(TT[state][int(character)]);
            lex[li] = character;
            li++;
        }else{
            // 128 is retracting state index
            if(stoi(TT[state][128])){
                lex[li-1] = '\0';
                buff.retract();
            } else {
                lex[li] = '\0';
            }

            li = 0;

            // 129 is token state index
            string token = generateToken(lex, TT[state][129]);
            if(token != ""){
                outputFile << token << endl;
            }

            state = 0;
        }
    }

    outputFile.close();
}