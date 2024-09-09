#pragma once

#include <new>                  // for placement new
#include "tiny_type_traits.h"   // for __type_traits, __true_type, __false_type
#include "tiny_iterator.h"

namespace Tiny
{

template <class T1, class T2>
void construct(T1* p, const T2& value) {
    if (p == nullptr) return;
    new(p) T1(value);
}

template <class T>
void destroy(T* pointer) {
    if (pointer == nullptr) return;
    pointer->~T();
}

template <typename ForwardIterator>
void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
    for (; first != last; first++)
        destroy(&*first);
}

template <typename ForWardIterator>
void __destroy_aux(ForWardIterator first, ForWardIterator last, __true_type) { }

template <class ForwardIterator, class T>
void _destroy(ForwardIterator first, ForwardIterator last, T*)
{
    using trivial_destructor = typename __type_traits<T>::has_trivial_destructor;
    __destroy_aux(first, last, trivial_destructor());
}

template <class ForwardIterator>
void destroy(ForwardIterator first, ForwardIterator last) {
    if (first == nullptr) return;
    _destroy(first, last, value_type(first));
}

void destroy(char*, char*) { }
void destroy(wchar_t*, wchar_t*) { }

}