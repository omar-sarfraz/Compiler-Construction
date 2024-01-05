#include <iostream>
#include <fstream>

using namespace std;

#define BUFFER_LENGTH 4

class Buffer{
    public:
        char buff1[BUFFER_LENGTH];
        char buff2[BUFFER_LENGTH];
        char* currentBuffer = buff1;

        // int currentBufferNo = 1;
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
            // cout << "EOF" << file.eof() << endl;
            if(file.eof() && file.gcount() < BUFFER_LENGTH){
                currentBuffer[file.gcount()] = '\0';
            }
        }

        void switchBuffer(){
            // cout << "Buffer " << (currentBuffer == buff1 ? "1" : "2") << endl;
            currentBuffer = currentBuffer == buff1 ? buff2 : buff1;
            currentChar = 0;
        }

        char getCharFromBuffer(){
            if (currentChar == BUFFER_LENGTH){
                readInBuffer();                
            }

            if(retracting){
                // cout << "Retracting " << currentChar << " " << prevCharacter << endl;
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

bool isLetter(char ch){
    if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')){
        // cout << ch << " is letter" << endl;
        return true;
    }else{
        return false;
    }
}

bool isDigit(char num){
    num = int(num);
    if(num >= '0' && num <= '9'){
        // cout << num << " is number" << endl;
        return true;
    }else{
        return false;
    }
}

void generateToken(char tokenArr[], string type){
    cout << '<' << tokenArr << ", " << type << '>' << endl;
}

int main(){
    int state = 0, li = 0;
    char lex[1024];
    
    Buffer buff = Buffer("test2.txt");
    char character;
    
    while((character = buff.getCharFromBuffer()) != EOF){
        // cout << character << " " << endl;
        switch (state)
        {
        case 0:
            if(isLetter(character) || character == '_'){
                state = 1;
                lex[li] = character;
                li++;
            }else if(character == '/'){
                state = 3;
            }
            break;

        case 1:
            if(isLetter(character) || isDigit(character) || character == '_'){
                state = 1;
                lex[li] = character;
                li++;
                
                //Breaking loop after state is 2 will read 2 additional characters
                break;
            }else{
                // cout << "Ending at " << character << endl;
                state = 2;
            }

        case 2:
            lex[li] = '\0';
            state = 0;
            li = 0;
            
            // cout << "Retract char " << character << endl;
            // cout << lex << endl;
            buff.retract();
            generateToken(lex, "IDENTIFIER");
            break;

        case 3:
            if(character == '/'){
                state = 4;
            }else if(character == '*'){
                state = 5;
            }
            break;

        case 4:
            if(character == '\n'){
                state = 6;
            }else{
                state = 4;
            }
            break;

        case 6:
            state = 0;
            buff.retract();
            break;

        case 5:
            if(character != '*'){
                state = 5;
            }else{
                state = 7;
            }
            break;

        case 7:
            if(character == '*'){
                state = 7;
            } else if (character == '/'){
                state = 8;
            } else {
                state = 10;
            }
            break;

        case 8:
            state = 0;
            break;

        // default:
        //     cout << "Unexpected Character " << character << endl;
        //     break;
        }
    }

}