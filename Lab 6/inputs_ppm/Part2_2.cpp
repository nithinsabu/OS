#include<iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <unistd.h> //for fork()
#include <fcntl.h>
#include <fstream>

// #define SEM_NAME "/s"
// #define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
// #define INITIAL_VALUE 0
// sem_t s;

using namespace std;

int main(){
    ifstream ifile;
    streampos pos;
    ifile.open("sample_pp3_5mb.ppm");
    int width, height, r, g, b, m;
    string dum;

    getline(ifile, dum);
    pos = ifile.tellg();
    while (getline(ifile, dum)){
        if (dum[0]!='#') break;
        pos = ifile.tellg();
    }
    ifile.seekg(pos);
    ifile >> width >> height >> m;
    // cout<<width<<" "<<height<<" "<<m;


    key_t key = 0x1234;
    int shmid = shmget(key, sizeof(int)*3* (height) * width, 0666 | IPC_CREAT);
    int *values = (int *)shmat(shmid, NULL, 0);
    // sem_t *s = sem_open(SEM_NAME, O_CREAT, SEM_PERMS, INITIAL_VALUE);
    // sem_init(&s, 0, 1);


    values[0] = 1;
    int pid = fork();
    if (pid>0){
        
        for (int i = 0; i<1000000; ++i){
            values[0]++;
            cout<<i<<endl;
        }
        wait(NULL);
    }
    else if (pid==0){

        for (int i = 0; i<1000000; ++i){
            values[0]++;
            cout<<"AAA"<<endl;
        }

        exit(0);
    }
    cout<<values[0];
}