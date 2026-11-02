#include "AirportBST.h"
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <set>
#include <sstream>
#include <stdexcept>
using namespace std;
void require(bool ok,const string& message) {if(!ok) throw runtime_error("TEST FAILED: "+message);}
void insertAll(AirportBST& t,const vector<Airport>& a,bool check=false) {
    int n=static_cast<int>(a.size());
    for(int i=0;i<n;i++) {require(t.insert(a[i]),"duplicate "+a[i].code);if(check)require(t.validate(),"insert validation");}
}
void printMetrics(const AirportBST& t) {
    auto m=t.metrics();cout<<"Count: "<<m.count<<" Height: "<<m.height<<" Average depth: "<<fixed<<setprecision(6)<<m.averageDepth<<'\n';
}
void printCodes(const vector<string>& a) {int n=static_cast<int>(a.size());for(int i=0;i<n;i++)cout<<(i?" ":"")<<a[i];cout<<'\n';}
void printSearch(const AirportBST& t,const string& key) {
    auto s=t.find(key);int n=static_cast<int>(s.path.size());cout<<"find "<<key<<"\nPath: ";
    for(int i=0;i<n;i++)cout<<(i?" -> ":"")<<s.path[i];
    if(!s.airport)cout<<(n?" -> ":"")<<"(NULL)";
    cout<<" (comparisons: "<<n<<")\n";
    if(s.airport)printAirport(*s.airport,cout);else cout<<"Not found\n";
}
Airport fake(const string& key) {
    Airport a;a.code=key;a.name="Somber Xie Test Airport";a.city="Troy";a.country="United States";a.icao="TEST";a.latitude=42.7284;a.longitude=-73.6918;return a;
}
void syntheticTests() {
    auto f=splitCSV("1,\"Airport, \"\"North\"\"\",Town,Country,AAA,TEST,1,2");
    require(f.size()==8 && f[1]=="Airport, \"North\"","quoted CSV");
    vector<vector<string>> builds={{},{"M"},{"M","F"},{"M","T"},{"M","F","T","W"},{"M","F","T","P","R"},{"M","F","T","C","H","P","W"}};
    for(int i=0;i<static_cast<int>(builds.size());i++) {
        AirportBST t;set<string> expected;int n=static_cast<int>(builds[i].size());
        for(int j=0;j<n;j++){require(t.insert(fake(builds[i][j])),"insert");expected.insert(builds[i][j]);require(t.validate(),"insert validation");}
        if(n)require(!t.insert(fake(builds[i][0])),"duplicate rejection");
        string detail;require(!t.remove("ABSENT",detail)&&t.validate(),"missing deletion");
        for(int j=0;j<n;j++) {
            require(t.remove(builds[i][j],detail),"delete");expected.erase(builds[i][j]);require(t.validate(),"delete validation");
            auto a=t.inorder();require(a.size()==expected.size(),"count");int k=0;
            for(auto it=expected.begin();it!=expected.end();++it){require(a[k].code==*it,"contents");k++;}
        }
        require(t.metrics().height==-1 && !t.remove("ANY",detail),"empty deletion");
    }
    AirportBST randomTree; set<string> oracle; mt19937 rng(7250);
    for(int i=0;i<1000;i++) {
        string key="K"+to_string(100+static_cast<int>(rng()%100));
        if(rng()%2) require(randomTree.insert(fake(key))==oracle.insert(key).second,"random insert");
        else {string detail;bool expected=oracle.erase(key)!=0;require(randomTree.remove(key,detail)==expected,"random remove");}
        require(randomTree.validate(),"random validation");auto actual=randomTree.inorder();
        require(actual.size()==oracle.size(),"random count");int k=0;
        for(auto it=oracle.begin();it!=oracle.end();++it){require(actual[k].code==*it,"random contents");k++;}
    }
    cout<<"1000 randomized mutations against std::set oracle: PASS (test oracle only).\n";
    cout<<"Synthetic tests: PASS (empty, root 0/1/2 children, leaf, both one-child\ndirections, successor links/right child, duplicates, quoted CSV).\n";
}
void verify(const string& filename) {
    syntheticTests();auto loaded=readAirports(filename);auto records=loaded.airports;int n=static_cast<int>(records.size());
    require(loaded.lines==7698 && loaded.skipped==1626 && n==6072,"loader counts");
    AirportBST t;insertAll(t,records,true);require(!t.insert(records[0])&&t.validate(),"real duplicate");
    set<string> countries;int us=0;
    for(int i=0;i<n;i++){countries.insert(records[i].country);if(records[i].country=="United States")us++;}
    require(countries.size()==235 && us==1251,"countries");
    cout<<"Loaded "<<n<<" airports; "<<loaded.skipped<<" skipped; 0 data duplicates; countries="<<countries.size()<<"; US="<<us<<'\n';
    printMetrics(t);require(t.metrics().height==177,"file-order height");
    cout<<"Minimum: "<<t.minimum()<<" Maximum: "<<t.maximum()<<"\n\nTASK 2: SEARCH CASES\n";
    vector<string> queries={t.rootCode(),t.caseCode(0),t.deepestCode(),"XYZ","!","~"};
    for(int i=0;i<static_cast<int>(queries.size());i++)printSearch(t,queries[i]);
    require(!t.findFast("XYZ")&&!t.findFast("!")&&!t.findFast("~")&&t.findFast(t.deepestCode()),"search cases");
    cout<<"\nTASK 3: INDEPENDENT DELETE TESTS (each starts in file order)\n";
    for(int i=0;i<5;i++) {
        AirportBST trial;insertAll(trial,records,true);string key=i==4?trial.rootCode():trial.caseCode(i);require(!key.empty(),"case exists");
        cout<<"Case "<<i+1<<": remove "<<key<<"\nTop 3 before: ";printCodes(trial.levelOrder(3));string detail;
        require(trial.remove(key,detail)&&trial.validate(),"real delete");require(!trial.findFast(key)&&trial.metrics().count==6071,"removed record");
        for(int j=0;j<n;j++) if(records[j].code!=key) {
            const Airport* found=trial.findFast(records[j].code);
            require(found && found->name==records[j].name && found->city==records[j].city &&
                found->country==records[j].country && found->icao==records[j].icao &&
                found->latitude==records[j].latitude && found->longitude==records[j].longitude,"full record preservation");
        }
        cout<<detail<<"\nTop 3 after:  ";printCodes(trial.levelOrder(3));cout<<"validate: OK; remaining: 6071\n";
    }
    require(!t.findFast("QQQ")&&t.insert(fake("QQQ"))&&t.validate(),"QQQ insert");
    cout<<"\nMade-up airport inserted; validate: OK\n";printSearch(t,"QQQ");string detail;
    require(t.remove("QQQ",detail)&&t.validate()&&!t.findFast("QQQ"),"QQQ deletion");cout<<"QQQ removed; validate: OK\n";
    cout<<"\nTASK 4: TRAVERSALS AND VALIDATION\n";auto a=t.inorder();
    for(int i=1;i<n;i++)require(a[i-1].code<a[i].code,"sorted inorder");
    cout<<"Inorder: 6072 records; strictly sorted: YES\n";require(t.save("docs/preorder.txt"),"save");
    AirportBST reloaded;string error;require(reloaded.loadPreorder("docs/preorder.txt",records,error),"reload: "+error);
    require(reloaded.validate()&&t.preorder()==reloaded.preorder()&&t.levelOrder()==reloaded.levelOrder()&&t.metrics().height==reloaded.metrics().height,"same shape");
    cout<<"Preorder reload: same preorder, full level order, height (177): YES\n";
    {ofstream bad("docs/invalid_preorder.txt");bad<<"NOT_IN_CATALOG\n";}
    auto before=reloaded.preorder();require(!reloaded.loadPreorder("docs/invalid_preorder.txt",records,error)&&before==reloaded.preorder(),"atomic failed reload");
    std::remove("docs/invalid_preorder.txt");cout<<"Invalid saved code rejected without changing tree\nStrict inherited bounds validated after every test insertion/deletion.\n";
    cout<<"\nTASK 5: RANGE\n";auto r=t.range("BOS","BWI");require(r.airports.size()==113,"range count");int k=0;
    for(int i=0;i<n;i++)if(a[i].code>="BOS"&&a[i].code<="BWI"){require(r.airports[k].code==a[i].code,"range contents");k++;}
    cout<<"range BOS BWI: "<<r.airports.size()<<" airports, "<<r.visited<<" nodes visited\nFirst: "<<r.airports.front().code<<" Last: "<<r.airports.back().code<<'\n';
    require(t.range("ZZZ","AAA").airports.empty()&&t.range("BOS","BOS").airports.size()==1&&t.range("!","~").airports.size()==6072&&t.range("!","!!").airports.empty(),"range edge cases");
    auto csv=readAirports("data/airports.csv");require(csv.airports.size()==records.size(),"CSV size");
    for(int i=0;i<n;i++)require(csv.airports[i].code==records[i].code&&csv.airports[i].name==records[i].name,"CSV records");
    cout<<"DAT/CSV records agree; singleton/empty/reversed/full ranges: PASS\nALL TESTS PASSED\n";
}
void experiment(const string& filename) {
    auto records=readAirports(filename).airports;auto sorted=records;
    sort(sorted.begin(),sorted.end(),[](const Airport& a,const Airport& b){return a.code<b.code;});
    vector<string> queries;mt19937 generator(2560);uniform_int_distribution<int> choose(0,static_cast<int>(records.size())-1);
    for(int i=0;i<10000;i++)queries.push_back(records[choose(generator)].code);
    ofstream csv("docs/experiment.csv");csv<<"build,count,height,average_depth,searches,total_comparisons,time_ms,checksum\n";
    cout<<"10,000 successful random searches with replacement; seed=2560; same queries for all trees.\nTimer: steady_clock; excludes build, validation, output and path collection.\n";
    for(int i=0;i<4;i++) {
        AirportBST t;string name;
        if(i==0){name="File order";insertAll(t,records);}
        if(i==1){name="Sorted order";insertAll(t,sorted);}
        if(i==2){name="Balanced";t.rebuildBalanced(sorted);}
        if(i==3){name="Random shuffle";auto shuffled=records;mt19937 rng(2560);shuffle(shuffled.begin(),shuffled.end(),rng);insertAll(t,shuffled);}
        require(t.validate(),"experiment validation");auto m=t.metrics();
        if(i==1)require(m.height==6071,"sorted height");
        if(i==2)require(m.height==12,"balanced height");
        long long total=0,checksum=0;auto start=chrono::steady_clock::now();
        for(int j=0;j<10000;j++){int comparisons=0;const Airport* a=t.findFast(queries[j],&comparisons);if(a)checksum+=static_cast<unsigned char>(a->code[0]);total+=comparisons;}
        double ms=chrono::duration<double,milli>(chrono::steady_clock::now()-start).count();
        cout<<name<<": n="<<m.count<<" height="<<m.height<<" average_depth="<<fixed<<setprecision(6)<<m.averageDepth<<" time_ms="<<ms<<" comparisons="<<total<<" checksum="<<checksum<<'\n';
        csv<<name<<','<<m.count<<','<<m.height<<','<<fixed<<setprecision(6)<<m.averageDepth<<",10000,"<<total<<','<<ms<<','<<checksum<<'\n';
    }
}
void interactive() {
    AirportBST tree;vector<Airport> catalog;string line;cout<<"Airport BST - type help for commands\n";
    while(cout<<"> " && getline(cin,line)) {
        istringstream input(line);string cmd;input>>cmd;
        try {
            if(cmd=="quit"||cmd=="exit")break;
            if(cmd=="help")cout<<"load FILE | find CODE | remove CODE | add CODE \"name\" \"city\"\nlist | save FILE | validate | range FROM TO | minmax | stats | top | rebalance | quit\nData: .dat/.csv. Saved codes: .txt (load master data first).\n";
            else if(cmd=="load") {
                string file;input>>file;
                if(file.size()>=4 && (file.substr(file.size()-4)==".dat"||file.substr(file.size()-4)==".csv")) {
                    auto loaded=readAirports(file);tree.clear();catalog=loaded.airports;int duplicates=0,n=static_cast<int>(catalog.size());
                    for(int i=0;i<n;i++)if(!tree.insert(catalog[i])){duplicates++;cout<<"Duplicate rejected (I4): "<<catalog[i].code<<'\n';}
                    cout<<"Loaded "<<tree.metrics().count<<" airports ("<<loaded.skipped<<" lines skipped: no IATA code). Duplicates: "<<duplicates<<'\n';
                } else {string error;if(!tree.loadPreorder(file,catalog,error))throw runtime_error(error);cout<<"Preorder tree loaded\n";}
                printMetrics(tree);cout<<"validate: "<<(tree.validate()?"OK":"FAIL")<<'\n';
            } else if(cmd=="find"){string key;input>>key;printSearch(tree,key);}
            else if(cmd=="remove") {string key,detail;input>>key;bool ok=tree.remove(key,detail);cout<<(ok?"Deleted ":"Cannot delete ")<<key<<" ("<<detail<<")\nvalidate: "<<(tree.validate()?"OK":"FAIL")<<'\n';}
            else if(cmd=="add") {
                string key,name,city;input>>key>>quoted(name)>>quoted(city);if(key.empty()||name.empty()||city.empty())throw runtime_error("Use add CODE \"name\" \"city\"");
                Airport a=fake(key);a.name=name;a.city=city;
                if(tree.insert(a)){bool known=false;int n=static_cast<int>(catalog.size());for(int i=0;i<n;i++)if(catalog[i].code==key){catalog[i]=a;known=true;break;}if(!known)catalog.push_back(a);cout<<"Added "<<key<<'\n';}
                else {cout<<"Duplicate rejected (I4): "<<key<<'\n';}
                cout<<"validate: "<<(tree.validate()?"OK":"FAIL")<<'\n';
            } else if(cmd=="list") {auto a=tree.inorder();int n=static_cast<int>(a.size());for(int i=0;i<n;i++)printAirport(a[i],cout);cout<<n<<" airports\n";}
            else if(cmd=="save") {string file;input>>file;if(!tree.save(file))throw runtime_error("Save failed");cout<<"Saved preorder codes to "<<file<<'\n';}
            else if(cmd=="validate")cout<<"validate: "<<(tree.validate()?"OK":"FAIL")<<'\n';
            else if(cmd=="range") {string lo,hi;input>>lo>>hi;auto r=tree.range(lo,hi);int n=static_cast<int>(r.airports.size());for(int i=0;i<n;i++)printAirport(r.airports[i],cout);cout<<n<<" airports, "<<r.visited<<" nodes visited\n";}
            else if(cmd=="minmax")cout<<"Minimum: "<<tree.minimum()<<" Maximum: "<<tree.maximum()<<'\n';
            else if(cmd=="stats")printMetrics(tree);
            else if(cmd=="top")printCodes(tree.levelOrder(3));
            else if(cmd=="rebalance"){tree.rebuildBalanced(tree.inorder());printMetrics(tree);cout<<"validate: "<<(tree.validate()?"OK":"FAIL")<<'\n';}
            else if(!cmd.empty())cout<<"Unknown command; type help\n";
        } catch(const exception& e){cout<<"Error: "<<e.what()<<'\n';}
    }
}
int main(int argc,char* argv[]) {
    try {
        if(argc>1&&string(argv[1])=="--test")verify(argc>2?argv[2]:"data/airports.dat");
        else if(argc>1&&string(argv[1])=="--experiment")experiment(argc>2?argv[2]:"data/airports.dat");
        else interactive();
    } catch(const exception& e){cerr<<e.what()<<'\n';return 1;}
    return 0;
}
