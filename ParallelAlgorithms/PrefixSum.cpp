//
//  PrefixSum.cpp
//  ParallelClosestPair
//
//  Created by Орест on 14.04.17.
//  Copyright © 2017 kupyn. All rights reserved.
//

#include "PrefixSum.hpp"
#include <math.h>
#include "tbb/parallel_for.h"

std::vector<int> sequentialPrefixSum(std::vector<int> input) {
    std::vector<int> output (input.size() + 1);
    output[0] = 0;
    
    for (int i = 1; i < input.size() + 1; i++) {
        output[i] = output[i - 1] + input[i - 1];
    }
    
    return output;
}

std::vector<int>parallelPrefixSum(std::vector<int> input) {
    std::vector<std::vector<int>> S(log2(input.size()) + 1,std::vector<int>(input.size()));
    S[0] = input;
    for (int d = 1; d < floor(log2(input.size())) + 1; d++) {
        tbb::parallel_for(
                          tbb::blocked_range<size_t>(0,(input.size() / pow(2,d))),
                          [&](const tbb::blocked_range<size_t>& r) {
                              for (int i = 0 ; i < (input.size() / pow(2,d)); i++) {
                                  S[d][i] = S[d-1][2*i] + S[d-1][2*i+1];
                              }
                          }
                          );
    }
    
    for (int d = floor(log2(input.size())) - 1; d >= 0; d--) {
        std::vector<int> upperLevel = S[d+1];
        tbb::parallel_for(size_t(1), size_t((input.size() / pow(2,d))),
                          [&] (size_t i) { S[d][i] = (i % 2 == 0) ? S[d][i] + upperLevel[i/2 - 1] : upperLevel[i/2]; });
    }
    
    S[0][0] = 0;
    return S[0];
}

std::vector<int> prefixSum(std::vector<int> input) {
    //return sequentialPrefixSum(input);
    return parallelPrefixSum(input);
}
