#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h> 
#include <queue>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <unordered_set>
#include <time.h>
using namespace std;

void fifo(vector<int> &arr, int pfCount){
    int hit = 0;
    vector<int> q;
    for(int page: arr){
        if(find(q.begin(), q.end(), page) != q.end()){
            hit++;
        } else {
            if(q.size() < pfCount){
                q.push_back(page);
            } else {
                q.erase(q.begin());
                q.push_back(page);
            }
        }
    }
    cout <<"Page fault percentage of FIFO: " << setprecision(5) << (arr.size()-hit)*100.0/arr.size() << "\n";

}
void lru(vector<int> &arr, int pfCount){
    int hit = 0;
    vector<int> q;
    for(int page: arr){
        auto iter = find(q.begin(), q.end(), page);
        if(iter != q.end()){
            hit++;
            q.erase(iter);
        } else {
            if(q.size() >= pfCount){
                q.erase(q.begin());
            }
        }
        q.push_back(page);
    }
    cout <<"Page fault percentage of LRU: " << setprecision(5) << (arr.size()-hit)*100.0/arr.size() << "\n";

}
void random(vector<int> &arr, int pfCount){
    int hit = 0;
    vector<int> q;
    for(int page: arr){
        auto iter = find(q.begin(), q.end(), page);
        if(iter != q.end()){
            hit++;
        } else {
            if(q.size() >= pfCount){
                q.erase(q.begin()+(rand()%pfCount));
            }
            q.push_back(page);
        }
    }
    cout <<"Page fault percentage of random: " << setprecision(5) << (arr.size()-hit)*100.0/arr.size() << "\n";
}
int main(int argc, char* argv[]){
    if(argc < 5){
        cout << "Not enough arguements\n";
        cout << "Usage : "  << argv[0] <<" <addressable pages> <main memory> <disk blocks> <filename>\n";
        exit(1);
    }
    srand(time(0));
    int vpCount = atoi(argv[1]);
    int pfCount = atoi(argv[2]);
    int sspCount = atoi(argv[3]);
    ifstream file(argv[4]);
    if(!file.is_open()){
        cout << "No such file exists" << "\n";
    }
    string line;
    vector<int> arr;
    unordered_set<int> st;
    while (getline(file, line)) {
        int page;
        istringstream iss(line);
        while ((iss >> page)){
            arr.push_back(page);
            st.insert(page);
        }
    }
    if(st.size() > pfCount + sspCount){
        cout << "Require more block in swap space than it is available.\n";
        exit(1);
    }

    if(st.size() > vpCount){
        cout << "Page is out of limit.\n";
        exit(1);
    }
    fifo(arr, pfCount);
    lru(arr, pfCount);
    random(arr, pfCount);
}