#pragma once

// waiting for less<T>

#include <functional>
#include "tiny_vector.h"
#include "tiny_heap.h"

namespace Tiny
{

template <typename T, typename Sequence = vector<T>,
          typename Compare = std::less<typename Sequence::value_type>>
class priority_queue
{
public:
    using value_type = typename Sequence::value_type;
    using size_type = typename Sequence::size_type;
    using reference = typename Sequence::reference;
    using const_reference = typename Sequence::const_reference;

protected:
    Sequence c;
    Compare comp;

public:
    priority_queue() = default;
    explicit priority_queue(const Compare& x) : comp(x) { }

    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    const_reference top() const { return c.front(); }
    void push(const value_type& x) {
        try {
            c.push_back(x);
            Tiny::push_heap(c.begin(), c.end(), comp);
        }
        catch (...) {
            c.clear();
            throw;
        }
    }
    void pop() {
        try {
            Tiny::pop_heap(c.begin(), c.end(), comp);
            c.pop_back();
        }
        catch (...) {
            c.clear();
            throw;
        }
    }
};

}