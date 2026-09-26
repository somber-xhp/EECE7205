#include <algorithm>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

using Interval = pair<int, int>;
using Point = pair<int, int>;

// ---------- Utility printing ----------
void printIntervals(const vector<Interval>& intervals) {
    cout << "[";
    for (size_t i = 0; i < intervals.size(); ++i) {
        cout << "[" << intervals[i].first << ", " << intervals[i].second << "]";
        if (i + 1 < intervals.size()) cout << ", ";
    }
    cout << "]\n";
}

void printIntVector(const vector<int>& values) {
    cout << "[";
    for (size_t i = 0; i < values.size(); ++i) {
        cout << values[i];
        if (i + 1 < values.size()) cout << ", ";
    }
    cout << "]\n";
}

void printPoints(const vector<Point>& points) {
    cout << "[";
    for (size_t i = 0; i < points.size(); ++i) {
        cout << "(" << points[i].first << ", " << points[i].second << ")";
        if (i + 1 < points.size()) cout << ", ";
    }
    cout << "]\n";
}

// ---------- Problem 1: Merge Intervals ----------
vector<Interval> mergeIntervals(vector<Interval> intervals) {
    if (intervals.empty()) return {};

    for (const auto& interval : intervals) {
        if (interval.first > interval.second) {
            throw invalid_argument("Each interval must satisfy start <= end.");
        }
    }

    // Sort intervals by start time before merging.
    sort(intervals.begin(), intervals.end(), [](const Interval& a, const Interval& b) {
        if (a.first != b.first) return a.first < b.first;
        return a.second < b.second;
    });

    vector<Interval> merged;
    merged.push_back(intervals[0]);

    // Merge the current interval if it overlaps the previous one.
    for (size_t i = 1; i < intervals.size(); ++i) {
        if (intervals[i].first <= merged.back().second) {
            merged.back().second = max(merged.back().second, intervals[i].second);
        } else {
            merged.push_back(intervals[i]);
        }
    }
    return merged;
}

// ---------- Problem 2: Sort 0, 1, 2 ----------
void validate012(const vector<int>& values) {
    for (int value : values) {
        if (value < 0 || value > 2) {
            throw invalid_argument("Problem 2 accepts only 0, 1, and 2.");
        }
    }
}

void sort012Counting(vector<int>& values) {
    validate012(values);
    // Count how many 0s, 1s, and 2s appear.
    int count[3] = {0, 0, 0};
    for (int value : values) ++count[value];

    size_t index = 0;
    for (int value = 0; value <= 2; ++value) {
        for (int c = 0; c < count[value]; ++c) {
            values[index++] = value;
        }
    }
}

void sort012DNF(vector<int>& values) {
    validate012(values);
    int low = 0;
    int mid = 0;
    int high = static_cast<int>(values.size()) - 1;

    // Keep 0s on the left, 1s in the middle, and 2s on the right.
    while (mid <= high) {
        if (values[mid] == 0) {
            swap(values[low], values[mid]);
            ++low;
            ++mid;
        } else if (values[mid] == 1) {
            ++mid;
        } else {
            swap(values[mid], values[high]);
            --high;
        }
    }
}

// ---------- Problem 3: Largest Number ----------
string largestNumber(const vector<int>& numbers) {
    if (numbers.empty()) return "";

    vector<string> parts;
    parts.reserve(numbers.size());
    for (int value : numbers) {
        if (value < 0) throw invalid_argument("Numbers must be nonnegative.");
        parts.push_back(to_string(value));
    }

    // Order strings by which concatenation creates a larger number.
    sort(parts.begin(), parts.end(), [](const string& a, const string& b) {
        return a + b > b + a;
    });

    if (parts[0] == "0") return "0";

    string result;
    for (const string& part : parts) result += part;
    return result;
}

// ---------- Problem 4: Group Anagrams ----------
vector<vector<string>> groupAnagrams(const vector<string>& words) {
    unordered_map<string, size_t> groupIndex;
    vector<vector<string>> groups;

    for (const string& word : words) {
        // Sorted letters form the same key for all anagrams.
        string key = word;
        sort(key.begin(), key.end());

        auto it = groupIndex.find(key);
        if (it == groupIndex.end()) {
            groupIndex[key] = groups.size();
            groups.push_back({word});
        } else {
            groups[it->second].push_back(word);
        }
    }
    return groups;
}

void printStringGroups(const vector<vector<string>>& groups) {
    cout << "[\n";
    for (const auto& group : groups) {
        cout << "  [";
        for (size_t i = 0; i < group.size(); ++i) {
            cout << '"' << group[i] << '"';
            if (i + 1 < group.size()) cout << ", ";
        }
        cout << "]\n";
    }
    cout << "]\n";
}

// ---------- Problem 5: Can Attend All Meetings ----------
bool canAttendAllMeetings(vector<Interval> meetings) {
    for (const auto& meeting : meetings) {
        if (meeting.first > meeting.second) {
            throw invalid_argument("Each meeting must satisfy start <= end.");
        }
    }

    sort(meetings.begin(), meetings.end(), [](const Interval& a, const Interval& b) {
        if (a.first != b.first) return a.first < b.first;
        return a.second < b.second;
    });

    // After sorting, only adjacent meetings need to be checked.
    for (size_t i = 1; i < meetings.size(); ++i) {
        if (meetings[i].first < meetings[i - 1].second) return false;
    }
    return true;
}

// ---------- Problem 6: K Closest Points ----------
// Squared distance is enough; square root is unnecessary.
long long squaredDistance(const Point& p) {
    return 1LL * p.first * p.first + 1LL * p.second * p.second;
}

vector<Point> kClosestSort(vector<Point> points, int k) {
    if (k <= 0 || points.empty()) return {};
    k = min(k, static_cast<int>(points.size()));

    sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
        long long da = squaredDistance(a);
        long long db = squaredDistance(b);
        if (da != db) return da < db;
        if (a.first != b.first) return a.first < b.first;
        return a.second < b.second;
    });

    points.resize(k);
    return points;
}

vector<Point> kClosestHeap(const vector<Point>& points, int k) {
    if (k <= 0 || points.empty()) return {};
    k = min(k, static_cast<int>(points.size()));

    using Item = tuple<long long, int, int>; // distance^2, x, y
    // Keep only the k closest points in a max-heap.
    priority_queue<Item> maxHeap;

    for (const Point& p : points) {
        Item item = make_tuple(squaredDistance(p), p.first, p.second);
        if (static_cast<int>(maxHeap.size()) < k) {
            maxHeap.push(item);
        } else if (item < maxHeap.top()) {
            maxHeap.pop();
            maxHeap.push(item);
        }
    }

    vector<Item> selected;
    while (!maxHeap.empty()) {
        selected.push_back(maxHeap.top());
        maxHeap.pop();
    }
    sort(selected.begin(), selected.end());

    vector<Point> result;
    for (const Item& item : selected) {
        result.push_back({get<1>(item), get<2>(item)});
    }
    return result;
}

// ---------- Problem 7: Top K Frequent ----------
vector<int> topKFrequent(const vector<int>& values, int k) {
    if (k <= 0 || values.empty()) return {};

    // Count the frequency of each value.
    unordered_map<int, int> frequency;
    for (int value : values) ++frequency[value];

    vector<pair<int, int>> items; // value, frequency
    items.reserve(frequency.size());
    for (const auto& entry : frequency) {
        items.push_back({entry.first, entry.second});
    }

    sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) return a.second > b.second;
        return a.first < b.first;
    });

    k = min(k, static_cast<int>(items.size()));
    vector<int> result;
    for (int i = 0; i < k; ++i) result.push_back(items[i].first);
    return result;
}

// ---------- Problem 8: Merge K Sorted Arrays ----------
vector<int> mergeKSortedArrays(const vector<vector<int>>& arrays) {
    using Node = tuple<int, int, int>; // value, array index, element index
    // Min-heap always gives the smallest next value among all arrays.
    priority_queue<Node, vector<Node>, greater<Node>> minHeap;

    for (int i = 0; i < static_cast<int>(arrays.size()); ++i) {
        if (!arrays[i].empty()) {
            minHeap.emplace(arrays[i][0], i, 0);
        }
    }

    vector<int> result;
    while (!minHeap.empty()) {
        auto [value, arrayIndex, elementIndex] = minHeap.top();
        minHeap.pop();
        result.push_back(value);

        int nextIndex = elementIndex + 1;
        if (nextIndex < static_cast<int>(arrays[arrayIndex].size())) {
            minHeap.emplace(arrays[arrayIndex][nextIndex], arrayIndex, nextIndex);
        }
    }
    return result;
}

// ---------- Problem 9: Kth Largest ----------
int kthLargest(const vector<int>& values, int k) {
    if (k < 1 || k > static_cast<int>(values.size())) {
        throw invalid_argument("k must be between 1 and the array size.");
    }

    // Keep the k largest values; heap top is the kth largest.
    priority_queue<int, vector<int>, greater<int>> minHeap;
    for (int value : values) {
        if (static_cast<int>(minHeap.size()) < k) {
            minHeap.push(value);
        } else if (value > minHeap.top()) {
            minHeap.pop();
            minHeap.push(value);
        }
    }
    return minHeap.top();
}

// ---------- Problem 10: Count Inversions ----------
long long mergeAndCount(vector<int>& values, vector<int>& temp, int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int t = left;
    long long inversions = 0;

    while (i <= mid && j <= right) {
        if (values[i] <= values[j]) {
            temp[t++] = values[i++];
        } else {
            temp[t++] = values[j++];
            // All remaining left-side values are larger than values[j].
            inversions += static_cast<long long>(mid - i + 1);
        }
    }

    while (i <= mid) temp[t++] = values[i++];
    while (j <= right) temp[t++] = values[j++];

    for (int index = left; index <= right; ++index) {
        values[index] = temp[index];
    }
    return inversions;
}

long long mergeSortCount(vector<int>& values, vector<int>& temp, int left, int right) {
    if (left >= right) return 0;

    int mid = left + (right - left) / 2;
    long long inversions = 0;
    inversions += mergeSortCount(values, temp, left, mid);
    inversions += mergeSortCount(values, temp, mid + 1, right);
    inversions += mergeAndCount(values, temp, left, mid, right);
    return inversions;
}

long long countInversions(vector<int> values) {
    if (values.empty()) return 0;
    vector<int> temp(values.size());
    return mergeSortCount(values, temp, 0, static_cast<int>(values.size()) - 1);
}

// ---------- Interactive runners ----------
vector<Interval> readIntervals() {
    int n;
    cout << "Number of intervals: ";
    cin >> n;
    if (n < 0) throw invalid_argument("Count cannot be negative.");
    vector<Interval> intervals(n);
    cout << "Enter each interval as: start end\n";
    for (int i = 0; i < n; ++i) cin >> intervals[i].first >> intervals[i].second;
    return intervals;
}

vector<int> readIntVector() {
    int n;
    cout << "Number of values: ";
    cin >> n;
    if (n < 0) throw invalid_argument("Count cannot be negative.");
    vector<int> values(n);
    cout << "Enter the values: ";
    for (int& value : values) cin >> value;
    return values;
}

void runProblem1() {
    auto intervals = readIntervals();
    cout << "Merged intervals: ";
    printIntervals(mergeIntervals(intervals));
}

void runProblem2() {
    auto values = readIntVector();
    vector<int> counting = values;
    vector<int> dnf = values;
    sort012Counting(counting);
    sort012DNF(dnf);
    cout << "Counting result: ";
    printIntVector(counting);
    cout << "Dutch National Flag result: ";
    printIntVector(dnf);
}

void runProblem3() {
    auto values = readIntVector();
    cout << "Largest number: " << largestNumber(values) << "\n";
}

void runProblem4() {
    int n;
    cout << "Number of words: ";
    cin >> n;
    if (n < 0) throw invalid_argument("Count cannot be negative.");
    vector<string> words(n);
    cout << "Enter words separated by spaces: ";
    for (string& word : words) cin >> word;
    cout << "Grouped anagrams:\n";
    printStringGroups(groupAnagrams(words));
}

void runProblem5() {
    auto meetings = readIntervals();
    cout << (canAttendAllMeetings(meetings) ? "Can attend all meetings: true\n"
                                            : "Can attend all meetings: false\n");
}

void runProblem6() {
    int n, k;
    cout << "Number of points: ";
    cin >> n;
    if (n < 0) throw invalid_argument("Count cannot be negative.");
    vector<Point> points(n);
    cout << "Enter each point as: x y\n";
    for (auto& p : points) cin >> p.first >> p.second;
    cout << "k: ";
    cin >> k;

    cout << "Sorting solution: ";
    printPoints(kClosestSort(points, k));
    cout << "Heap solution: ";
    printPoints(kClosestHeap(points, k));
}

void runProblem7() {
    auto values = readIntVector();
    int k;
    cout << "k: ";
    cin >> k;
    cout << "Top k frequent values: ";
    printIntVector(topKFrequent(values, k));
}

void runProblem8() {
    int k;
    cout << "Number of sorted arrays: ";
    cin >> k;
    if (k < 0) throw invalid_argument("Count cannot be negative.");
    vector<vector<int>> arrays(k);
    for (int i = 0; i < k; ++i) {
        int n;
        cout << "Size of array " << i + 1 << ": ";
        cin >> n;
        if (n < 0) throw invalid_argument("Array size cannot be negative.");
        arrays[i].resize(n);
        cout << "Enter sorted values: ";
        for (int& value : arrays[i]) cin >> value;
    }
    cout << "Merged array: ";
    printIntVector(mergeKSortedArrays(arrays));
}

void runProblem9() {
    auto values = readIntVector();
    int k;
    cout << "k: ";
    cin >> k;
    cout << k << "th largest element: " << kthLargest(values, k) << "\n";
}

void runProblem10() {
    auto values = readIntVector();
    cout << "Inversion count: " << countInversions(values) << "\n";
}

void printMenu() {
    cout << "\n===== EECE 7205 Assignment #01 =====\n"
         << "1. Merge Intervals\n"
         << "2. Sort 0, 1, 2\n"
         << "3. Largest Number\n"
         << "4. Group Anagrams\n"
         << "5. Can Attend All Meetings\n"
         << "6. K Closest Points\n"
         << "7. Top K Frequent Values\n"
         << "8. Merge K Sorted Arrays\n"
         << "9. Kth Largest Element\n"
         << "10. Count Inversions\n"
         << "0. Exit\n"
         << "Choose a problem: ";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(&cout);

    while (true) {
        printMenu();
        int choice;
        if (!(cin >> choice)) break;
        if (choice == 0) break;

        try {
            switch (choice) {
                case 1: runProblem1(); break;
                case 2: runProblem2(); break;
                case 3: runProblem3(); break;
                case 4: runProblem4(); break;
                case 5: runProblem5(); break;
                case 6: runProblem6(); break;
                case 7: runProblem7(); break;
                case 8: runProblem8(); break;
                case 9: runProblem9(); break;
                case 10: runProblem10(); break;
                default: cout << "Invalid menu choice.\n"; break;
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }

    cout << "Program ended.\n";
    return 0;
}
