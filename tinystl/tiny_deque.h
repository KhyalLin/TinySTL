#pragma once

// waiting for copy(), copy_backward(), max;

#include <algorithm>
#include "tiny_alloc.h"
#include "tiny_construct.h"
#include "tiny_uninitialized.h"
#include "tiny_iterator.h"
#include <iso646.h>

namespace Tiny
{

size_t __deque_buf_size(size_t n, size_t sz) {
    if (n != 512) return n;
    if (sz >= 512) return 1;
    return 512 / sz;
}

template <typename T, typename Ref, typename Ptr, size_t BufSiz>
struct __deque_iterator
{
    using iterator = __deque_iterator<T, T&, T*, BufSiz>;
    using const_iterator = __deque_iterator<T, const T&, const T*, BufSiz>;
    size_t buffer_size() const { return __deque_buf_size(BufSiz, sizeof(T)); }

    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using pointer = Ptr;
    using reference = Ref;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using map_pointer = T**;
    using self = __deque_iterator;

    T* cur;
    T* first;
    T* last;
    map_pointer node;

    void set_node(map_pointer new_node) {
        node = new_node;
        first = *new_node;
        last = first + buffer_size();
    }
    reference operator*() const { return *cur; }
    pointer operator->() const { return &(*cur); }
    bool operator==(const self& x) const { return cur == x.cur; }
    bool operator!=(const self& x) const { return !(*this == x); }
    bool operator<(const self& x) const {
        if (node == x.node) return cur < x.cur;
        return node < x.node;
    }
    difference_type operator-(const self& x) const {
        return buffer_size() * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
    }

    self& operator++() {
        cur++;
        if (cur == last) {
            set_node(node + 1);
            cur = first;
        }
        return *this;
    }
    self operator++(int) {
        self tmp = *this;
        ++*this;
        return tmp;
    }
    self& operator--() {
        if (cur == first) {
            set_node(node - 1);
            cur = last;
        }
        --cur;
        return *this;
    }
    self operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    }

    self& operator+=(difference_type n) {
        difference_type offset = n + (cur - first);
        if (offset >= 0 and offset < buffer_size())
            cur += n;
        else {
            difference_type node_offset = offset / buffer_size();
            if (offset <= 0) {
                node_offset = (-offset - 1) / buffer_size();
                node_offset = -node_offset - 1;
            }
            set_node(node + node_offset);
            cur = first + (offset - node_offset * buffer_size());
        }
        return *this;
    }
    self operator+(difference_type n) const {
        self tmp = *this;
        return tmp += n;
    }
    self& operator-=(difference_type n) {
        return *this += n;
    }
    self operator-(difference_type n) const {
        self tmp = *this;
        return tmp -= n;
    }

    reference operator[](difference_type n) const {
        return *(*this + n);
    }
};

template <typename T, typename Alloc = alloc, size_t BufSiz = 512>
class deque
{
public:
    using value_type = T;
    using pointer = value_type*;
    using reference = value_type&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using iterator = __deque_iterator<T, T&, T*, BufSiz>;
    using const_reference = const value_type&;
    using const_iterator = __deque_iterator<T, const T&, const T*, BufSiz>;
    
protected:
    using data_allocator = simple_alloc<value_type, Alloc>;
    using map_allocator = simple_alloc<pointer, Alloc>;
    using map_pointer = pointer*;
    map_pointer map;
    size_type map_size;
    iterator start;
    iterator finish;
    
    size_t buffer_size() const { return __deque_buf_size(BufSiz, sizeof(T)); }
    size_type initial_map_size() const { return 8; };
    void create_map_and_nodes(size_type num_elements);
    void reallocate_map(size_type nodes_to_add, bool add_at_front);
    void deallocate_map();
    void reverse_map_at_front(size_type nodes_to_add = 1);
    void reverse_map_at_back(size_type nodes_to_add = 1);
    void fill_initialize(size_type, const value_type&);
    pointer allocate_node();
    void deallocate_node(pointer);
    void push_front_aux(const value_type&);
    void push_back_aux(const value_type&);
    void pop_front_aux();
    void pop_back_aux();
    iterator insert_aux(iterator, const value_type&);

public:
    iterator begin() { return start; }
    iterator end() { return finish; }
    const_iterator begin() const { return start; }
    const_iterator end() const { return finish; }
    reference operator[](size_type n) { return start[n]; }    
    reference front() { return *start; }
    reference back() { 
        iterator tmp = finish;
        --tmp;
        return *tmp;
    }
    const_reference operator[](size_type n) const { return start[n]; }
    const_reference front() const { return *start; }
    const_reference back() const { 
        iterator tmp = finish;
        --tmp;
        return *tmp;
    }

    size_type size() const { return finish - start; }
    size_type max_size() const { return -1; }
    bool empty() const { return finish == start; }
    
    deque() : map(nullptr), map_size(0) {
        create_map_and_nodes(0);
    }
    deque(int n, const T& value) : map(nullptr), map_size(0) {
        fill_initialize(n, value);
    }
    deque(const deque& q) : map(nullptr), map_size(0) {
        create_map_and_nodes(0);
        for (const T& x : q)
            push_back(x);
    }
    deque(deque&& q) {
        map = q.map;
        map_size = q.size;
        q.map = nullptr;
        q.map_size = 0;
    }
    ~deque() {
        clear();
        deallocate_map();
    }
    void swap(deque& q) {
        swap(map, q.map);
        swap(map_size, q.map_size);
        swap(start, q.start);
        swap(finish, q.finish);
    }

    void push_front(const T&);
    void push_back(const T&);
    void pop_front();
    void pop_back();
    void clear();
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    iterator insert(iterator position, const T&);
};

template <typename T, typename Alloc, size_t BufSiz>
auto deque<T, Alloc, BufSiz>::allocate_node() -> pointer
{
    return data_allocator::allocate(BufSiz);
}

template <typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::deallocate_node(pointer p)
{
    data_allocator::deallocate(p, BufSiz);
}

template <typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::create_map_and_nodes(size_type num_elements)
{
    size_type num_nodes = num_elements / buffer_size() + 1;
    map_size = std::max(initial_map_size(), num_nodes + 2);
    map = map_allocator::allocate(map_size);

    map_pointer nstart = map + (map_size - num_nodes) / 2;
    map_pointer nfinish = nstart + num_nodes - 1;
    try {
        for (map_pointer cur = nstart; cur <= nfinish; cur++)
            *cur = allocate_node();
    }
    catch (...) {
        for (map_pointer cur = nstart; cur <= nfinish; cur++)
            deallocate_node(*cur);
        throw;
    }
    start.set_node(nstart);
    finish.set_node(nfinish);
    start.cur = start.first;
    finish.cur = finish.first + num_elements % buffer_size();
}

template <typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::reallocate_map(size_type nodes_to_add, bool add_at_front)
{
    size_type old_num_nodes = finish.node - start.node + 1;
    size_type new_num_nodes = old_num_nodes + nodes_to_add;
    map_pointer new_nstart;
    if (map_size > 2 * new_num_nodes)
    {
        new_nstart = map + (map_size - new_num_nodes) / 2;
        if (add_at_front) new_nstart += nodes_to_add;
        if (new_nstart < start.node)
            std::copy(start.node, finish.node + 1, new_nstart);
        else
            std::copy_backward(start.node, finish.node + 1, new_nstart);
    }
    else
    {
        size_type new_map_size = map_size + std::max(map_size, nodes_to_add) + 2;
        map_pointer new_map = map_allocator::allocate(new_map_size);
        new_nstart = new_map + (new_map_size - new_num_nodes) / 2;
        if (add_at_front) new_nstart += nodes_to_add;
        
        std::copy(start.node, finish.node + 1, new_nstart);
        map_allocator::deallocate(map, map_size);
        map = new_map;
        map_size = new_map_size;
    }

    start.set_node(new_nstart);
    finish.set_node(new_nstart + old_num_nodes + 1);
}


template <typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::deallocate_map()
{
    if (map == nullptr) return;
    map_allocator::deallocate(map, map_size);
}

template <typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::reverse_map_at_front(size_type nodes_to_add)
{
    if (nodes_to_add >= map_size - (finish.node - map))
        reallocate_map(nodes_to_add, false);
}

template <typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::reverse_map_at_back(size_type nodes_to_add)
{
    if (nodes_to_add > start.node - map)
        reallocate_map(nodes_to_add, true);
}

template <typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::fill_initialize(size_type n, const value_type& value)
{
    create_map_and_nodes(n);
    try {
        for (map_pointer cur = start.node; cur < finish.node; cur++)
            uninitialized_fill(*cur, *cur + buffer_size(), value);
        uninitialized_fill(finish.first, finish.cur, value);
    }
    catch (...) {
        for (map_pointer cur = start.node; cur < finish.node; cur++)
            destroy(*cur, *cur + buffer_size());
        destroy(finish.first, finish.cur);
        throw;
    }
}

template <typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::push_back_aux(const value_type& value)
{
    reverse_map_at_back();
    map_pointer new_node = finish.node + 1;
    *new_node = allocate_node();
    try {
        construct(finish.cur, value);
        finish.set_node(new_node);
        finish.cur = finish.first;
    }
    catch (...) {
        deallocate_node(*new_node);
        throw;
    }
}

template <typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::push_back(const T& value)
{
    if (finish.cur + 1 != finish.last)
    {
        construct(finish.cur, value);
        finish.cur++;
    }
    else
        push_back_aux(value);
}

template <typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::push_front_aux(const value_type& value)
{
    reverse_map_at_front();
    map_pointer new_node = start.node - 1;
    *new_node = allocate_node();
    try {
        start.set_node(new_node);
        start.cur = start.last - 1;
        construct(start.cur, value);
    }
    catch (...) {
        start.set_node(start.node + 1);
        start.cur = start.first;
        deallocate_node(*new_node);
        throw;
    }
}

template <typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::push_front(const T& value)
{
    if (start.cur != start.first) {
        construct(start.cur - 1, value);
        start.cur--;
    }
    else
        push_front_aux(value);
}

template <typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::pop_back_aux()
{
    deallocate_node(finish.first);
    finish.set_node(finish.node - 1);
    finish.cur = finish.last - 1;
    destroy(finish.cur);
}

template <typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::pop_back()
{
    if (finish.cur != finish.first)
    {
        finish.cur--;
        destroy(finish.cur);
    }
    else
        pop_back_aux();
}

template <typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::pop_front_aux()
{
    destroy(start.cur);
    deallocate_node(start.first);
    start.set_node(start.node + 1);
    start.cur = start.first;
}

template <typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::pop_front()
{
    if (start.cur != start.last - 1)
    {
        destroy(start.cur);
        start.cur++;
    }
    else
        pop_front_aux();
}

template <typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::clear()
{
    for (map_pointer node = start.node + 1; node < finish.node; node++)
    {
        destroy(*node, *node + buffer_size());
        data_allocator::deallocate(*node, buffer_size());
    }
    if (start.node != finish.node) {
        destroy(start.cur, start.last);
        destroy(finish.first, finish.cur);
        data_allocator::deallocate(finish.first, buffer_size());
    }
    else
        destroy(start.cur, finish.cur);
    
    finish = start;
}

template <typename T, typename Alloc, size_t BufSiz>
auto deque<T, Alloc, BufSiz>::erase(iterator pos) -> iterator
{
    iterator next = pos;
    next++;
    difference_type index = pos - start;
    if (index < size() / 2) {
        copy_backward(start, pos, next);
        pop_front();
    }
    else {
        copy(next, finish, pos);
        pop_back();
    }
    return start + index;
}

template <typename T, typename Alloc, size_t BufSiz>
auto deque<T, Alloc, BufSiz>::erase(iterator first, iterator last) -> iterator
{
    if (first == start and last == finish) {
        clear();
        return finish;
    }

    difference_type n = last - first;
    difference_type elems_before = first - start;
    if (elems_before < (size() - n) / 2) {
        copy_backward(start, first, last);
        iterator new_start = start + n;
        destroy(start, new_start);
        for (map_pointer cur = start.node; cur < new_start.node; cur++)
            data_allocator::deallocate(*cur, buffer_size());
        start = new_start;
    }
    else
    {
        copy(last, finish, first);
        iterator new_finish = finish - n;
        destroy(new_finish, finish);
        for (map_pointer cur = new_finish.node + 1; cur <= finish.node; cur++)
            data_allocator::deallocate(*cur, buffer_size());
        finish = new_finish;
    }
    return start + elems_before;
}

template <typename T, typename Alloc, size_t BufSiz>
auto deque<T, Alloc, BufSiz>::insert(iterator position, const T& value) -> iterator
{
    if (position.cur == start.cur) {
        push_front(value);
        return start;
    }
    if (position.cur == finish.cur) {
        push_back(value);
        iterator tmp = finish;
        tmp--;
        return tmp;
    }
    return insert_aux(position, value);
}

template <typename T, typename Alloc, size_t BufSiz>
auto deque<T, Alloc, BufSiz>::insert_aux(iterator pos, const T& value) -> iterator
{
    difference_type index = pos - start;
    if (index < size() / 2) {
        push_front(front());
        iterator front1 = start;
        front1++;
        iterator front2 = front1;
        front2++;
        pos = start + index;
        iterator pos1 = pos;
        pos1++;
        copy(front2, pos1, front1);
    }
    else {
        push_back(back());
        iterator back1 = finish;
        back1--;
        iterator back2 = back1;
        back2--;
        pos = start + index;
        copy_backward(pos, back2, back1);
    }
    *pos = value;
    return pos;
}

}