#include<iostream>
#include<fstream>
#include<vector>
#include<semaphore.h>
#include <thread>

using namespace std;

sem_t sem;
void grayscale(vector<vector<unsigned char>> &image, int width, int height){
    for (int i = 0; i<width*height; ++i){
        sem_wait(&sem);
        int grayscale = 0.299*image[i][0] + 0.587 *image[i][1] + 0.144 *image[i][2];
        image[i][0] = grayscale;
        image[i][1] = grayscale;
        image[i][2] = grayscale;
        sem_post(&sem);
    }
}

void blur(vector<vector<unsigned char>> &image, int width, int height){
    int blur = 4;
    int n = blur*2+1;
    float kernel[n][n];

    for (int i = 0; i<n; ++i){
        for (int j = 0; j<n; ++j){
            kernel[i][j] = 1.0/(n*n);
        }
    }
    for (int i = 0; i<width*height; ++i){

        sem_wait(&sem);
        float sumR = 0, sumG = 0, sumB = 0;
        int x1 = -blur;
        for (int x = i-blur*width; x<=i+width*blur; x+=width){
            for (int y = -blur; y<=blur; ++y){
                if (x+y>=0 && x+y<width*height){
                    sumG += kernel[blur+x1][blur+y] * image[x+y][1]; 
                    sumB += kernel[blur+x1][blur+y] * image[x+y][2]; 
                    sumR += kernel[blur+x1][blur+y] * image[x+y][0];
                }
            }
            x1++;
        }
        image[i][0] = sumR;
        image[i][1] = sumG;
        image[i][2] = sumB;
        sem_post(&sem);
    }
}
int main(int argc, char* argv[]){
    ifstream ifile;
    streampos pos;
    ifile.open(argv[1]);
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
    vector<vector<unsigned char>> image(height*width, vector<unsigned char>(3));

    for (int i = 0; i<width*height; ++i){
        ifile >> r >> g >> b;
        image[i][0] = r, image[i][1] = g; image[i][2] = b;
    }
    ofstream ofile;
    ofile.open(argv[2]);
    ofile << "P3\n" << width << " " << height << "\n" << m << "\n";
    
    sem_post(&sem);
    thread T1(grayscale, std::ref(image), width, height);
    thread T2(blur, std::ref(image), width, height);
    T1.join();
    T2.join();
    for (int i = 0; i<width*height; ++i){
        ofile << (int)image[i][0]<<" " << (int)image[i][1]<< " " << (int)image[i][2]<< " ";
        if (i%5==4) ofile << "\n";
    }

}