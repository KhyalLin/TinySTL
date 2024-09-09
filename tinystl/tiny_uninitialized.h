#pragma once

// waiting for fill(), copy(), fill_n()

#include <algorithm>
#include <cstring>              // for memmove()
#include "tiny_construct.h"     // for construct(), destroy()
#include "tiny_type_traits.h"   // for __type_traits, __true_type, __false_type
#include "tiny_iterator.h"

namespace Tiny
{

// function uninitialized_fill_n()

template <typename ForwardIterator, typename Size, typename T>
ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type)
{
    return std::fill_n(first, n, x);
}

template <typename ForwardIterator, typename Size, typename T>
ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type)
{
    ForwardIterator cur = first;
    while (n--) {
        construct(&*cur, x);
        cur++;
    }
    return cur;
}

template <typename ForwardIterator, typename Size, typename T, class T1>
ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*)
{
    using is_POD = typename __type_traits<T1>::is_POD_type;
    return __uninitialized_fill_n_aux(first, n, x, is_POD());
}

template <typename ForwardIterator, typename Size, typename T>
ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, value_type(first));
}

// function uninitialized_copy()

template <typename InputIterator, typename ForwardIterator>
ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type)
{
    return std::copy(first, last, result);
}

template <typename InputIterator, typename ForwardIterator>
ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type)
{
    ForwardIterator cur = result;
    for (; first != last; first++) {
        construct(&*cur, *first);
        cur++;
    }
    return cur;
}

template <typename InputIterator, typename ForwardIterator, typename T>
ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*)
{
    using is_POD = typename __type_traits<T>::is_POD_type;
    return __uninitialized_copy_aux(first, last, result, is_POD());
}

template <typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
{
    return __uninitialized_copy(first, last, result, value_type(result));
}

char* uninitialized_copy(const char* first, const char* last, char* result)
{
    memmove(result, first, last - first);
    return result + (last - first);
}

wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
    memmove(result, first, (last - first) * sizeof(wchar_t));
    return result + (last - first);
}

// function uninitialized_fill()

template <typename ForwardIterator, typename T>
void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __true_type)
{
    std::fill(first, last, x);
}

template <typename ForwardIterator, typename T>
void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __false_type)
{
    for (; first != last; first++)
        construct(&*first, x);
}

template <typename ForwardIterator, typename T, typename T1>
void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*)
{
    using is_POD = typename __type_traits<T1>::is_POD_type;
    __uninitialized_fill_aux(first, last, x, is_POD());
}

template <typename ForwardIterator, typename T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
{
    __uninitialized_fill(first, last, x, value_type(first));
}

}