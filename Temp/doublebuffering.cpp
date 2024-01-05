#include <iostream>
#include <fstream>

using namespace std;

int main(){
    char buff1[4096];
    char buff2[4096];

    ifstream file("test.txt");

    int numBuf = 1;

    while (!file.eof()) {
        if(numBuf == 1){
            cout << "Reading " << numBuf << endl;
            file.getline(buff1, 4096);            
            cout << buff1 << endl;
            numBuf = 2;
        }else{
            cout << "Reading " << numBuf << endl;
            file.getline(buff2, 4096);            
            cout << buff2 << endl;
            numBuf = 1;
        }
    }
}