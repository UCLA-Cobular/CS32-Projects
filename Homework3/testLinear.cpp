//
// Created by jdc10 on 2/3/2021.
//

#include <cassert>
#include <iostream>
#include "linear.cpp"

int main() {
    {
        double arr[10] = {0.1, 1.0, -15, 34, -10, -15, 30, -0.111, -0.000001, 4};
        double arr2[10] = {0.1, 1.0, 15, 34, 10, 15, 30, 0.111, 0.000001, 4};
        double arr3[10] = {0.1, 1.0, 15, 34, 10, 15, 30, 0.111, 0.000001, -4};
        double arr4[10] = {-0.1, 1.0, 15, 34, 10, 15, 30, 0.111, 0.000001, 4};

        assert(!anyTrue(arr, -1));
        assert(!anyTrue(arr, 0));
        assert(anyTrue(arr, 10));
//        std::cout << std::endl << std::endl;
        assert(!anyTrue(arr2, 10));
//        std::cout << std::endl << std::endl;
        assert(anyTrue(arr3, 10));
//        std::cout << std::endl << std::endl;
        assert(anyTrue(arr4, 10));
//        std::cout << std::endl << std::endl;
    }
    {
        double arr[10] = {0.1, 1.0, -15, 34, -10, -15, 30, -0.111, -0.000001, 4};
        double arr2[10] = {0.1, 1.0, 15, 34, 10, 15, 30, 0.111, 0.000001, 4};
        double arr3[10] = {0.1, 1.0, 15, 34, 10, 15, 30, 0.111, 0.000001, -4};
        double arr4[10] = {-0.1, 1.0, 15, 34, 10, 15, 30, 0.111, 0.000001, 4};

        assert(countTrue(arr, -1) == 0);
        assert(countTrue(arr, 0) == 0);
        assert(countTrue(arr, 10) == 5);
        assert(countTrue(arr, 10) == 5);
//        std::cout << std::endl << std::endl;
        assert(countTrue(arr2, 10) == 0);
//        std::cout << std::endl << std::endl;
        assert(countTrue(arr3, 10) == 1);
//        std::cout << std::endl << std::endl;
        assert(countTrue(arr4, 10) == 1);
//        std::cout << std::endl << std::endl;
    }
    {
        double arr10[10] = {0.1, 1.0, -15, 34, -10, -15, 30, -0.111, -0.000001, 4};
        double arr11[11] = {0.1, 1.0, 15, 34, 10, 15, 30, 0.111, 0.000001, 4, 66};
        double arr3[10] = {100, 1.0, 15, 34, 10, 15, 30, 0.111, 0.000001, -4};
        double arr4[10] = {-0.1};
        double arr5[10] = {-0.1, 1.0};

        assert(positionOfMax(arr10, -1) == -1);
        assert(positionOfMax(arr10, 0) == -1);
        assert(positionOfMax(arr10, 10) == 3);
//        std::cout << std::endl << std::endl;
        assert(positionOfMax(arr11, 11) == 10);
//        std::cout << std::endl << std::endl;
        assert(positionOfMax(arr3, 10) == 0);
//        std::cout << std::endl << std::endl;
        assert(positionOfMax(arr4, 1) == 0);
//        std::cout << std::endl << std::endl;
        assert(positionOfMax(arr5, 2) == 1);
//        std::cout << std::endl << std::endl;
    }
    {
        double arr1[10] = {0.1, 1.0, -15, 34, -10, -15, 30, -0.111, -0.000001, 4};
        double arr1in1[11] = {0.1, 1.0, -15};
        double arr1in2[10] = {0.1, 34, -0.111};
        double arr1in3[10] = {0.1, 34, 4};
        double arr1out1[10] = {1.0, 34, 5};
        double arr1out2[10] = {444};
        double arr1out3[10] = {4, 30, 34};

        assert(isIn(arr1in1, 3, arr1, 10));
        assert(isIn(arr1in1, 0, arr1, 10));
        assert(isIn(arr1in2, 3, arr1, 10));
        assert(isIn(arr1in3, 3, arr1, 10));
        assert(!isIn(arr1out1, 3, arr1, 10));
        assert(!isIn(arr1out2, 1, arr1, 10));
        assert(!isIn(arr1out3, 3, arr1, 10));
    }


    std::cout << "tests fin" << std::endl;
}
