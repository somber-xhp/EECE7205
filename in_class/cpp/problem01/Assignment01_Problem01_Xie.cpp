#include <iostream>
using namespace std;

void printArray(const int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        cout << arr[i];
        if (i + 1 < n) cout << ' ';
    }
    cout << '\n';
}

void bubbleSort(int arr[], int n, int &comparisons, int &swaps, int &passes, bool showSteps = true) {
    comparisons = 0;
    swaps = 0;
    passes = 0;

    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;

        for (int j = 0; j < n - 1 - i; ++j) {
            ++comparisons;
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                ++swaps;
                swapped = true;
            }
        }

        ++passes;
        if (showSteps) {
            cout << "Pass " << passes << ": ";
            printArray(arr, n);
        }

        // Stop early when the array is already sorted.
        if (!swapped) break;
    }
}

int main() {
    int A[] = {34, 7, 23, 32, 5, 62, 14, 19};
    int n = sizeof(A) / sizeof(A[0]);

    cout << "Original: ";
    printArray(A, n);

    int comparisons = 0, swaps = 0, passes = 0;
    bubbleSort(A, n, comparisons, swaps, passes);

    cout << "Sorted: ";
    printArray(A, n);
    cout << "Comparisons: " << comparisons << "  Swaps: " << swaps << '\n';

    int sortedA[] = {5, 7, 14, 19, 23, 32, 34, 62};
    int sortedComparisons = 0, sortedSwaps = 0, sortedPasses = 0;
    bubbleSort(sortedA, n, sortedComparisons, sortedSwaps, sortedPasses, false);

    cout << "\nAlready sorted input: ";
    printArray(sortedA, n);
    cout << "Passes: " << sortedPasses
         << "  Comparisons: " << sortedComparisons
         << "  Swaps: " << sortedSwaps << '\n';

    return 0;
}
