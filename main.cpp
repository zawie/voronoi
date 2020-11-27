#include <iostream>

const int N = 512;
const int P = 100;

/**
 * Takes a double array and converts it to a .bmp image
 *
 * @param file_name the file name to save to
 * @param M an NxN matrix so save
 */
void save(char* file_name, double M[N][N]) {
    typedef unsigned char U8;
    typedef struct { U8 p[4]; } color;

    const int width = N;
    const int height = N;

    FILE* f = fopen(file_name, "wb");


    color tablo_color[256];
    for (int i = 0; i < 256; i++)
        tablo_color[i] = { (U8)i,(U8)i,(U8)i,(U8)255 };//BGRA 32 bit


    U8 pp[54] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0 ,
                  40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 32 };
    *(int*)(pp + 2) = 54 + 4 * width * height;  //file size
    *(int*)(pp + 18) = width;
    *(int*)(pp + 22) = height;
    *(int*)(pp + 42) = height * width * 4;      //bitmap size
    fwrite(pp, 1, 54, f);


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            U8 indis = (U8)((int)(M[i][j]*255));
            fwrite(tablo_color+indis,4,1,f);
        }
    }

    fclose(f);
}

/**
 * Prints out the contents of the passed matrix.
 *
 * @param M an NxN array
 */
void print(double M[N][N]) {
    for (int i = 0; i < N; i++) {
        if (i == 0){
            std::cout << "[";
        } else {
            std::cout << " ";
        }
        for (int j = 0; j < N; j++) {
            std::cout << M[i][j];
            if (j < N-1 || i < N-1) std::cout << ", ";
        }
        if (i < N-1) std::cout << "\n";
    }
    std::cout << "]" << std::endl;
}

/**
 * Sets all the values of the matrix to 0.0
 *
 * @param M an NxN array
 */
void clear(double M[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            M[i][j] = 0.0;
        }
    }
}


/**
 * Adds a voroni diagram to the passed matrix
 *
 * @param M
 * @param featurePointCount
 */
void voroniDiagram(double M[N][N], int points[P][2], double c0, double c1) {
    //Color the matrix
    double max = 1;
    double min = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            //Find nearest feature points
            double d0 = N*N;
            double d1 = N*N;
            for (int p=0; p < P; p++){
                double d = sqrt(pow(i - points[p][0],2) + pow(j - points[p][1],2));
                if (d < d0){
                    d1 = d0;
                    d0 = d;
                } else if(d < d1){
                    d1 = d;
                }
            }
            //Calculate height
            double h = c0*d0 + c1*d1;
            M[i][j] = h;
            //Adjust max and min
            if(h > max) max = h;
            if(h < min) min = h;
        }
    }
    //Scale to double between 0 and 1
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            M[i][j] = (M[i][j]-min)/(max-min);
        }
    }
}

void createPoints(int points[P][2]){
    for (int p=0; p < P; p++){
        points[p][0] = rand()%N;
        points[p][1] = rand()%N;
    }
}


int main() {
    //Create matrix
    double M[N][N];
    clear(M);
    //Create points
    int points[P][2];
    createPoints(points);
    //Set seed
    srand(time(0));
    //
    for (double i = -1; i <= 1; i+=1){
        for (double j = -1; j <= 1; j+=1){
            std::cout << "Creating Voroni Diagram" << "\n\tc0:" << i << "\n\tc1:" << j << std::endl;
            char buf[12];
            snprintf(buf, 12, "%d|%d.bmp", (int)i, (int) j); // puts string into buffer
            clear(M);
            voroniDiagram(M,points,i,j);
            save(buf,M);
        }
    }
    return 0;
}

