#include "AirportBST.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <queue>
#include <stack>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <ostream>

std::vector<std::string> splitCSV(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool quoted = false;
    int n = static_cast<int>(line.size());
    for (int i = 0; i < n; i++) {
        char c = line[i];
        if (c == '"') {
            if (quoted && i + 1 < n && line[i + 1] == '"') {
                field += '"'; i++;
            } else quoted = !quoted;
        } else if (c == ',' && !quoted) {
            fields.push_back(field); field.clear();
        } else if (c != '\r') field += c;
    }
    if (quoted) throw std::runtime_error("Unclosed CSV quote");
    fields.push_back(field);
    return fields;
}

LoadResult readAirports(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) throw std::runtime_error("Cannot open " + filename);
    LoadResult result;
    std::string line;
    while (std::getline(in, line)) {
        auto f = splitCSV(line);
        if (result.lines == 0 && !f.empty() && f[0] == "airport_id") continue;
        result.lines++;
        if (f.size() < 8) throw std::runtime_error("Short data row");
        if (f[4] == "\\N" || f[4].empty()) { result.skipped++; continue; }
        // Keep every nonmissing key, including the posted file's DU9.
        Airport a;
        a.name=f[1]; a.city=f[2]; a.country=f[3]; a.code=f[4]; a.icao=f[5];
        a.latitude=std::stod(f[6]); a.longitude=std::stod(f[7]);
        result.airports.push_back(a);
    }
    return result;
}
void printAirport(const Airport& a, std::ostream& out) {
    out << a.code << " | " << a.name << " | " << a.city << " | "
        << a.country << " | ICAO=" << a.icao << " | "
        << std::setprecision(12) << a.latitude << ", " << a.longitude << '\n';
}
AirportBST::~AirportBST() { clear(); }
void AirportBST::clear() {
    // Constant-extra-space rotations; safe for the sorted chain.
    while (root != nullptr) {
        if (root->left != nullptr) {
            Node* next=root->left;
            root->left=next->right; next->right=root; root=next;
        } else { Node* next=root->right; delete root; root=next; }
    }
}
bool AirportBST::insert(const Airport& a) {
    Node** link=&root;
    while (*link != nullptr) {
        if (a.code < (*link)->airport.code) link=&(*link)->left;
        else if (a.code > (*link)->airport.code) link=&(*link)->right;
        else return false; // I4: reject duplicates, preserve existing record.
    }
    *link=new Node(a); return true;
}
const Airport* AirportBST::findFast(const std::string& code, int* comparisons) const {
    Node* cur=root;
    if (comparisons) *comparisons=0;
    while (cur != nullptr) {
        if (comparisons) (*comparisons)++;
        if (code == cur->airport.code) return &cur->airport;
        cur=(code < cur->airport.code) ? cur->left : cur->right;
    }
    return nullptr;
}
AirportBST::Search AirportBST::find(const std::string& code) const {
    Search result; Node* cur=root;
    while (cur != nullptr) {
        result.path.push_back(cur->airport.code);
        if (code == cur->airport.code) { result.airport=&cur->airport; break; }
        cur=(code < cur->airport.code) ? cur->left : cur->right;
    }
    return result;
}
bool AirportBST::remove(const std::string& code, std::string& detail) {
    Node* parent=nullptr; Node* cur=root;
    while (cur && cur->airport.code != code) {
        parent=cur; cur=(code < cur->airport.code) ? cur->left : cur->right;
    }
    if (!cur) { detail=root ? "not found" : "tree is empty"; return false; }
    bool isRoot=(parent == nullptr);
    if (cur->left && cur->right) {
        Node* sp=cur; Node* succ=cur->right;
        while (succ->left) { sp=succ; succ=succ->left; }
        detail="D3: two children; successor " + succ->airport.code;
        cur->airport=succ->airport; // Copy the entire airport record.
        if (sp == cur) sp->right=succ->right;
        else sp->left=succ->right;
        delete succ;
    } else {
        Node* child=cur->left ? cur->left : cur->right;
        if (!child) detail="D1: leaf";
        else detail=std::string("D2: only a ") + (cur->left ? "left" : "right")
                    + " child, " + child->airport.code;
        if (!parent) root=child;
        else if (parent->left == cur) parent->left=child;
        else parent->right=child;
        delete cur;
    }
    if (isRoot) detail += "; root deletion";
    return true;
}
bool AirportBST::validate() const {
    // Strict inherited bounds catch violations involving any ancestor.
    using Frame=std::tuple<Node*,const std::string*,const std::string*>;
    std::stack<Frame> work;
    if (root) work.push({root,nullptr,nullptr});
    while (!work.empty()) {
        auto [node,lo,hi]=work.top(); work.pop();
        const std::string& k=node->airport.code;
        if ((lo && k <= *lo) || (hi && k >= *hi)) return false;
        if (node->right) work.push({node->right,&k,hi});
        if (node->left) work.push({node->left,lo,&k});
    }
    return true;
}
AirportBST::Metrics AirportBST::metrics() const {
    Metrics m; long long total=0;
    std::stack<std::pair<Node*,int>> work;
    if (root) work.push({root,0});
    while (!work.empty()) {
        auto [node,depth]=work.top(); work.pop();
        m.count++; total+=depth; m.height=std::max(m.height,depth);
        if (node->right) work.push({node->right,depth+1});
        if (node->left) work.push({node->left,depth+1});
    }
    if (m.count) m.averageDepth=static_cast<double>(total)/m.count;
    return m;
}
std::vector<Airport> AirportBST::inorder() const {
    std::vector<Airport> out; std::stack<Node*> work; Node* cur=root;
    while (cur || !work.empty()) {
        while (cur) { work.push(cur); cur=cur->left; }
        cur=work.top(); work.pop(); out.push_back(cur->airport); cur=cur->right;
    }
    return out;
}
std::vector<std::string> AirportBST::preorder() const {
    std::vector<std::string> out; std::stack<Node*> work;
    if (root) work.push(root);
    while (!work.empty()) {
        Node* node=work.top(); work.pop(); out.push_back(node->airport.code);
        if (node->right) work.push(node->right);
        if (node->left) work.push(node->left);
    }
    return out;
}
std::vector<std::string> AirportBST::levelOrder(int maxLevels) const {
    std::vector<std::string> out; std::queue<std::pair<Node*,int>> work;
    if (root && maxLevels != 0) work.push({root,0});
    while (!work.empty()) {
        auto [node,depth]=work.front(); work.pop(); out.push_back(node->airport.code);
        if (maxLevels >= 0 && depth+1 >= maxLevels) continue;
        if (node->left) work.push({node->left,depth+1});
        if (node->right) work.push({node->right,depth+1});
    }
    return out;
}
AirportBST::Range AirportBST::range(const std::string& lo,const std::string& hi) const {
    Range out; if (lo > hi) return out;
    // A node is counted once when first examined. Deferred output is not a visit.
    std::stack<std::pair<Node*,bool>> work;
    if (root) work.push({root,false});
    while (!work.empty()) {
        auto [node,emit]=work.top(); work.pop();
        const std::string& k=node->airport.code;
        if (emit) { out.airports.push_back(node->airport); continue; }
        out.visited++;
        if (k < hi && node->right) work.push({node->right,false});
        if (lo <= k && k <= hi) work.push({node,true});
        if (lo < k && node->left) work.push({node->left,false});
    }
    return out;
}
std::string AirportBST::minimum() const {
    Node* n=root; if (!n) return "(empty)";
    while (n->left) n=n->left;
    return n->airport.code;
}
std::string AirportBST::maximum() const {
    Node* n=root; if (!n) return "(empty)";
    while (n->right) n=n->right;
    return n->airport.code;
}
std::string AirportBST::rootCode() const { return root ? root->airport.code : "(empty)"; }
std::string AirportBST::caseCode(int kind) const {
    std::stack<Node*> work; if(root) work.push(root);
    while(!work.empty()) {
        Node* n=work.top(); work.pop();
        bool l=n->left != nullptr, r=n->right != nullptr;
        if ((kind==0 && !l && !r) || (kind==1 && l && !r) ||
            (kind==2 && !l && r) || (kind==3 && l && r && n != root)) return n->airport.code;
        if(n->right) work.push(n->right);
        if(n->left) work.push(n->left);
    }
    return "";
}
std::string AirportBST::deepestCode() const {
    std::queue<Node*> work; if(root) work.push(root); std::string last;
    while(!work.empty()) {
        Node* n=work.front(); work.pop(); last=n->airport.code;
        if(n->left) work.push(n->left);
        if(n->right) work.push(n->right);
    }
    return last;
}
AirportBST::Node* AirportBST::balanced(const std::vector<Airport>& a,int lo,int hi) {
    if(lo > hi) return nullptr;
    int mid=lo+(hi-lo)/2; Node* n=new Node(a[mid]);
    n->left=balanced(a,lo,mid-1); n->right=balanced(a,mid+1,hi); return n;
}
void AirportBST::rebuildBalanced(const std::vector<Airport>& sorted) {
    clear(); root=balanced(sorted,0,static_cast<int>(sorted.size())-1);
}
bool AirportBST::save(const std::string& filename) const {
    std::ofstream out(filename); if(!out) return false;
    auto codes=preorder(); int n=static_cast<int>(codes.size());
    for(int i=0;i<n;i++) out << codes[i] << '\n';
    return static_cast<bool>(out);
}
bool AirportBST::loadPreorder(const std::string& filename,
                              const std::vector<Airport>& catalog,std::string& error) {
    std::ifstream in(filename); if(!in) {error="Cannot open " + filename;return false;}
    std::vector<Airport> sorted=catalog;
    std::sort(sorted.begin(),sorted.end(),[](const Airport& a,const Airport& b){return a.code<b.code;});
    AirportBST temporary; std::string code;
    while(in >> code) {
        int lo=0,hi=static_cast<int>(sorted.size())-1;
        while(lo<=hi) {int mid=lo+(hi-lo)/2;if(sorted[mid].code<code)lo=mid+1;else hi=mid-1;}
        if(lo==static_cast<int>(sorted.size()) || sorted[lo].code!=code) {
            error="Code not present in catalog: " + code;return false;
        }
        if(!temporary.insert(sorted[lo])) {error="Duplicate saved code: " + code;return false;}
    }
    if(!in.eof()) {error="Error reading preorder file";return false;}
    clear(); root=temporary.root; temporary.root=nullptr; return true;
}
