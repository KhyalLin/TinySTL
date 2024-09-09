#pragma once

// waiting for unary_function

namespace Tiny
{

template <typename T>
class identity // : public unary_function<T, T>
{
public:
    const T& operator()(const T& x) const { return x; }
};

template <typename T1, typename T2>
struct pair {
    using first_type = T1;
    using second_type = T2;
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

}