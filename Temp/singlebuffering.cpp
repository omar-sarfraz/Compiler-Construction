#include <iostream>
#include <fstream>

using namespace std;

int main(){
    char messageVar[4096];

    ifstream file("test.txt");

    while (!file.eof()) {
        cout << "Reading Buffer" << endl;
        file.getline(messageVar, 4096);
        for(int i=0; i < 4096; i++){
            if(!messageVar[i]){
                break;
            }
            cout << messageVar[i] << endl;
        }
    
    }
}