#include <iostream>
#include <string>
using namespace std;

void printArray(const int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        cout << arr[i];
        if (i + 1 < n) cout << ' ';
    }
    cout << '\n';
}

void swapValues(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

int partitionArray(int arr[], int low, int high, int &comparisons, bool detailedTrace) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        ++comparisons;

        if (detailedTrace) {
            cout << "  i = " << i << ", j = " << j
                 << ", compare " << arr[j] << " with pivot " << pivot << '\n';
        }

        if (arr[j] <= pivot) {
            ++i;
            swapValues(arr[i], arr[j]);

            if (detailedTrace) {
                cout << "  after swap: ";
                printArray(arr, high - low + 1);
            }
        }
    }

    swapValues(arr[i + 1], arr[high]);
    if (detailedTrace) {
        cout << "  after pivot swap: ";
        printArray(arr + low, high - low + 1);
        cout << "  pivot final position = " << (i + 1) << '\n';
    }

    return i + 1;
}

void quickSort(int arr[], int low, int high, int &comparisons,
               int depth = 0, bool showSteps = true, bool *firstPartition = nullptr) {
    if (low >= high) return;

    if (showSteps) {
        cout << string(depth * 2, ' ')
             << "pivot = " << arr[high] << ": ";
        printArray(arr + low, high - low + 1);
    }

    bool detailedTrace = false;
    if (firstPartition != nullptr && *firstPartition) {
        detailedTrace = showSteps;
        *firstPartition = false;
    }

    int pivotIndex = partitionArray(arr, low, high, comparisons, detailedTrace);

    quickSort(arr, low, pivotIndex - 1, comparisons, depth + 1, showSteps, firstPartition);
    quickSort(arr, pivotIndex + 1, high, comparisons, depth + 1, showSteps, firstPartition);
}

int main() {
    int A[] = {34, 7, 23, 32, 5, 62, 14, 19};
    int n = sizeof(A) / sizeof(A[0]);

    cout << "Original: ";
    printArray(A, n);

    int comparisons = 0;
    bool firstPartition = true;
    quickSort(A, 0, n - 1, comparisons, 0, true, &firstPartition);

    cout << "Sorted: ";
    printArray(A, n);
    cout << "Comparisons: " << comparisons << '\n';

    int sortedA[] = {5, 7, 14, 19, 23, 32, 34, 62};
    int sortedComparisons = 0;
    bool noDetailedTrace = false;
    quickSort(sortedA, 0, n - 1, sortedComparisons, 0, false, &noDetailedTrace);

    cout << "\nAlready sorted input comparisons: " << sortedComparisons << '\n';

    return 0;
}
