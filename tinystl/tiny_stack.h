#pragma once

#include "tiny_deque.h"

namespace Tiny
{

template <typename T, typename Sequence = deque<T>>
class stack
{
    // friend bool operator==(const stack&, const stack&);
    // friend bool operator< (const stack&, const stack&);

public:
    using value_type = typename Sequence::value_type;
    using size_type = typename Sequence::size_type;
    using refrence = typename Sequence::reference;
    using const_reference = typename Sequence::const_reference;

protected:
    Sequence c;

public:
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    reference top() { return c.back(); }
    const_reference top() const { return c.back(); }
    void push(const value_type& x) { c.push_back(); }
    void pop() { c.pop_back(); }
};

}