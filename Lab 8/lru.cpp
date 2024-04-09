#include<iostream>
#include<fstream>
#include<vector>
#include<unordered_set>

using namespace std;

int main(int argc, char* argv[]){
    ifstream inp;
    inp.open("req1.dat");
    unordered_set<int> pages;
    vector<int> sequence;
    int req;
    while (inp >> req){
        pages.insert(req);
        sequence.push_back(req);
    }
    cout<< pages.size();
    cout<<" "<<sequence.size();
}