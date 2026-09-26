#include <iostream>
using namespace std;

void printArray(const int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        cout << arr[i];
        if (i + 1 < n) cout << ' ';
    }
    cout << '\n';
}

void mergeArrays(int arr[], int low, int mid, int high,
                 int &comparisons, int &finalMergeComparisons,
                 int fullLow, int fullHigh, bool showSteps) {
    int leftSize = mid - low + 1;
    int rightSize = high - mid;

    int *left = new int[leftSize];
    int *right = new int[rightSize];

    for (int i = 0; i < leftSize; ++i) left[i] = arr[low + i];
    for (int j = 0; j < rightSize; ++j) right[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = low;
    int localComparisons = 0;

    while (i < leftSize && j < rightSize) {
        ++comparisons;
        ++localComparisons;

        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }

    while (i < leftSize) arr[k++] = left[i++];
    while (j < rightSize) arr[k++] = right[j++];

    if (low == fullLow && high == fullHigh) {
        finalMergeComparisons = localComparisons;
    }

    if (showSteps) {
        cout << "merged: ";
        printArray(arr + low, high - low + 1);
    }

    delete[] left;
    delete[] right;
}

void mergeSort(int arr[], int low, int high,
               int &comparisons, int &finalMergeComparisons,
               int fullLow, int fullHigh, bool showSteps = true) {
    if (low >= high) return;

    if (showSteps) {
        cout << "split: ";
        printArray(arr + low, high - low + 1);
    }

    int mid = (low + high) / 2;

    mergeSort(arr, low, mid, comparisons, finalMergeComparisons,
              fullLow, fullHigh, showSteps);
    mergeSort(arr, mid + 1, high, comparisons, finalMergeComparisons,
              fullLow, fullHigh, showSteps);

    mergeArrays(arr, low, mid, high, comparisons, finalMergeComparisons,
                fullLow, fullHigh, showSteps);
}

int main() {
    int A[] = {34, 7, 23, 32, 5, 62, 14, 19};
    int n = sizeof(A) / sizeof(A[0]);

    cout << "Original: ";
    printArray(A, n);

    int comparisons = 0, finalMergeComparisons = 0;
    mergeSort(A, 0, n - 1, comparisons, finalMergeComparisons, 0, n - 1);

    cout << "Sorted: ";
    printArray(A, n);
    cout << "Comparisons: " << comparisons
         << "  (final merge: " << finalMergeComparisons << ")\n";

    int sortedA[] = {5, 7, 14, 19, 23, 32, 34, 62};
    int sortedComparisons = 0, sortedFinal = 0;
    mergeSort(sortedA, 0, n - 1, sortedComparisons, sortedFinal, 0, n - 1, false);

    int reversedA[] = {62, 34, 32, 23, 19, 14, 7, 5};
    int reversedComparisons = 0, reversedFinal = 0;
    mergeSort(reversedA, 0, n - 1, reversedComparisons, reversedFinal, 0, n - 1, false);

    cout << "\nSorted-copy comparisons: " << sortedComparisons
         << "  (final merge: " << sortedFinal << ")\n";
    cout << "Reversed-copy comparisons: " << reversedComparisons
         << "  (final merge: " << reversedFinal << ")\n";

    return 0;
}
