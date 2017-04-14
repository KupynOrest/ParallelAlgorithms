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

int main(int argc, const char * argv[]) {
    
    //Task2
    int  arr1[] = { 1,2,3,4,5,1,2,3,4,5 };
    
    sort(arr1,10);
    for (int i = 0; i < 10; i++) {
        std::cout<<arr1[i]<<", ";
    }
    std::cout << std::endl;
    
    
    //Task 4
    Point P[] = {{2, 3}, {12, 30}, {40, 50}, {5, 1}, {12, 10}, {3, 4}};
    std::vector<Point> v(std::begin(P), std::end(P));
    
    std::cout<<"The closest distance is - "<<closestPair(v)<<std::endl;
    
    //Task 1
    std::vector<int> vector = {7, 0, 2, 9, 5, 1, 8, 6};
    
    std::vector<int> result = prefixSum(vector);
    
    for (int i = 0; i < result.size(); i++) {
        std::cout<<result[i]<<" "<<std::endl;
    }
    
    //Task 3
    std::vector<int> vector1 (1024);
    
    for (int i = 0; i < 1024; i++) {
        int max = 9;
        int min = 0;
        vector1[i] = min + (rand() % (int)(max - min + 1));
    }
    
    MapReduce *rdd = new MapReduce();
    auto map = [](int i) { return  std::pair<int,int>(i,1);};
    auto reduce = [] (int a, int b){ return a + b;};
    std::map<int,int> res = rdd->mapReduce(vector1, map, reduce);
    
    
    //Task 5
    int w = 5;
    int h = 5;
    int** a = new int*[w];
    int** b = new int*[w];
    for(int i = 0; i < h; ++i) {
        a[i] = new int[h];
        b[i] = new int[h];
    }
    
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            int max = 9;
            int min = 1;
            a[i][j] = min + (rand() % (int)(max - min + 1));
            b[i][j] = min + (rand() % (int)(max - min + 1));
        }
    }
    Matrix<int> *A = new Matrix<int>(a,w,h);
    Matrix<int> *B = new Matrix<int>(b,w,h);
    A->print();
    B->print();
    Matrix<int> C = *A * *B;
    C.print();
    
    return 0;
}
