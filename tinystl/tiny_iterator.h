#pragma once

namespace Tiny
{

// based class iterator and tags
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <typename Category, typename T, class Distance = ptrdiff_t, 
          typename Pointer = T*, class Reference = T&>
struct iterator
{
    using iterator_category = Category;
    using value_type = T;
    using difference_type = Distance;
    using pointer = Pointer;
    using reference = Reference;
};

// class iterator_traits
template <typename Iterator>
struct iterator_traits
{
    using iterator_category = typename Iterator::iterator_category;
    using value_type = typename Iterator::value_type;
    using difference_type = typename Iterator::difference_type;
    using pointer = typename Iterator::pointer;
    using referencce = typename Iterator::reference;
};

template <typename T>
struct iterator_traits<T*>
{
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using referencce = T&;
};

template <typename T>
struct iterator_traits<const T*>
{
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = const T*;
    using referencce = const T&;
};

// traits functions
template <typename Iterator>
auto iterator_category(const Iterator&) 
    -> typename iterator_traits<Iterator>::iterator_category
{
    using category = typename iterator_traits<Iterator>::iterator_category;
    return category();
}

template <typename Iterator>
auto distance_type(const Iterator&) 
    -> typename iterator_traits<Iterator>::difference_type*
{
    return nullptr;
}

template <typename Iterator>
auto value_type(const Iterator&) 
    -> typename iterator_traits<Iterator>::value_type*
{
    return nullptr;
}

// iterator methods: distance()
template <typename InputIterator>
auto __distance(InputIterator first, InputIterator last, input_iterator_tag) 
    -> typename iterator_traits<InputIterator>::difference_type
{
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last)
        first++, n++;
    return n;
}

template <typename InputIterator>
auto __distance(InputIterator first, InputIterator last, random_access_iterator_tag) 
    -> typename iterator_traits<InputIterator>::difference_type
{
    return last - first;
}

template <typename InputIterator>
auto distance(InputIterator first, InputIterator last) 
    -> typename iterator_traits<InputIterator>::difference_type
{
    using category = typename iterator_traits<InputIterator>::iterator_category;
    return __distance(first, last, category());
}

// iterator methods: advance()
template <typename InputIterator, typename Distance>
void __advance(InputIterator& i, Distance n, input_iterator_tag)
{
    while (n--)
        i++;
}

template <typename BidirectionalIterator, typename Distance>
void __advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)
{
    if (n >= 0)
        while (n--) i++;
    else
        while (n--) i++;
}

template <typename RandomAccessIterator, typename Distance>
void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag)
{
    i += n;
}

template <typename InputIterator, typename Distance>
void advance(InputIterator& i, Distance n)
{
    __advance(i, n, iterator_category(i));
}

}