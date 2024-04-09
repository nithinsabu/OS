#include<bits/stdc++.h>
using namespace std;

class Process{
    public:
    int pid;
    int arrival_time;
    int completion_time;
    int total_time = 0;
    vector<int> bursts;
    Process(int pid, int time): pid(pid), arrival_time(time){}
};

int main(int argc, char* argv[]){
    vector<Process> processes;
    ifstream f("process1.dat");
    string read;
    int temp_pid = 0;
    while (getline(f, read)){
        if (read[0]=='<') continue;
        int temp;
        istringstream iss(read);
        iss >> temp;
        Process newprocess(temp_pid, temp);
        while ((iss >> temp)){
            if (temp==-1) break;
            newprocess.bursts.push_back(temp);
            newprocess.total_time+=temp;
        }
        processes.push_back(newprocess);
        temp_pid++;
    }

    int total_processes = temp_pid;
    int completed_ex = 0;
    bool cpu_busy = false;
    int clock = 0;
    int time_slice = 3;
    queue<Process> cpu_queue;
    queue<Process> io_process;
    while (completed_ex!=total_processes){
        if (!cpu_busy){
            
        }
        clock++;
    }   


}