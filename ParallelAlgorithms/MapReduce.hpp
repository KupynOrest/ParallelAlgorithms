//
//  MapReduce.hpp
//  ParallelAlgorithms
//
//  Created by Орест on 14.04.17.
//  Copyright © 2017 kupyn. All rights reserved.
//

#ifndef MapReduce_hpp
#define MapReduce_hpp

#include <stdio.h>
#include <map>
#include <vector>
#include <algorithm>
#include <numeric>
#include "tbb/parallel_for_each.h"
#include "tbb/mutex.h"


#endif /* MapReduce_hpp */

using namespace std;
class MapReduce {
public:
    tbb::mutex   countMutex;
    template <typename T,typename unop, typename binop>
    map<T, int> mapReduce(vector<T> input,unop mapOp, binop reduceOp) {
        multimap<T, int> intermidiate = map(input, mapOp);
        
        std::map<T, std::vector<int>> reducer;
        
        for_each(intermidiate.begin(), intermidiate.end(),
                 [&reducer](pair<T,int> element) {
                     reducer[element.first].push_back(element.second);
                 }
                 );
        
        return reduce(reducer, reduceOp);
    }
private:
    template <typename T,typename unop>
    std::multimap<T,int> map(vector<T> input,unop mapOp) {
        multimap<T, int> result;
        tbb::parallel_for_each(input.begin(),input.end(),[&](T i) {
            pair<T,int> pair = mapOp(i);
            tbb::mutex::scoped_lock lock(countMutex);
            result.insert(pair);});
        return result;
    }
    
    template <typename T,typename binop>
    std::map<T,int> reduce(std::map<T, std::vector<int>> input, binop reduceOp) {
        std::map<T, int> result;
        
        tbb::parallel_for_each(input.begin(),input.end(),[&](pair<T, vector<int>> element) {
            result[element.first] =  accumulate(element.second.begin(), element.second.end(), 0,reduceOp);
        });
        
        return result;
    }
    
};
