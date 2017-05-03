//
//  main.cpp
//  ParallelAlgorithms
//
//  Created by Орест on 08.04.17.
//  Copyright © 2017 kupyn. All rights reserved.
//

#include <iostream>
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
    //Task2
    int* arr1 = generateRandomVector(2048, 0, 10);
    
    sort(arr1,size);
    for (int i = 0; i < size; i++) {
        std::cout<<arr1[i]<<", ";
    }
    std::cout << "------------------------------------------" <<std::endl;
    std::cout << std::endl;
    
    
    //Task 4
    Point* P = generateRandomPoints(size, 1, 100);
    std::vector<Point> v(P, P + size);
    
    std::cout<<"The closest distance is - "<<closestPair(v)<<std::endl;
    std::cout << "------------------------------------------" <<std::endl;
    std::cout << std::endl;
    
    //Task 1
    int* arr2 = generateRandomVector(size, 0, 200);
    std::vector<int> vector(arr2, arr2 + size);
    
    std::vector<int> resultParallel = prefixSum(vector, true);
    std::vector<int> resultSeq = prefixSum(vector, false);
    
    int sum = 0;
    for (int i = 0; i < resultParallel.size(); i++) {
        if (i < 5) {
            std::cout<<"Vector - "<<vector[i]<<", Parallel Sum - "<<resultParallel[i]<<", Parallel Seq - "<<resultSeq[i]<<std::endl;
        }
        sum += resultParallel[i] - resultSeq[i];
    }
    
    std::cout<<" The sum is - "<<sum<<std::endl;
    
    
    for (int i = 0; i < resultParallel.size(); i++) {
        std::cout<<resultParallel[i]<<" "<<std::endl;
    }
    std::cout << "------------------------------------------" <<std::endl;
    std::cout << std::endl;
    
    //Task 3
    int* arr3 = generateRandomVector(size, 0, 9);
    std::vector<int> vector1(arr3, arr3 + size);
    
    MapReduce *rdd = new MapReduce();
    auto map = [](int i) { return  std::pair<int,int>(i,1);};
    auto reduce = [] (int a, int b){ return a + b;};
    std::map<int,int> res = rdd->mapReduce(vector1, map, reduce);
    
    std::cout << "------------------------------------------" <<std::endl;
    std::cout << std::endl;
    
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
    Matrix<int> C = *A * *B;
    C.print();
    
    return 0;
}
