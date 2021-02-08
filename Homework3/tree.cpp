// Return the number of ways that all n1 elements of a1 appear in
// the n2 element array a2 in the same order (though not necessarily
// consecutively).  The empty sequence (i.e. one where n1 is 0)
// appears in a sequence of length n2 in 1 way, even if n2 is 0.
// For example, if a2 is the 7 element array
//	10 50 40 20 50 40 30
// then for this value of a1     the function must return
//	10 20 40			1
//	10 40 30			2
//	20 10 40			0
//	50 40 30			3
int countIsIn(const double a1[], int n1, const double a2[], int n2) {
    if (n2 < 0) return 0; // Handle the negative case
    if (n1 <= 0)
        return 1; // If n1 is ever 0, then we know that we've gotten to the bottom of it and we can return 1
    if (n2 == 0) // We have looked through all of n2 and found no matches in n1, so return 0
        return 0;

    // If n1 is less than n2, we know there's no reason to continue testing so we can skip
    if (n2 < n1)
        return 0;

    int count = 0;

    if (a1[0] == a2[0]) // Match found
    {
        count += countIsIn(a1 + 1, n1 - 1, a2 + 1, n2 - 1); // Match found, increment both items
        count += countIsIn(a1, n1, a2 + 1, n2 - 1);
    } else {
        count += countIsIn(a1, n1, a2 + 1, n2 - 1); // Match not found for this item in superlist, check next item
    }
    return count;
}

// Exchange two doubles
void exchange(double &x, double &y) {
    double t = x;
    x = y;
    y = t;
}

// Rearrange the elements of the array so that all the elements
// whose value is > divider come before all the other elements,
// and all the elements whose value is < divider come after all
// the other elements.  Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= divider, or n if there is no such element, and firstLess is
// set to the index of the first element that is < divider, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > divider
//   * for firstNotGreater <= i < firstLess, a[i] == divider
//   * for firstLess <= i < n, a[i] < divider
// All the elements > divider end up in no particular order.
// All the elements < divider end up in no particular order.
void divide(double a[], int n, double divider,
            int &firstNotGreater, int &firstLess) {
    if (n < 0)
        n = 0;

    // It will always be the case that just before evaluating the loop
    // condition:
    //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
    //  Every element earlier than position firstNotGreater is > divider
    //  Every element from position firstNotGreater to firstUnknown-1 is
    //    == divider
    //  Every element from firstUnknown to firstLess-1 is not known yet
    //  Every element at position firstLess or later is < divider

    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess) {
        if (a[firstUnknown] < divider) {
            firstLess--;
            exchange(a[firstUnknown], a[firstLess]);
        } else {
            if (a[firstUnknown] > divider) {
                exchange(a[firstNotGreater], a[firstUnknown]);
                firstNotGreater++;
            }
            firstUnknown++;
        }
    }
}

// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n) {
    if (n <= 1) return;

    order(a, n - 1); // On first run, open up a tree of every sublist like:
    // a b c d e f g 7
    //   b c d e f g 6
    //     c d e f g 5
    //       d e f g 4
    //         e f g 3
    //           f g 2
    //             g 1

    // Starting at the end, shift the smallest element to the right (aka shift the largest element to the far left)
    if (a[n - 2] < a[n - 1]) exchange(a[n - 1], a[n - 2]);
    order(a, n - 1);
}
