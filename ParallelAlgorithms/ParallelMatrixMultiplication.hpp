//
//  ParallelMatrixMultiplication.hpp
//  ParallelAlgorithms
//
//  Created by Орест on 14.04.17.
//  Copyright © 2017 kupyn. All rights reserved.
//

#ifndef ParallelMatrixMultiplication_hpp
#define ParallelMatrixMultiplication_hpp

#include <stdio.h>
#include <iostream>
#include "tbb/parallel_for.h"

#endif /* ParallelMatrixMultiplication_hpp */

template <typename T>
class Matrix {
public:
    Matrix(T **d, int width, int height) : data(d), w(width), h(height) {}
    friend Matrix operator * (Matrix& lhs, Matrix& rhs) {
        T** result = new T*[lhs.getW()];
        for(int i = 0; i < lhs.getW(); ++i)
            result[i] = new T[rhs.getH()];
        
        tbb::parallel_for(size_t(0), size_t((lhs.getW())),
                          [&] (size_t i) {
                              tbb::parallel_for(size_t(0), size_t((rhs.getH())),
                                                [&] (size_t j) {
                                                    for (int k = 0; k < lhs.getH(); k++) {
                                                        result[i][j] += lhs.getElem(i,k) * rhs.getElem(k,j);
                                                    }
                                                });
                          });

        return *new Matrix(result, lhs.getW(), rhs.getH());
    }
    
    int getW() {return w;}
    int getH() {return h;}
    T getElem(int row, int col) {return data[row][col];}
    
    void print() {
        std::cout<<"---------------------------------------"<<std::endl;
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                std::cout<<data[i][j]<<" ";
            }
            std::cout<<std::endl;
        }
        std::cout<<"---------------------------------------"<<std::endl;
    }
private:
    int w;
    int h;
    T **data;
};
