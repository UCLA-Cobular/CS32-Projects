// Return true if the somePredicate function returns true for at
// least one of the array elements, false otherwise.
bool anyTrue(const double a[], int n) {
    if (n <= 0) return false;
    return somePredicate(a[n - 1]) || anyTrue(a, n - 1);
}

// Return the number of elements in the array for which the
// somePredicate function returns true.
// 0 is falsy, 1 is truthy
int countTrue(const double a[], int n) {
    if (n <= 0) return 0;
    if (n == 1) return somePredicate(a[n - 1]);
    return somePredicate(a[n - 1]) + countTrue(a, n - 1);
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns true.  If there is no such
// element, return -1.
int firstTrue(const double a[], int n) {
    if (n <= 0) return -1;
    if (somePredicate(a[0])) {
        return (n * -1) + 10;
    }
    return firstTrue(a + 1, n - 1);  // This is incorrect.
}

// Return the subscript of the largest element in the array (i.e.,
// return the smallest subscript m such that a[m] >= a[k] for all
// k from 0 to n-1).  If the function is told to examine no
// elements, return -1.
int positionOfMax(const double a[], int n) {
    if (n <= 0) return -1;
    if (n == 1) return 0;
    if (n == 2) {
        if (a[0] >= a[1]) return 0;
        else return 1;
    }
    int index1 = positionOfMax(a, n / 2);
    int index2 = n / 2 + positionOfMax(a + n / 2, n - n / 2);
    if (a[index1] >= a[index2]) return index1;
    else return index2;
}

// If all n1 elements of a1 appear in the n2 element array a2, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a2 is not a
// not-necessarily-contiguous subsequence of a1), return false.
// (Of course, if a1 is empty (i.e., n1 is 0), return true.)
// For example, if a2 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a1 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a1 is
//    50 30 20
// or
//    10 20 20
// a1 is sublist, a2 is superlist
bool isIn(const double a1[], int n1, const double a2[], int n2) {
    // Handle the negative cases
    if (n1 < 0) return true;
    if (n2 < 0) return false;

    // Handle the escape cases
    // If n1 is ever 0, then we know that we've gotten to the bottom of it and we can return 0
    if (n1 == 0) return true;
    // We have looked through all of n2 and found no matches in n1, so return false
    if (n2 == 0)
        return false;

    if (a1[0] == a2[0]) // Match found
    {
        return isIn(a1 + 1, n1 - 1, a2 + 1, n2 - 1); // Match found, increment both items
    } else {
        return isIn(a1, n1, a2 + 1, n2 - 1); // Match not found for this item in superlist, check next item
    }
}
