#include <iostream>
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

void siftDown(int arr[], int root, int size, int &swaps) {
    while (true) {
        int largest = root;
        int left = 2 * root + 1;
        int right = 2 * root + 2;

        if (left < size && arr[left] > arr[largest]) largest = left;
        if (right < size && arr[right] > arr[largest]) largest = right;

        if (largest == root) break;

        swapValues(arr[root], arr[largest]);
        ++swaps; // Count only swaps made inside siftDown.
        root = largest;
    }
}

void heapSort(int arr[], int n, int &siftDownSwaps) {
    siftDownSwaps = 0;

    // Build the max-heap.
    for (int i = n / 2 - 1; i >= 0; --i) {
        siftDown(arr, i, n, siftDownSwaps);
    }

    cout << "Heap: ";
    printArray(arr, n);

    // Move the maximum to the sorted end one position at a time.
    for (int end = n - 1; end >= 1; --end) {
        swapValues(arr[0], arr[end]);
        siftDown(arr, 0, end, siftDownSwaps);

        cout << "end = " << end << ": heap: ";
        printArray(arr, end);
        cout << "         sorted: ";
        printArray(arr + end, n - end);
    }
}

int main() {
    int A[] = {34, 7, 23, 32, 5, 62, 14, 19};
    int n = sizeof(A) / sizeof(A[0]);

    cout << "Original: ";
    printArray(A, n);

    int swaps = 0;
    heapSort(A, n, swaps);

    cout << "Sorted: ";
    printArray(A, n);
    cout << "Swaps in siftDown: " << swaps << '\n';

    return 0;
}
