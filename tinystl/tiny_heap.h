#pragma once

#include "tiny_iterator.h"
#include <iso646.h>

namespace Tiny
{

// function push_heap

template <typename RandomAccessIterator, typename Distance, typename T>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value)
{
    Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex and first[parent] < value)
    {
        first[holeIndex] = first[parent];
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    first[holeIndex] = value;
}

template <typename RandomAccessIterator, typename Distance, typename T>
void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*)
{
    __push_heap(first, Distance(last - first - 1), Distance(0), T(*(last - 1)));
}

template <typename RandomAccessIterator>
void push_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    __push_heap_aux(first, last, distance_type(first), value_type(first));
}

template <typename RandomAccessIterator, typename Compare, typename Distance, typename T>
void __push_heap(RandomAccessIterator first, const Compare& comp, Distance holeIndex, Distance topIndex, T value)
{
    Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex and comp(first[parent], value))
    {
        first[holeIndex] = first[parent];
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    first[holeIndex] = value;
}

template <typename RandomAccessIterator, typename Compare, typename Distance, typename T>
void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, const Compare& comp, Distance*, T*)
{
    __push_heap(first, comp, Distance(last - first - 1), Distance(0), T(*(last - 1)));
}

template <typename RandomAccessIterator, typename Compare>
void push_heap(RandomAccessIterator first, RandomAccessIterator last, const Compare& comp)
{
    __push_heap_aux(first, last, comp, distance_type(first), value_type(first));
}

// function pop_heap

template <typename RandomAccessIterator, typename Distance, typename T>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value)
{
    Distance topIndex = holeIndex;
    Distance secondChild = holeIndex * 2 + 2;
    while (secondChild < len)
    {
        if (first[secondChild] < first[secondChild - 1])
            secondChild--;
        first[holeIndex] = first[secondChild];
        holeIndex = secondChild;
        secondChild = holeIndex * 2 + 2;
    }
    if (secondChild == len)
    {
        first[holeIndex] = first[secondChild - 1];
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, value);
}

template <typename RandomAccessIterator, typename Compare, typename Distance, typename T>
void __adjust_heap(RandomAccessIterator first, const Compare& comp, Distance holeIndex, Distance len, T value)
{
    Distance topIndex = holeIndex;
    Distance secondChild = holeIndex * 2 + 2;
    while (secondChild < len)
    {
        if (comp(first[secondChild], first[secondChild - 1]))
            secondChild--;
        first[holeIndex] = first[secondChild];
        holeIndex = secondChild;
        secondChild = holeIndex * 2 + 2;
    }
    if (secondChild == len)
    {
        first[holeIndex] = first[secondChild - 1];
        holeIndex = secondChild - 1;
    }
    __push_heap(first, comp, holeIndex, topIndex, value);
}

template <typename RandomAccessIterator, typename T, typename Distance>
void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, 
                RandomAccessIterator result, T value, Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), value);
}

template <typename RandomAccessIterator, typename T>
void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*)
{
    __pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));
}

template <typename RandomAccessIterator>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    __pop_heap_aux(first, last, value_type(first));
}

template <typename RandomAccessIterator, typename Compare, typename T, typename Distance>
void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, 
                RandomAccessIterator result, const Compare& comp, T value, Distance*)
{
    *result = *first;
    __adjust_heap(first, comp, Distance(0), Distance(last - first), value);
}

template <typename RandomAccessIterator, typename Compare, typename T>
void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, const Compare& comp, T*)
{
    __pop_heap(first, last - 1, last - 1, comp, T(*(last - 1)), distance_type(first));
}

template <typename RandomAccessIterator, typename Compare>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last, const Compare& comp)
{
    __pop_heap_aux(first, last, comp, value_type(first));
}

// function sort_heap

template <typename RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    while (last - first > 1)
        pop_heap(first, last--);
}

template <typename RandomAccessIterator, typename Compare>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last, const Compare& comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}

// function make_heap

template <typename RandomAccessIterator, typename T, typename Distance>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*)
{
    if (last - first <= 1) return;
    Distance len = last - first;
    Distance parent = (len - 2) / 2;
    while (true) {
        __adjust_heap(first, parent, len, first[parent]);
        if (parent == 0) return;
        parent--;
    }
}

template <typename RandomAccessIterator>
void make_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    __make_heap(first, last, value_type(first), distance_type(first));
}

template <typename RandomAccessIterator, typename Compare, typename T, typename Distance>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, const Compare& comp, T*, Distance*)
{
    if (last - first <= 1) return;
    Distance len = last - first;
    Distance parent = (len - 2) / 2;
    while (true) {
        __adjust_heap(first, comp, parent, len, first[parent]);
        if (parent == 0) return;
        parent--;
    }
}

template <typename RandomAccessIterator, typename Compare>
void make_heap(RandomAccessIterator first, RandomAccessIterator last, const Compare& comp)
{
    __make_heap(first, last, comp, value_type(first), distance_type(first));
}

}