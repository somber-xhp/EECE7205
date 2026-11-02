#ifndef AIRPORT_BST_H
#define AIRPORT_BST_H
#include <string>
#include <vector>
#include <iosfwd>

class Airport {
public:
    std::string code, name, city, country, icao;
    double latitude = 0, longitude = 0;
};

class AirportBST {
    struct Node {
        Airport airport;
        Node* left = nullptr;
        Node* right = nullptr;
        explicit Node(const Airport& a) : airport(a) {}
    };
    Node* root = nullptr;
    static Node* balanced(const std::vector<Airport>& a, int lo, int hi);
public:
    struct Metrics { int count = 0, height = -1; double averageDepth = 0; };
    struct Search { const Airport* airport = nullptr; std::vector<std::string> path; };
    struct Range { std::vector<Airport> airports; int visited = 0; };
    AirportBST() = default;
    ~AirportBST();
    AirportBST(const AirportBST&) = delete;
    AirportBST& operator=(const AirportBST&) = delete;
    void clear();
    bool insert(const Airport& a);
    const Airport* findFast(const std::string& code, int* comparisons = nullptr) const;
    Search find(const std::string& code) const;
    bool remove(const std::string& code, std::string& detail);
    bool validate() const;
    Metrics metrics() const;
    std::vector<Airport> inorder() const;
    std::vector<std::string> preorder() const;
    std::vector<std::string> levelOrder(int maxLevels = -1) const;
    Range range(const std::string& lo, const std::string& hi) const;
    std::string minimum() const;
    std::string maximum() const;
    std::string rootCode() const;
    std::string caseCode(int kind) const; // 0 leaf, 1 left-only, 2 right-only, 3 two
    std::string deepestCode() const;
    void rebuildBalanced(const std::vector<Airport>& sorted);
    bool save(const std::string& filename) const;
    bool loadPreorder(const std::string& filename,
                      const std::vector<Airport>& catalog, std::string& error);
};

struct LoadResult {
    std::vector<Airport> airports;
    int lines = 0, skipped = 0;
};
std::vector<std::string> splitCSV(const std::string& line);
LoadResult readAirports(const std::string& filename);
void printAirport(const Airport& a, std::ostream& out);
#endif
