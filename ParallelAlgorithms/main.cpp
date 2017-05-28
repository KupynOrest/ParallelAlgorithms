//
//  main.cpp
//  ParallelAlgorithms
//
//  Created by Орест on 08.04.17.
//  Copyright © 2017 kupyn. All rights reserved.
//

#include <iostream>
#include <ctime>
#include "ParallelSorting.hpp"
#include "ParallelGeometry.hpp"
#include "PrefixSum.hpp"
#include "MapReduce.hpp"
#include "ParallelMatrixMultiplication.hpp"

int *generateRandomVector(int size, int min, int max) {
    int* result = new int[size];
    
    for (int i = 0; i < size; i++) {
        result[i] = min + (rand() % (int)(max - min + 1));
    }
    
    return result;
}

Point* generateRandomPoints(int size, int min, int max) {
    int* x = generateRandomVector(size, min, max);
    int* y = generateRandomVector(size, min, max);
    
    Point *result = new Point[size];
    for (int i = 0; i < size; i++) {
        result[i].x = x[i];
        result[i].y = y[i];
    }
    return result;

}

int main(int argc, const char * argv[]) {
    const int size = 2048;
    
    //Task 5
    int w = size/2;
    int h = size/2;
    int** a = new int*[w];
    int** b = new int*[w];
    for(int i = 0; i < h; ++i) {
        a[i] = generateRandomVector(h, 0, 9);
        b[i] = generateRandomVector(h, 0, 9);
    }
    
    Matrix<int> *A = new Matrix<int>(a,w,h);
    Matrix<int> *B = new Matrix<int>(b,w,h);
    
    clock_t begin = clock();
    
    Matrix<int> C = matrixMultiplyNaive(*A, *B);
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout<<"Naive Matrix multiply took - "<<elapsed_secs<<" seconds"<<endl;
    
    begin = clock();
    
    C = matrixMultiplyNaiveParallel(*A, *B);
    
    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout<<"Naive Parallel Matrix multiply took - "<<elapsed_secs<<" seconds"<<endl;
    
    begin = clock();
    
    C = matrixMultiplyTiled(*A, *B);
    
    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout<<"Tiled Parallel Matrix multiply took - "<<elapsed_secs<<" seconds"<<endl;
    
    
    C.print();
    
    return 0;
}
