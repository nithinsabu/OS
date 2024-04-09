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
void grayscale(vector<vector<unsigned char>> &image, int width, int height, int pipefds[2])
{
    close(pipefds[0]);
    for (int i = 0; i < width * height; ++i)
    {
        int grayscaleVal = 0.299 * image[i][0] + 0.587 * image[i][1] + 0.144 * image[i][2];
        image[i][0] = grayscaleVal;
        image[i][1] = grayscaleVal;
        image[i][2] = grayscaleVal;
        write(pipefds[1], &grayscaleVal, sizeof(grayscaleVal));
    }
    close(pipefds[1]);
}

void blur(vector<vector<unsigned char>> &image, int width, int height, int pipefds[2]){
    close(pipefds[1]);
    int blur = 4;
    int n = blur * 2 + 1;
    float kernel[n][n];
    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j){
            kernel[i][j] = 1.0 / (n * n);
        }
    }
    vector<vector<unsigned char>> newImage(height * width, vector<unsigned char>(3));
    for (int i = 0; i < width * height; ++i){
        int val;
        read(pipefds[0], &val, sizeof(val));
        newImage[i][0] = val, newImage[i][1] = val;
        newImage[i][2] = val;
    }
    for (int i = 0; i < width * height; ++i){
        float sumR = 0, sumG = 0, sumB = 0;
        int x1 = -blur;
        for (int x = i - blur * width; x <= i + width * blur; x += width){
            for (int y = -blur; y <= blur; ++y){
                if (x + y >= 0 && x + y < width * height){
                    sumG += kernel[blur + x1][blur + y] * newImage[x + y][1];
                    sumB += kernel[blur + x1][blur + y] * newImage[x + y][2];
                    sumR += kernel[blur + x1][blur + y] * newImage[x + y][0];
                }
            }x1++;
        }
        newImage[i][0] = sumR;
        newImage[i][1] = sumG;
        newImage[i][2] = sumB;
    }
    image = newImage;
    close(pipefds[0]);
}
int main(int argc, char* argv[])
{
    ifstream ifile;
    streampos pos;
    ifile.open(argv[1]);
    int width, height, r, g, b, m;
    string dum;

    getline(ifile, dum);
    pos = ifile.tellg();
    while (getline(ifile, dum))
    {
        if (dum[0] != '#')
            break;
        pos = ifile.tellg();
    }
    ifile.seekg(pos);
    ifile >> width >> height >> m;
    vector<vector<unsigned char>> image(height * width, vector<unsigned char>(3));

    for (int i = 0; i < width * height; ++i)
    {
        ifile >> r >> g >> b;
        image[i][0] = r, image[i][1] = g;
        image[i][2] = b;
    }
    
    int pipefds[2];
    if (pipe(pipefds) == -1) {
        perror("Pipe failed");
        exit(1);
    }
    int pid = fork();

    if (pid == 0)
    {
        grayscale(image, width, height, pipefds);
        exit(0);
    }
    else if (pid>0)
    {
        blur(image, width, height, pipefds);
        wait(NULL);
    }

    ofstream ofile;
    ofile.open(argv[2]);
    ofile << "P3\n"
          << width << " " << height << "\n"
          << m << "\n";
    for (int i = 0; i < width * height; ++i)
    {
        ofile << (int)image[i][0] << " " << (int)image[i][1] << " " << (int)image[i][2] << " ";
        if (i % 5 == 4)
            ofile << "\n";
    }
}