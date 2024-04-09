#include<iostream>
#include<vector>
#include<fstream>
#include <sstream>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include<cmath>
using namespace std;

#define SEM_NAME "/semLock"
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

class pixel{
public:
    int r, g, b;
    pixel(int x, int y, int z){
        r = x; g = y; b = z;
    }
};
void grayscale(int width, int height, int pid, key_t key){
    if (pid > 0) return;
    int shmid = shmget(key, sizeof( pixel) *(height) * width, 0666 | IPC_CREAT);
    pixel *values;
    values = ( pixel *)shmat(shmid, NULL, 0);
    sem_t *semL = sem_open(SEM_NAME, O_RDWR);
    for (int i = 0; i<width*height; i++){
        int grayscale = 0.299*values[i].r + 0.587 *values[i].g + 0.144 *values[i].b;
        values[i].r = grayscale;
        values[i].g = grayscale;
        values[i].b = grayscale;
    }
    sem_post(semL);
}

void blur(int width, int height, key_t key){
    int shmid = shmget(key, sizeof( pixel) *(height) * width, 0666 | IPC_CREAT);
    pixel *values;
    values = ( pixel *)shmat(shmid, NULL, 0);

    // sem_t *semL = sem_open(SEM_NAME, O_RDWR);
    int blur = 4;
    int n = blur*2+1;
    float kernel[n][n];

    for (int i = 0; i<n; ++i){
        for (int j = 0; j<n; ++j){
            kernel[i][j] = 1.0/(n*n);
        }
    }
    for (int i = 0; i<width*height; i++){
        float sumR = 0, sumG = 0, sumB = 0;
        int x1 = -blur;
        for (int x = i-blur*width; x<=i+width*blur; x+=width){
            for (int y = -blur; y<=blur; ++y){
                if (x+y>=0 && x+y<width*height){
                    sumR += kernel[blur+x1][blur+y] * values[x+y].r;
                    sumG += kernel[blur+x1][blur+y] * values[x+y].g; 
                    sumB += kernel[blur+x1][blur+y] * values[x+y].b; 
                }
            }
            x1++;
        }
        values[i].r = sumR;
        values[i].g = sumG;
        values[i].b = sumB;
    }
    // sem_post(semL);
}
int main(int argc, char *argv[]){
    int width, height, maxValue;

    char P_3[3];
    FILE *inputFile = fopen(argv[1], "r");
    fscanf(inputFile, "%s%d%d%d", P_3, &width, &height, &maxValue);

    key_t key = 0x1234;
    int shmid = shmget(key, sizeof( pixel) *(height) * width, 0666 | IPC_CREAT);

    pixel *values;
    values = ( pixel *)shmat(shmid, NULL, 0);
    int red, green, blue;
    for (int i = 0; i < height; i++){
        for (int j=0; j<width; j++){  
            fscanf(inputFile, "%d%d%d", &red, &green, &blue);
            values[(i * width) + j].r = red;
            values[(i * width) + j].g = green;
            values[(i * width) + j].b = blue;
        }
    }
    fclose(inputFile);
    // grayscale
    ofstream ofile;
    ofile.open(argv[2]);
    ofile << "P3\n" << width << " " << height << "\n" << maxValue << "\n";
    grayscale(width, height, fork(), key);
    //blurring
    blur(width, height, key);

    for (int i = 0; i<width*height; ++i){
        ofile << values[i].r<<" " << values[i].g<< " " << values[i].b<< " ";
        if (i%5==4) ofile << "\n";
    }
    // Detaching the shared memory.
    shmdt(values);
    shmctl(shmid,IPC_RMID,NULL);
    return 0;
}