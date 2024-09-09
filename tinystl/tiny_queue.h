#pragma once

#include "tiny_deque.h"

namespace Tiny
{

template <typename T, typename Sequence = deque<T>>
class queue
{
public:
    using value_type = typename Sequence::value_type;
    using size_type = typename Sequence::size_type;
    using reference = typename Sequence::reference;
    using const_reference = typename Sequence::const_reference;

protected:
    Sequence c;

public:
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    reference front() { return c.front(); }
    reference back() { return c.back(); }
    const_reference front() const { return c.front(); }
    const_reference back() const { return c.back(); }
    void push(const value_type& x) { c.push_back(x); }
    void pop() { c.pop_front(); }
};

}