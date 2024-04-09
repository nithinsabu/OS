#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    pid_t pid;
    char hello[] = "Hello World";
    srand(time(NULL));
    for (int i = 0; i < 11; i++) {
        printf("%c (PID: %d)\n", hello[i], getpid());
        int wait_time = rand()%4 + 1;
        sleep(wait_time);
        pid = fork();
        wait(NULL);
        if (pid>0){
            exit(0);
        }
    }
    exit(1);
    return 0;
}
