#include <iostream>
using namespace std;

void printArray(const int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        cout << arr[i];
        if (i + 1 < n) cout << ' ';
    }
    cout << '\n';
}

void selectionSort(int arr[], int n, int &comparisons, int &swaps, bool showSteps = true) {
    comparisons = 0;
    swaps = 0;

    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i;

        for (int j = i + 1; j < n; ++j) {
            ++comparisons;
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }

        int minValue = arr[minIdx];
        if (minIdx != i) {
            int temp = arr[i];
            arr[i] = arr[minIdx];
            arr[minIdx] = temp;
            ++swaps;
        }

        if (showSteps) {
            cout << "Pass " << (i + 1)
                 << ": min = " << minValue
                 << " at index " << minIdx << " -> ";
            printArray(arr, n);
        }
    }
}

int main() {
    int A[] = {34, 7, 23, 32, 5, 62, 14, 19};
    int n = sizeof(A) / sizeof(A[0]);

    cout << "Original: ";
    printArray(A, n);

    int comparisons = 0, swaps = 0;
    selectionSort(A, n, comparisons, swaps);

    cout << "Sorted: ";
    printArray(A, n);
    cout << "Comparisons: " << comparisons << "  Swaps: " << swaps << '\n';

    return 0;
}
