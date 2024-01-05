#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct Node {
    string data;
    Node* next;
};

void insert(Node*& head, string value) {
    Node* newNode = new Node{value, nullptr};

    if (head == nullptr) {
        head = newNode;
        return;
    }

    Node* current = head;
    while (current->next != nullptr) {
        current = current->next;
    }

    current->next = newNode;
}


void display(Node* head) {
    Node* current = head;
    while (current != nullptr) {
        cout << current->data << " -> ";
        current = current->next;
    }
    cout << "nullptr" << endl;
}

void checkLR(string rules){
    stringstream lineAsStream(rules);
    
    string head;
    getline(lineAsStream, head, '=');

    string rule;
    while(getline(lineAsStream, rule, '|')){
        if (head[0] == rule[0]){
            // cout << endl;
            cout << rules << " is LEFT RECURSIVE" << endl;
        } else {
            cout << rules << " is not LEFT RECURSIVE" << endl;
        }
        return;
    }
}

void checkList(Node* head) {
    Node* current = head;
    while (current != nullptr) {
        // cout << current->data << " -> ";
        checkLR(current->data);
        current = current->next;
    }
}

int main() {

    ifstream file;
    file.open("rules.txt");

    string line;
    Node* myList = nullptr;

    while (!file.eof()){        
        getline(file, line);

        insert(myList, line);
        // cout << line << endl;
    }

    // cout << "Linked List: ";
    // display(myList);

    checkList(myList);

    return 0;
}