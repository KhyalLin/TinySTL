#pragma once

// waiting for copy(), fill(), copy_backward(), max(), swap()

#include <algorithm>
#include "tiny_construct.h"
#include "tiny_alloc.h"
#include "tiny_uninitialized.h"
#include "tiny_iterator.h"

namespace Tiny
{

template <typename T, typename Alloc = alloc>
class vector
{
public:
    using value_type = T;
    using pointer = value_type*;
    using iterator = value_type*;
    using reference = value_type&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using const_iterator = const value_type*;
    using const_reference = const value_type&;

protected:
    using data_allocator = simple_alloc<value_type, Alloc>;
    iterator start;
    iterator finish;
    iterator end_of_storage;

    iterator allocate_and_fill(size_type n, const T& x) {
        iterator result = data_allocator::allocate(n);
        uninitialized_fill_n(result, n, x);
        return result;
    }
    void deallocate(void) {
        if (start == nullptr) return;
        data_allocator::deallocate(start, end_of_storage - start);
    }
    void fill_initialize(size_type n, const T& value) {
        start = allocate_and_fill(n, value);
        finish = start + n;
        end_of_storage = finish;
    }

public:
    iterator begin() { return start; }
    iterator end() { return finish; }
    const_iterator begin() const { return start; }
    const_iterator end() const { return finish; }
    size_type size() const { return end() - begin(); }
    size_type capacity() const { return end_of_storage - begin(); }
    bool empty() const { return begin() == end(); }
    reference operator[](size_type n) { return *(begin() + n); }
    reference front() { return *begin(); }
    reference back() { return *(end() - 1); }
    const_reference operator[](size_type n) const { return *(begin() + n); }
    const_reference front() const { return *begin(); }
    const_reference back() const { return *(end() - 1); }

    vector() : start(nullptr), finish(nullptr), end_of_storage(nullptr) {  }
    explicit vector(size_type n) {  fill_initialize(n, T()); }
    vector(size_type n, const T& value) { fill_initialize(n, value); }
    vector(const vector&);
    vector(vector&&);
    ~vector() { 
        destroy(begin(), end());
        deallocate(); 
    }

    vector& operator=(const vector&);
    void swap(vector&);
    void assign(size_type n, const T& x);
    void insert(iterator postion, const T&);
    void insert(iterator postion, size_type, const T&);
    void push_back(const T& x)
    {
        if (finish != end_of_storage) {
            construct(finish, x);
            finish++;
        }
        else {
            insert(end(), x);
        }
    }
    void pop_back()
    {
        finish--;
        destroy(finish);
    }
    iterator erase(iterator position)
    {
        if (position + 1 != end())
            copy(position + 1, finish, position);
        finish--;
        destroy(finish);
        return position;
    }
    iterator erase(iterator first, iterator last)
    {
        iterator i = std::copy(last, finish, first);
        destroy(i, finish);
        finish -= last - first;
        return first;
    }
    void resize(size_type new_size, const T& x)
    {
        if (new_size < size())
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size(), x);
    }
    void reserve(size_type new_len)
    {
        if (new_len <= capacity())
            return;
        iterator new_start = data_allocator::allocate(new_len);
        iterator new_finish = uninitialized_copy(start, finish, new_start);
        iterator new_end = new_start + new_len;
        
        clear(), deallocate();
        start = new_start;
        finish = new_finish;
        end_of_storage = new_end;
    }
    void clear() {
        erase(begin(), end());
    }
};

template <typename T, typename Alloc>
vector<T, Alloc>::vector(const vector& x)
    : vector(x.begin(), x.end()) { }

template <typename T, typename Alloc>
vector<T, Alloc>::vector(vector&& x)
{
    start = x.start;
    finish = x.finish;
    end_of_storage = x.end_of_storage;
    x.start = nullptr;
    x.finish = nullptr;
    x.end_of_storage = nullptr;
}

template <typename T, typename Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& x)
{
    clear();
    iterator first = x.begin();
    iterator last = x.end();
    size_type len = distance(first, last);
    iterator result = data_allocator::allocate(len);
    start = result;
    finish = start + len;
    end_of_storage = finish;
    for (; first != last; first++)
        construct(result, *first);
    return *this;
}

template <typename T, typename Alloc>
void vector<T, Alloc>::swap(vector& x)
{
    std::swap(start, x.start);
    std::swap(finish, x.finish);
    std::swap(end_of_storage, x.end_of_storage);
}

template <typename T, typename Alloc>
void vector<T, Alloc>::assign(size_type n, const T& x)
{
    if (n <= capacity()) {
        destroy(begin(), end());
        finish = uninitialized_fill_n(begin(), n, x);
        return;
    }

    iterator new_start = data_allocator::allocate(n);
    iterator new_finish = new_start;
    try {
        new_finish = uninitialized_fill_n(new_start, n, x);
    }
    catch (...) {
        destroy(new_start, new_finish);
        data_allocator::deallocate(start, end_of_storage - start);
        throw;
    }

    destroy(start, finish);
    deallocate();
    start = new_start;
    finish = new_finish;
    end_of_storage = new_finish;
}

template <typename T, typename Alloc>
void vector<T, Alloc>::insert(iterator position, const T& x) {
    if (finish != end_of_storage) {
        construct(finish, *(finish - 1));
        std::copy_backward(position, finish - 1, finish);
        *position = x;
        finish++;
        return;
    }
    
    const size_type old_size = size();
    const size_type len = old_size ? old_size * 2 : 1;
    iterator new_start = data_allocator::allocate(len);
    iterator new_finish = new_start;
    try {
        new_finish = uninitialized_copy(start, position, new_start);
        construct(new_finish, x);
        new_finish++;
        new_finish = uninitialized_copy(position, finish, new_finish);
    }
    catch(...) {
        destroy(new_start, new_finish);
        data_allocator::deallocate(new_start, len);
        throw;
    }

    destroy(begin(), end());
    deallocate();
    start = new_start;
    finish = new_finish;
    end_of_storage = new_start + len;
}

template <typename T, typename Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const T& x)
{
    if (n == 0) return;
    if (end_of_storage - finish >= n)
    {
        const size_type elems_after = finish - position;
        iterator old_finish = finish;
        if (elems_after > n) {
            uninitialized_copy(finish - n, finish, finish);
            std::copy_backward(position, finish - n, finish);
            std::fill(position, position + n, x);
            finish += n;
        }
        else
        {
            uninitialized_fill_n(finish, n - elems_after, x);
            finish += n - elems_after;
            uninitialized_copy(position, old_finish, finish);
            finish += elems_after;
            std::fill(position, old_finish, x);
        }
        return;
    }

    const size_type old_size = size();
    const size_type len = old_size + std::max(old_size, n);
    iterator new_start = data_allocator::allocate(len);
    iterator new_finish = start;
    try {
        new_finish = uninitialized_copy(start, position, new_start);
        new_finish = uninitialized_fill_n(new_finish, n, x);
        new_finish = uninitialized_copy(position, finish, new_finish);
    }
    catch (...) {
        destroy(new_start, new_finish);
        data_allocator::deallocate(new_start, len);
        throw;
    }

    destroy(start, finish);
    deallocate();
    start = new_start;
    finish = new_finish;
    end_of_storage = new_start + len;
}

}
