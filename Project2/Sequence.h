//
// Created by cover on 1/20/2021.
//

#ifndef PROJECT2_SEQUENCE_H
#define PROJECT2_SEQUENCE_H

#define DEVELOPING

#include <string>

using ItemType = std::string;

class Sequence {
public:
    Sequence();
    ~Sequence();
    Sequence(const Sequence &sequence);
    Sequence &operator=(const Sequence &sequence);
    bool empty() const;  // tested
    int size() const;  // tested
    int insert(int pos, const ItemType &value);  // tested
    int insert(const ItemType &value);
    bool erase(int pos);  // tested
    int remove(const ItemType &value);
    bool get(int pos, ItemType &value) const;  // tested
    bool set(int pos, const ItemType &value);  // tested
    int find(const ItemType &value) const;
    void swap(Sequence &other);

private:
    class Node;

    Node *first() const;
    Node *last() const;
    Node *get_node(int pos) const;
    void report_null_prt_err() const;

    bool loop_complete();
    Node *m_head;
    int m_current_size;


    class Node {
    public:
        Node(); // Used to create a starter node that refers to itself
        Node(const ItemType &val, Node *next, Node *prev);
        void setNodeVal(const ItemType &val);
        ItemType getNodeVal() const;

        void setNextNode(Node *next);
        void setPrevNode(Node *prev);

        Node *getNextNode();
        Node *getPrevNode();
    private:
        ItemType m_val;
        Node *m_next;
        Node *m_prev;
    };
};


#endif //PROJECT2_SEQUENCE_H
