//
// Created by jdc10 on 2/6/2021.
//coun

#include "tree.cpp"
#include <cassert>

using namespace std;

int main() {
    {
        double list[] = {10, 30, 40, 20};
        double a1[] = {10, 40, 20};
        double a2[] = {10, 30, 40};
        double a3[] = {10, 10, 40};
        double a4[] = {50, 40, 30};

//        assert(countIsIn(a1, 3, list, 4) == 1);
        assert(countIsIn(a2, 3, list, 4) == 1);
//        assert(countIsIn(a1, 3, list, 7) == 1);
//        assert(countIsIn(a1, 3, list, 7) == 1);
    }
    {
        double list[] = {10, 50, 40, 20, 50, 40, 30};
        double a1[] = {10, 20, 40};
        double a2[] = {10, 40, 30};
        double a3[] = {10, 10, 40};
        double a4[] = {50, 40, 30};

//        assert(countIsIn(a1, 3, list, 7) == 1);
//        assert(countIsIn(a2, 3, list, 7) == 2);
//        assert(countIsIn(a1, 3, list, 7) == 1);
//        assert(countIsIn(a1, 3, list, 7) == 1);
    }
//    {
//        double list[] = {10, 50, 40, 20, 50, 40, 30};
//        order(list, 7);
//    }
//    {
//        double list[] = {50, 40, 30};
//        order(list, 3);
//        cout << list << endl;
//    }
//    {
//        double list[] = {10, 40, 30};
//        order(list, 3);
//        cout << list << endl;
//    }
//    {
//        double list[] = {10, 10, 40};
//        order(list, 3);
//        cout << list << endl;
//    }
//    {
//        double list[] = {50, 40, 30};
//        order(list, 3);
//        cout << list << endl;
//    }
}
