//
// Created by jdc10 on 1/9/2021.
//

#ifndef PROJECT1_FLATULAN_H
#define PROJECT1_FLATULAN_H


class City;  // This is needed to let the compiler know that City is a
// type name, since it's mentioned in the Flatulan declaration.

class Flatulan
{
public:
    // Constructor
    Flatulan(City* cp, int r, int c);

    // Accessors
    int  row() const;
    int  col() const;

    // Mutators
    void move();
    bool possiblyGetConverted();

private:
    City* m_city;
    int   m_row;
    int   m_col;
};

#endif //PROJECT1_FLATULAN_H
