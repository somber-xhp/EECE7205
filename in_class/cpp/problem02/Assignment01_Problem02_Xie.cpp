#include <iostream>
using namespace std;

void printArray(const int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        cout << arr[i];
        if (i + 1 < n) cout << ' ';
    }
    cout << '\n';
}

void insertionSort(int arr[], int n, int &comparisons, int &shifts, bool showSteps = true) {
    comparisons = 0;
    shifts = 0;

    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;

        // Shift larger values right until the key can be inserted.
        while (j >= 0) {
            ++comparisons;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                ++shifts;
                --j;
            } else {
                break;
            }
        }
        arr[j + 1] = key;

        if (showSteps) {
            cout << "i = " << i << ", key = " << key << ": ";
            printArray(arr, n);
        }
    }
}

int main() {
    int A[] = {34, 7, 23, 32, 5, 62, 14, 19};
    int n = sizeof(A) / sizeof(A[0]);

    cout << "Original: ";
    printArray(A, n);

    int comparisons = 0, shifts = 0;
    insertionSort(A, n, comparisons, shifts);

    cout << "Sorted: ";
    printArray(A, n);
    cout << "Comparisons: " << comparisons << "  Shifts: " << shifts << '\n';

    int bestCase[] = {5, 7, 14, 19, 23, 32, 34, 62};
    int bestComparisons = 0, bestShifts = 0;
    insertionSort(bestCase, n, bestComparisons, bestShifts, false);

    int worstCase[] = {62, 34, 32, 23, 19, 14, 7, 5};
    int worstComparisons = 0, worstShifts = 0;
    insertionSort(worstCase, n, worstComparisons, worstShifts, false);

    cout << "\nFewest-shift ordering (sorted): ";
    printArray(bestCase, n);
    cout << "Comparisons: " << bestComparisons << "  Shifts: " << bestShifts << '\n';

    cout << "Most-shift ordering (reversed): ";
    printArray(worstCase, n);
    cout << "Comparisons: " << worstComparisons << "  Shifts: " << worstShifts << '\n';

    return 0;
}
