//
//  ParallelGeometry.cpp
//  ParallelAlgorithms
//
//  Created by Орест on 13.04.17.
//  Copyright © 2017 kupyn. All rights reserved.
//

#include "ParallelGeometry.hpp"
#include <math.h>
#include "tbb/parallel_invoke.h"
#include "tbb/parallel_for.h"

float distance(Point a, Point b) {
    return sqrt(pow((b.x - a.x),2) + pow((b.y - a.y),2));
}

float boundaryMerge(std::vector<Point> points, float leftDist, float rightDist, int median) {
    float dist = fmin(leftDist, rightDist);
    std::vector<Point> M (points.size());
    auto it = std::copy_if (points.begin(), points.end(), M.begin(), [&](Point i){return ((i.x >= median - dist) && (i.x <= median + dist));} );
    M.resize(std::distance(M.begin(),it));
    size_t size = M.size();
    
    tbb::parallel_for(
                      tbb::blocked_range<size_t>(0,size),
                      [&](const tbb::blocked_range<size_t>& r) {
                          for (int i = 0; i < size; ++i) {
                              for (int j = i + 1; j < size && (M[j].y - M[i].y) < dist; ++j) {
                                  if (distance(M[i],M[j]) < dist) {
                                      dist = distance(M[i],M[j]);
                                  }
                              }
                          }
                      }
                      );

    return dist;
}

std::vector<Point> paralellClosestPair(std::vector<Point> points, float & dist) {
    size_t size = points.size();
    
    if (size < 2) {
        dist = MAXFLOAT;
        return points;
    }
    std::nth_element (points.begin(), points.begin() + size / 2, points.end());
    
    std::vector<Point> left (points.begin(), points.begin() + points.size() / 2);
    std::vector<Point> right (points.begin() + points.size() / 2, points.end());

    float leftDist;
    float rightDist;
    std::vector<Point> leftResult(left.size());
    std::vector<Point> rightResult(right.size());
    tbb::parallel_invoke([&] () { leftResult = paralellClosestPair(left, leftDist); },
                         [&] () { rightResult = paralellClosestPair(right, rightDist); });
    
    
    std::vector<Point> mergedPoints (leftResult.size() + rightResult.size());
    std::merge (leftResult.begin(),leftResult.end(),rightResult.begin(),rightResult.end(),mergedPoints.begin(), [](Point l, Point r){ return l.y < r.y; });
    
    dist = boundaryMerge(mergedPoints, leftDist, rightDist, points[points.size() / 2].x);
    
    return mergedPoints;
}

float closestPair(std::vector<Point> points) {
    std::pair<Point, Point> result;
    
    float minDist;
    std::vector<Point> closestPair = paralellClosestPair(points, minDist);
    
    return minDist;
}
