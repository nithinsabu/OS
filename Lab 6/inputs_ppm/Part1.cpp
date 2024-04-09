#include<iostream>
#include<fstream>
#include<vector>

using namespace std;
// int grayprogress = 0;
void grayscale(vector<vector<unsigned char>> &image, int width, int height){
    for (int i = 0; i<width*height; ++i){
        int grayscale = 0.299*image[i][0] + 0.587 *image[i][1] + 0.144 *image[i][2];
        image[i][0] = grayscale;
        image[i][1] = grayscale;
        image[i][2] = grayscale;
    }
}

void blur(vector<vector<unsigned char>> &image, int width, int height){
    int blur = 1;
    int n = blur*2+1;
    float kernel[n][n];
    // auto image = temp;
    for (int i = 0; i<n; ++i){
        for (int j = 0; j<n; ++j){
            kernel[i][j] = 1.0/(n*n);
        }
    }
    for (int i = 0; i<width*height; ++i){

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

    }
    // image = temp;
}

void redtint(vector<vector<unsigned char>> &image, int width, int height){
    for (int i = 0; i<width*height; ++i){
        image[i][0] = image[i][0] + (255-image[i][0])*0.5;
        // image[i][1] = image[i][1] - image[i][0]*0.25;
        // image[i][1] = image[i][1] - image[i][0]*0.25;

    }
}
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
    vector<vector<unsigned char>> image(height*width, vector<unsigned char>(3));

    for (int i = 0; i<width*height; ++i){
        ifile >> r >> g >> b;
        image[i][0] = r, image[i][1] = g; image[i][2] = b;
    }
    // grayscale
    ofstream ofile;
    ofile.open("output.ppm");
    ofile << "P3\n" << width << " " << height << "\n" << m << "\n";
    grayscale(image, width, height);
    
    //blurring
    blur(image, width, height);
    // redtint(image, width, height);

    for (int i = 0; i<width*height; ++i){
        ofile << (int)image[i][0]<<" " << (int)image[i][1]<< " " << (int)image[i][2]<< " ";
        if (i%5==4) ofile << "\n";
    }

}