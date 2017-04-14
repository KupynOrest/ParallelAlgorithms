//
//  ParallelSorting.cpp
//  ParallelAlgorithms
//
//  Created by Орест on 13.04.17.
//  Copyright © 2017 kupyn. All rights reserved.
//

#include "ParallelSorting.hpp"
#include <algorithm>
#include "tbb/parallel_invoke.h"
#include "tbb/parallel_for.h"

int binarySearch(int *arr, int l, int r, int x, int count, bool flag)
{
    if ((r - l) == 1) {
        bool f = flag ? x > arr[l] : x >= arr[l];
        return f ? count + 1 : count;
    } else if (r > l)
    {
        int mid = l + (r - l)/2;
        if (arr[mid] == x && !flag) {
            count++;
        }
        bool f = flag ? (arr[mid] > x) : (arr[mid] >= x);
        if (f) {
            return binarySearch(arr, l, mid, x, count, flag);
        }
        return binarySearch(arr, mid, r, x, count + (mid - l ), flag);
    }
    return count;
}


void binaryMerge(int *arr, int *mergeArray, int *secondArray, int position, int mergeSize, int secondSize, bool flag) {
    tbb::parallel_for(
                      tbb::blocked_range<size_t>(0,mergeSize),
                      [&](const tbb::blocked_range<size_t>& r) {
                          for(int i = 0; i < mergeSize; i++) {
                              int mergePosition = binarySearch(secondArray, 0, secondSize, mergeArray[i], 0,flag);
                              arr[position + mergePosition + i] = mergeArray[i];
                          }
                      }
                      );
}

void merge (int *arr, int left, int middle, int right) {
    int *leftTemp = new int[middle - left];
    memcpy(leftTemp, arr + left, (middle - left) * sizeof(int));
    int *rightTemp = new int[right - middle];
    memcpy(rightTemp, arr + middle, (right - middle) * sizeof(int));
    tbb::parallel_invoke([&] () { binaryMerge(arr, leftTemp, rightTemp, left, middle - left, right - middle, true); },
                         [&] () { binaryMerge(arr, rightTemp, leftTemp, left, right - middle, middle - left, false); });
}

void mergeSort(int *arr, int left, int right) {
    if (right - left > 1) {
        int middle = left + (right-left)/2;
        tbb::parallel_invoke([&] () { mergeSort(arr, left, middle); },
                             [&] () { mergeSort(arr, middle, right); });
        
        merge(arr, left, middle, right);
    }
}

void sort(int *arr, int n) {
    if (n > 1) {
        mergeSort(arr, 0, n);
    }
}
