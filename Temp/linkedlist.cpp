#include <iostream>

// Define a struct for a node in the linked list
struct Node {
    int data;
    Node* next;
};

int main() {
    // Create nodes for the linked list
    Node* head = nullptr;
    Node* second = nullptr;
    Node* third = nullptr;

    // Allocate memory for nodes
    head = new Node();
    second = new Node();
    third = new Node();

    // Assign data to nodes
    head->data = 1;
    second->data = 2;
    third->data = 3;

    // Link the nodes
    head->next = second;
    second->next = third;
    third->next = nullptr;

    // Traverse and print the linked list
    Node* current = head;
    while (current != nullptr) {
        std::cout << current->data << " -> ";
        current = current->next;
    }
    std::cout << "nullptr" << std::endl;

    // Don't forget to deallocate memory to prevent memory leaks
    delete head;
    delete second;
    delete third;

    return 0;
}
