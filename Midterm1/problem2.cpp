#include <iostream>
#include <cassert>
#include <string>

using namespace std;

class StringList {
public:
    StringList();
    // You may declare other member functions for your test code to call
    void insertAtFront(char value);
    void eraseEvens();
    string makeString();
private:
    struct Node {
        char value;
        Node *next;
    };
    Node *head;  // points to first Node in the list
};

StringList::StringList() {
    head = nullptr;
}


// You may implement other member functions here for test purposes,
// but you will not turn them in as part of your submission.


void StringList::insertAtFront(char value) {
    // setup next newNode
    Node *newNode = new Node;
    newNode->value = value;
    newNode->next = nullptr;
    // If head is nullptr, special case
    if (head == nullptr) {
        head = newNode;
        return;
    }

    // Else, head is a node
    Node *nextNode = head;
    // loop over nodes until nextNode == last node
    while (nextNode->next != nullptr) {
        nextNode = nextNode->next;
    }
    // nextNode is the final node
    nextNode->next = newNode;
}


string StringList::makeString() {
    string string1;
    // Else, head is a node
    Node *currentNode = head;
    // loop over nodes until currentNode == last node
    while (currentNode != nullptr) {
        string1 += currentNode->value;
        currentNode = currentNode->next;
    }
    return string1;
}

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE
// COMMENT AFTER THE END OF THE eraseEvens IMPLEMENTATION

void StringList::eraseEvens() {
    // Case with zero or one nodes
    if (head == nullptr || head->next == nullptr) return;

    // At least 2 items in list, need to do some deletions
    bool even = true;
    Node *currentNode = head->next;  // Init currentNode to the first node in the list
    Node *prevNode = head;

    // Loop over every node in the list, starting on the second node (the first one to delete)
    while (currentNode != nullptr) {
        if (even) {
            // erase the node, then go on to the next node
            Node *nextNode = currentNode->next;
            delete currentNode;
            prevNode->next = nextNode;
            prevNode = currentNode;
            currentNode = nextNode;
        } else {
            // go on to next node
            prevNode = currentNode;
            currentNode = currentNode->next;
        }
        // always flip even
        even = !even;
    }

}


// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE
// COMMENT BEFORE THE START OF THE eraseEvens IMPLEMENTATION

int main() {
    // You may put some code here for test purposes
    {
        StringList stringList = StringList();


        stringList.insertAtFront('a');
        stringList.insertAtFront('b');
        stringList.insertAtFront('c');
        stringList.insertAtFront('d');
        stringList.insertAtFront('e');
        assert(stringList.makeString() == "abcde");
        stringList.eraseEvens();
        assert(stringList.makeString() == "ace");
        stringList.eraseEvens();
        assert(stringList.makeString() == "ae");
        stringList.eraseEvens();
        assert(stringList.makeString() == "a");
        stringList.eraseEvens();
        assert(stringList.makeString() == "a");

    }
    {
        StringList stringList = StringList();
        stringList.eraseEvens();
        assert(stringList.makeString().empty());
    }
    {
        StringList stringList = StringList();

        stringList.insertAtFront('a');
        stringList.insertAtFront('b');
        stringList.insertAtFront('c');
        stringList.insertAtFront('d');
        assert(stringList.makeString() == "abcd");
        stringList.eraseEvens();
        assert(stringList.makeString() == "ac");
        stringList.eraseEvens();
        assert(stringList.makeString() == "a");
        stringList.eraseEvens();
        assert(stringList.makeString() == "a");
    }
    cout << "All tests succeeded" << endl;
}


//// Get length of StringList
//Node *nextNode = head;
//int length = 1;
//// loop over nodes until nextNode == last node
//while (nextNode->next != nullptr) {
//length++;
//nextNode = nextNode->next;
//}
//
//// Case where list is less than 2
