//
// Created by jdc10 on 2/23/2021.
//

#include <iostream>
#include <cassert>

using namespace std;

struct Node {
    int data;
    Node *child[3];
};

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE
// COMMENT AT THE END OF THE countLessers FUNCTION

int countLessers(const Node *p) {
    // Check if the node itself is null or if it's children are all null, and return 0
    // I'm aware the three checks aren't strictly necessary as the same function is accomplished by the for loop below,
    //  but I'm leaving them here for clarity and readability, as it makes sense to check them as an escape case first.
    if (p == nullptr || (p->child[0] == nullptr && p->child[1] == nullptr && p->child[2] == nullptr)) return 0;

    // Sum up countLessers of the children nodes of this node and check if this node itself is a lesser at the same time
    int numLesserNodes = 0;
    bool isLesser = false;
    for (const Node *childNode : p->child) {
        if (childNode != nullptr) {
            numLesserNodes += countLessers(childNode);
            if (p->data < childNode->data) isLesser = true;
        }
    }
    if (isLesser) numLesserNodes++;
    return numLesserNodes;
}

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE
// COMMENT AT THE START OF THE countLessers FUNCTION

Node *makeNode(int d) {
    Node *p = new Node;
    p->data = d;
    for (int k = 0; k < 3; k++)
        p->child[k] = nullptr;
    return p;
}

int main() {
    // Build the 3-tree pictured in the problem as an example

    Node *n8 = makeNode(8);
    n8->child[2] = makeNode(3);

    Node *n2 = makeNode(2);
    n2->child[1] = n8;

    Node *n5 = makeNode(5);
    n5->child[0] = makeNode(8);
    n5->child[2] = makeNode(9);

    Node *n7 = makeNode(7);
    n7->child[1] = n5;

    Node *root = makeNode(4);
    root->child[0] = n2;
    root->child[1] = makeNode(1);
    root->child[2] = n7;

    // Test countLessers with that tree

    assert(countLessers(root) == 3);
    assert(countLessers(root->child[2]) == 1);
    assert(countLessers(nullptr) == 0);
    assert(countLessers(root->child[1]) == 0);
    cout << "All tests succeeded" << endl;

    // Clean up the tree

    delete n8->child[2];
    delete n8;
    delete n2;
    delete root->child[1];
    delete n5->child[0];
    delete n5->child[2];
    delete n5;
    delete n7;
    delete root;
}
