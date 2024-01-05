/*
    Muhammad Omar Sarfraz
    FA20-BCS-038

    The program prints out the domain names, top level domains, country level domain and protocols from the url.
    You can run using the attached sample txt file.

    We are assuming the following protocols: https, http, www | PROTOCOL
    We are assuming the following country domains: pk | CTLD
    We are assuming the following top level domains: org, com, edu | TLD
*/

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

            // cout << "Prev is " << prevCharacter << endl;
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

// Domain and Protocol type seperation
void verifyDomain(char tokenArr[]){
    // cout << tokenArr << "<=>" << "pk: " << (string(tokenArr) == "pk") << endl;
    string castToken = string(tokenArr);
    if(castToken == "pk"){
        cout << '<' << tokenArr << ", " << "CTLD" << '>' << endl;
    }else if(castToken == "com" || castToken == "org" || castToken == "edu"){
        cout << '<' << tokenArr << ", " << "TLD" << '>' << endl;
    }else if(castToken == "www" || castToken == "http" || castToken == "https"){
        cout << '<' << tokenArr << ", " << "PROTOCOL" << '>' << endl;
    }else{
        cout << '<' << tokenArr << ", " << "DOMAIN NAME" << '>' << endl;
    }
}

void generateToken(char tokenArr[], string type){
    cout << '<' << tokenArr << ", " << type << '>' << endl;
}

int main(){
    int state = 0, li = 0;
    char lex[1024];
    
    Buffer buff = Buffer("labquiz1.txt");
    char character;
    
    while((character = buff.getCharFromBuffer()) != EOF){
        // cout << character << " " << endl;
        switch (state)
        {
        case 0:
            if(isLetter(character)){
                state = 1;
                lex[li] = character;
                li++;
            }else if(character == '.'){
                // cout << "Going to 3" << endl;
                state = 3;
                lex[li] = character;
                li++;
            }else if(character == '/'){
                state = 4;
                lex[li] = character;
                li++;
            }else if(character == ':'){
                // cout << "Going to 3" << endl;
                state = 6;
                lex[li] = character;
                li++;
            }
            break;

        case 1:
            if(isLetter(character)){
                state = 1;
                lex[li] = character;
                li++;
            }else{
                state = 2;
                buff.retract();
            }
            break;

        case 2:
            state = 0;
            lex[li] = '\0';
            li = 0;

            // cout << "Generated ID, Current char " << character << endl;
            verifyDomain(lex);
            buff.retract();
            break;

        case 3:
            state = 0;
            lex[li] = '\0';
            li = 0;

            // cout << "Generated PUNC, Current char " << character << endl;

            generateToken(lex, "PUNC");
            buff.retract();
            break;

        case 4:
            if(character == '/'){
                state = 5;
                lex[li] = character;
                li++;
            }
            break;

        case 5:
            state = 0;
            lex[li] = '\0';
            li = 0;

            generateToken(lex, "DSLASH");
            buff.retract();
            break;

        case 6:
            state = 0;
            lex[li] = '\0';
            li = 0;

            generateToken(lex, "PUNC");
            buff.retract();
            break;
        }
    }

}