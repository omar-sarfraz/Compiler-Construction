#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

# define BUFFER_LENGTH 4096
# define ROWS 9
# define COLUMNS 7

enum COL {L=0, E, G, N, A, R, T, INVALID};

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

void generateToken(char tokenArr[], string type){
    string token = "<" + string(tokenArr) + ", " + type + ">";
    cout << token << endl;
}

COL getLetter(char character){
    if (character == '<'){
        return L;
    } else if (character == '>'){
        return G;
    } else if (character == '='){
        return E;
    } else if (character == '!'){
        return N;
    } else {
        return INVALID;
    }
}

int main() {

    string TT[ROWS][COLUMNS] = {
        {"1", "0", "4", "7", "0", "0", "N"},
        {"3", "2", "3", "3", "0", "0", "N"},
        {"0", "0", "0", "0", "1", "0", "LE OPERATOR"},
        {"0", "0", "0", "0", "1", "1", "L OPERATOR"},
        {"6", "5", "6", "6", "0", "0", "N"},
        {"0", "0", "0", "0", "1", "0", "GE OPERATOR"},
        {"0", "0", "0", "0", "1", "1", "GOPERATOR"},
        {"0", "8", "0", "0", "0", "0", "N"},
        {"0", "0", "0", "0", "1", "0", "NE OPERATOR"}
    };

    int state = 0, li = 0;
    char lex[1024];
    
    Buffer buff = Buffer("code.txt");
    char character;
    
    while((character = buff.getCharFromBuffer()) != EOF){
        // if(state == -1){
        //     state = 0;
        //     li = 0;
        // }

        if(!stoi(TT[state][A])){
            int column = getLetter(character);
            
            state = stoi(TT[state][column]);

            if(column == 7){
                continue;
            }

            lex[li] = character;
            li++;
        }else{
            if(stoi(TT[state][R])){
                lex[li-1] = '\0';
                buff.retract();
            } else {
                lex[li] = '\0';
            }

            li = 0;
            
            generateToken(lex, TT[state][T]);

            state = 0;
        }
    }
}