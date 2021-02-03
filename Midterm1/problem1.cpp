#include <iostream>

class Num
{
public:
    Num(int i) : m_i(i) {}
    // no other constructors are declared
    int geti() const { return m_i; }
//    … other members and implementation details are
//            irrelevant to this problem …
private:
    int m_i;
};


class Blah
{
public:
    Blah(int i);
    ~Blah();
    Blah(const Blah& other);
//    … other public members are not shown here …
private:
    Num* m_num;
    int m_ii;
    int* m_eyes;
};

Blah::Blah(int i)
{
    m_num = new Num(i);
    m_ii = 2 * i;
    m_eyes = &m_ii;
}

/////////////////////////////////////////////////////////

/// Spec - The copy will access a **copy** of the num object
/// m_num must be a dynamically allocated Num
/// m_eyes must point to that blah's m_ii
/// @param other
Blah::Blah(const Blah &other) {
    m_num = new Num(*other.m_num);
    m_ii = other.m_ii;
    m_eyes = &m_ii;
}

Blah::~Blah() {
    delete m_num;
}

//////////////////////////////////////////////////////////

// must not do anything undefined
void f()
{
    Blah b1(3);
    Blah b2(b1);
    std::cout << "fin" << std:: endl;
}

int main() {
    f();
}
