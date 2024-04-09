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
    ifstream f(argv[1]);
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
        // cout<<read<<endl;
    }   
    vector<bool> process_status(processes.size(), 0);
    // for (Process i: processes){
    //     cout<<"Pid: "<<i.pid<<" "<<i.arrival_time<<endl;
    //     for (int j: i.bursts){
    //         cout<<j<<" ";
    //     }cout<<endl;
    // }
    queue<Process> io_queue;
    queue<Process> cpu_queue;
    
    int current_time = processes[0].arrival_time;
    int total_processes = processes.size();
    int completed_processes = 0;
    
    for (auto i: processes){
        if (i.arrival_time==current_time){
            cpu_queue.push(i);
        }
    }
    int time_slice = 3;
    if (argc>2){
        time_slice = atoi(argv[2]);
    }
    unordered_map<int, int> m;
    while (!cpu_queue.empty() || !io_queue.empty()){
        int processing_time = 1;
        if (!cpu_queue.empty()){
            Process current_process = cpu_queue.front();
            cpu_queue.pop();
            processing_time = min(time_slice, current_process.bursts[0]);
            // cout<<current_process.pid<<" "<<current_time<<" "<<current_time+processing_time<<endl;
            for (Process i: processes){
                if (i.arrival_time>current_time && i.arrival_time<=current_time+processing_time){
                    cpu_queue.push(i);
                }
            }

            current_process.bursts[0]-=processing_time;
            if (current_process.bursts[0]==0){
                current_process.bursts.erase(current_process.bursts.begin());
                if (current_process.bursts.size()!=0) {
                    io_queue.push(current_process);
                    m[current_process.pid] = current_time+processing_time;
                }
                else{
                    for (auto& i: processes){
                        if (i.pid==current_process.pid){
                            i.completion_time = current_time+processing_time;
                                // cout<<current_process.pid<<" "<<i.completion_time<<endl;

                            break;
                        }
                    }
                    //process execution complete
                }
            }else{
                cpu_queue.push(current_process);
            }
        }
        //IO
        
            int temp_time = processing_time;
            while (temp_time>0 && !io_queue.empty()){
                Process& current_process = io_queue.front();
                int t = min(temp_time, current_process.bursts[0]);
                // cout << m[current_process.pid] << " "<< current_time+processing_time-temp_time<<endl;
                if(m[current_process.pid] <= current_time+processing_time-temp_time){
                    current_process.bursts[0]-= t;
                    if (current_process.bursts[0]==0){
                        current_process.bursts.erase(current_process.bursts.begin());
                        if (current_process.bursts.size()==0){
                            for (auto& i: processes){
                                if (i.pid==current_process.pid){
                                    i.completion_time = current_time+t;
                                    // cout<<current_process.pid<<" "<<i.completion_time<<endl;
                                    break;
                                }
                            }
                        //process execution complete

                        }
                        else{
                            cpu_queue.push(current_process);
                        }
                        io_queue.pop();
                    }
                }
                temp_time-=t;
        }
        current_time+=processing_time;
    }
    int avg_turnaround_time = 0, avg_waiting = 0, avg_penalty = 0;
    for (auto i: processes){
        cout<<"PID: "<<i.pid<<endl;
        cout<<"Completion time: "<<i.completion_time<<endl;
        cout<< "Turnaround time: "<< i.completion_time- i.arrival_time<<endl;
        avg_turnaround_time+=i.completion_time- i.arrival_time;
        cout<<"Waiting time: "<< (i.completion_time- i.arrival_time)-i.total_time << endl;
        avg_waiting+=(i.completion_time- i.arrival_time)-i.total_time;
        cout<<"Penalty Ratio: "<< (double)(i.completion_time- i.arrival_time)/i.total_time << endl;
        avg_penalty+=(double)(i.completion_time- i.arrival_time)/i.total_time ;
    }

    cout<<"Average turnaround time: "<<(double)avg_turnaround_time/processes.size()<<endl;
    cout<<"Average waiting time: "<<(double)avg_waiting/processes.size()<<endl;
    cout<<"Average penalty ratio: "<<(double)avg_penalty/processes.size()<<endl;
    
    return 0; 
}