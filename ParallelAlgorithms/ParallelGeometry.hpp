//
//  ParallelGeometry.hpp
//  ParallelAlgorithms
//
//  Created by Орест on 13.04.17.
//  Copyright © 2017 kupyn. All rights reserved.
//

#ifndef ParallelGeometry_hpp
#define ParallelGeometry_hpp

#include <stdio.h>
#include <tuple>
#include <vector>

#endif /* ParallelGeometry_hpp */

struct Point {
    int x;
    int y;
    
    friend bool operator < (const Point& lhs, const Point& rhs) {
        return lhs.x < rhs.x;
    }
    inline bool operator >= (const Point& rhs){ return !(*this < rhs); }
};

float closestPair(std::vector<Point> points);
