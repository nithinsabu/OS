#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char* argv[]){
    char* newargv[argc];
    for (int i = 1; i<argc; ++i){
        newargv[i-1] = argv[i];
    }
    newargv[argc-1] = NULL;
    int output = atoi(argv[argc-1])*2;
    char outstr[100];
    sprintf(outstr, "%d", output);
    newargv[argc-2] = outstr;

    printf("Twice: Current process id: %d, Current result: %d\n", getpid(), output);
    execvp(newargv[0], newargv);
    return 0;
}