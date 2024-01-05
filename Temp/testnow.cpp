#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <cctype>

using namespace std;

void switchh(char currentChar, int& state, int& li, char lex[]) {
    switch (state) {
    case 0:
        if (isalpha(currentChar)) {
            state = 1;
            lex[li] = currentChar;
            li++;
        }
        if (currentChar == ':'||currentChar == '/') {
            state = 0;
            li = 0;
        }
        break;
    case 1:
        if (currentChar == ':'||currentChar == '/') {
            state = 0;
            li = 0;
            break;
        }
        else{
                 if (isalnum(currentChar) || currentChar == '.' || isalpha(currentChar)) {
                    lex[li++] = currentChar;
                    }
                 else{
                        state=2;
                    }

        }

        break;
    case 2:
        lex[li] = '\0';
        cout << "Found domain name: " << lex << endl;
        state = 0;
        li = 0;
        break;
    }
}

int main() {
    char buffer1[4096];
    char buffer2[4096];
    char lex[1024];
    int li = 0;
    int state = 0;
    char* currentBuffer = buffer1;

    ifstream inputFile("labquiz1.txt");

    if (!inputFile.is_open()) {
        cerr << "Error opening the file." << endl;
        return 1;
    }

    while (!inputFile.eof()) {
        inputFile.read(currentBuffer, 4096);
        int bytesRead = inputFile.gcount();

        for (int i = 0; i < bytesRead; i++) {
            char currentChar = currentBuffer[i];
            switchh(currentChar, state, li, lex);
        }

        if (currentBuffer == buffer1) {
            currentBuffer = buffer2;
        } else {
            currentBuffer = buffer1;
        }
    }

    inputFile.close();

    return 0;
}
