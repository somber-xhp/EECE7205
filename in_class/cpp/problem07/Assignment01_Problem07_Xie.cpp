#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>
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

long long bubbleSort(int arr[], int n) {
    long long comparisons = 0;
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; ++j) {
            ++comparisons;
            if (arr[j] > arr[j + 1]) {
                swapValues(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
    return comparisons;
}

long long insertionSort(int arr[], int n) {
    long long comparisons = 0;
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            ++comparisons;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                --j;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
    }
    return comparisons;
}

long long selectionSort(int arr[], int n) {
    long long comparisons = 0;
    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < n; ++j) {
            ++comparisons;
            if (arr[j] < arr[minIdx]) minIdx = j;
        }
        if (minIdx != i) swapValues(arr[i], arr[minIdx]);
    }
    return comparisons;
}

int partitionArray(int arr[], int low, int high, long long &comparisons) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        ++comparisons;
        if (arr[j] <= pivot) {
            ++i;
            swapValues(arr[i], arr[j]);
        }
    }
    swapValues(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortRecursive(int arr[], int low, int high, long long &comparisons) {
    if (low >= high) return;
    int p = partitionArray(arr, low, high, comparisons);
    quickSortRecursive(arr, low, p - 1, comparisons);
    quickSortRecursive(arr, p + 1, high, comparisons);
}

long long quickSort(int arr[], int n) {
    long long comparisons = 0;
    quickSortRecursive(arr, 0, n - 1, comparisons);
    return comparisons;
}

void mergeArrays(int arr[], int low, int mid, int high, long long &comparisons, vector<int> &temp) {
    int i = low;
    int j = mid + 1;
    int k = low;

    while (i <= mid && j <= high) {
        ++comparisons;
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }

    while (i <= mid) temp[k++] = arr[i++];
    while (j <= high) temp[k++] = arr[j++];

    for (int p = low; p <= high; ++p) arr[p] = temp[p];
}

void mergeSortRecursive(int arr[], int low, int high, long long &comparisons, vector<int> &temp) {
    if (low >= high) return;
    int mid = (low + high) / 2;
    mergeSortRecursive(arr, low, mid, comparisons, temp);
    mergeSortRecursive(arr, mid + 1, high, comparisons, temp);
    mergeArrays(arr, low, mid, high, comparisons, temp);
}

long long mergeSort(int arr[], int n) {
    long long comparisons = 0;
    vector<int> temp(n);
    mergeSortRecursive(arr, 0, n - 1, comparisons, temp);
    return comparisons;
}

void siftDown(int arr[], int root, int size, long long &comparisons) {
    while (true) {
        int largest = root;
        int left = 2 * root + 1;
        int right = 2 * root + 2;

        if (left < size) {
            ++comparisons;
            if (arr[left] > arr[largest]) largest = left;
        }
        if (right < size) {
            ++comparisons;
            if (arr[right] > arr[largest]) largest = right;
        }

        if (largest == root) break;
        swapValues(arr[root], arr[largest]);
        root = largest;
    }
}

long long heapSort(int arr[], int n) {
    long long comparisons = 0;
    for (int i = n / 2 - 1; i >= 0; --i) siftDown(arr, i, n, comparisons);
    for (int end = n - 1; end >= 1; --end) {
        swapValues(arr[0], arr[end]);
        siftDown(arr, 0, end, comparisons);
    }
    return comparisons;
}

using SortFunction = long long (*)(int[], int);

struct Algorithm {
    string name;
    SortFunction sortFunction;
};

long long comparisonsFor(const vector<int> &input, SortFunction function) {
    vector<int> copy = input;
    return function(copy.data(), static_cast<int>(copy.size()));
}

double timeSortMs(const vector<int> &input, SortFunction function, vector<int> &sortedOutput) {
    sortedOutput = input;
    auto start = chrono::high_resolution_clock::now();
    function(sortedOutput.data(), static_cast<int>(sortedOutput.size()));
    auto stop = chrono::high_resolution_clock::now();
    return chrono::duration<double, milli>(stop - start).count();
}

int main() {
    vector<Algorithm> algorithms = {
        {"Bubble", bubbleSort},
        {"Insertion", insertionSort},
        {"Selection", selectionSort},
        {"Quick", quickSort},
        {"Merge", mergeSort},
        {"Heap", heapSort}
    };

    vector<int> A = {34, 7, 23, 32, 5, 62, 14, 19};
    vector<int> sortedA = {5, 7, 14, 19, 23, 32, 34, 62};
    vector<int> reversedA = {62, 34, 32, 23, 19, 14, 7, 5};

    cout << "Comparison counts\n";
    cout << left << setw(12) << "Algorithm"
         << right << setw(10) << "A"
         << setw(12) << "Sorted"
         << setw(12) << "Reversed" << '\n';

    for (const auto &algorithm : algorithms) {
        cout << left << setw(12) << algorithm.name
             << right << setw(10) << comparisonsFor(A, algorithm.sortFunction)
             << setw(12) << comparisonsFor(sortedA, algorithm.sortFunction)
             << setw(12) << comparisonsFor(reversedA, algorithm.sortFunction)
             << '\n';
    }

    mt19937 generator(2026);
    uniform_int_distribution<int> distribution(1, 100000);
    int sizes[] = {1000, 5000, 10000};

    cout << fixed << setprecision(3);

    for (int n : sizes) {
        vector<int> randomData(n);
        for (int &value : randomData) value = distribution(generator);

        cout << "\nRandom array size = " << n << "\n";
        cout << left << setw(12) << "Algorithm"
             << right << setw(14) << "Time (ms)" << "  First 10 sorted values\n";

        for (const auto &algorithm : algorithms) {
            vector<int> sortedOutput;
            double elapsed = timeSortMs(randomData, algorithm.sortFunction, sortedOutput);

            cout << left << setw(12) << algorithm.name
                 << right << setw(14) << elapsed << "  ";
            printArray(sortedOutput.data(), 10);
        }
    }

    return 0;
}
