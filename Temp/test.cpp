#include <iostream>
#include <string>
#include <sys/stat.h>
#include <windows.h>

#include <filesystem>

using namespace std;

struct Node {
    int data;
    Node* next;
};

void insert(Node*& head, int value) {
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

int main() {
    Node* myList = nullptr;

    insert(myList, 10);
    insert(myList, 20);
    insert(myList, 30);
    insert(myList, 40);

    cout << "Linked List: ";
    display(myList);

    return 0;
}
