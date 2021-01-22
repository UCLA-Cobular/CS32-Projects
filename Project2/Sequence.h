//
// Created by cover on 1/20/2021.
//

#ifndef PROJECT2_SEQUENCE_H
#define PROJECT2_SEQUENCE_H


using ItemType = unsigned long;

class Sequence {
public:
    Sequence();
    ~Sequence();
    Sequence(const Sequence &sequence);
    Sequence &operator=(const Sequence &sequence);
    bool empty() const;
    int size() const;
    int insert(int pos, const ItemType &value);
    int insert(const ItemType &value);
    bool erase(int pos);
    int remove(const ItemType &value);
    bool get(int pos, ItemType &value) const;
    bool set(int pos, const ItemType &value);
    int find(const ItemType &value) const;
    void swap(Sequence &other);

private:
    class Node;

    Node *first() const;
    Node *get_last();
    Node *get_node(int pos);

    bool loop_complete();
    Node *m_head;
    int m_current_size;


    class Node {
    public:
        Node(); // Used to create a starter node that refers to itself
        Node(const ItemType &val, Node *prev, Node *next);
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
