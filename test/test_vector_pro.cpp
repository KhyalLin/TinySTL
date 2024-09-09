#include <iostream>
#include <algorithm>
#include <exception> // for bad_alloc
#include <cstdlib>   // for malloc(), free(), exit()
#define __USE_MALLOC
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
 
namespace Tiny
{
 
struct __true_type { };
struct __false_type { };
 
template <typename T>
struct __type_traits
{
    using __this_dummy_member_must_be_first = __true_type;
    using has_trivial_default_constructor = __false_type; 
    using has_trivial_copy_constructor = __false_type;
    using has_trivial_assignment_operator = __false_type;
    using has_trivial_destructor = __false_type;
    using is_POD_type = __false_type;
};
 
template <>
struct __type_traits<char>
{
    using has_trivial_default_constructor = __true_type; 
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};
 
template <>
struct __type_traits<signed char>
{
    using has_trivial_default_constructor = __true_type; 
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};
 
template <>
struct __type_traits<unsigned char>
{
    using has_trivial_default_constructor = __true_type; 
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};
 
template <>
struct __type_traits<short>
{
    using has_trivial_default_constructor = __true_type; 
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};
 
template <>
struct __type_traits<unsigned short>
{
    using has_trivial_default_constructor = __true_type; 
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};
 
template <>
struct __type_traits<int>
{
    using has_trivial_default_constructor = __true_type; 
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};
 
template <>
struct __type_traits<unsigned int>
{
    using has_trivial_default_constructor = __true_type; 
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};
 
template <>
struct __type_traits<long>
{
    using has_trivial_default_constructor = __true_type; 
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};
 
template <>
struct __type_traits<unsigned long>
{
    using has_trivial_default_constructor = __true_type; 
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};
 
template <>
struct __type_traits<long long>
{
    using has_trivial_default_constructor = __true_type; 
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};
 
template <>
struct __type_traits<unsigned long long>
{
    using has_trivial_default_constructor = __true_type; 
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};
 
template <>
struct __type_traits<float>
{
    using has_trivial_default_constructor = __true_type; 
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};
 
template <>
struct __type_traits<double>
{
    using has_trivial_default_constructor = __true_type; 
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};
 
template <>
struct __type_traits<long double>
{
    using has_trivial_default_constructor = __true_type; 
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};
 
template <typename T>
struct __type_traits<T*>
{
    using has_trivial_default_constructor = __true_type; 
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};
 
}
 
namespace Tiny
{
 
// class __malloc_alloc_template<inst>
 
template <int inst>
class __malloc_alloc_template
{
private:
    static volatile constexpr void (*__malloc_alloc_oom_handler)() = nullptr;
    static void* oom_malloc(size_t);
    static void *oom_realloc(void*, size_t);
 
public:
    static void* allocate(size_t n)
    {
        void* result = malloc(n);
        if (result == nullptr)
            result = oom_malloc(n);
        return result;
    }
 
    static void deallocate(void* p, size_t)
    {
        free(p);
    }
 
    static void* reallocate(void* p, size_t, size_t new_sz)
    {
        void *result = realloc(p, new_sz);
        if (result == nullptr)
            result = oom_realloc(p, new_sz);
        return result;
    }
    static void* set_malloc_handler(void (*f)())
    {
        void (*old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return old;
    }
};
 
template <int inst>
void* __malloc_alloc_template<inst>::oom_malloc(size_t n)
{
    volatile void (*my_malloc_handler)();
    while (true)
    {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (my_malloc_handler == nullptr)
            throw std::bad_alloc();
        my_malloc_handler();
        void* result = malloc(n);
        if (result) return result;
    }
}
 
template <int inst>
void* __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n)
{
    void (*my_malloc_handler)();
    while (true)
    {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (my_malloc_handler == nullptr)
            throw std::bad_alloc();
        my_malloc_handler();
        void* result = realloc(p, n);
        if (result) return result;
    }
}
 
using malloc_alloc = __malloc_alloc_template<0>;
 
// class __default_alloc_template<threads, inst>
 
template <bool threads, int inst>
class __default_alloc_template
{
private:
    static const int __ALIGN = 8;
    static const int __MAX_BYTES = 128;
    static const int __NFREELISTS = __MAX_BYTES / __ALIGN;
 
    union obj {
        obj* free_list_link;
        char client_data[1];
    };
    static char* start_free;
    static char* end_free;
    static size_t heap_size;
    static obj* volatile free_list[__NFREELISTS];
 
    static size_t ROUND_UP(size_t bytes) {
        return (bytes + __ALIGN - 1) & ~(__ALIGN - 1);
    }
 
    static size_t FREELIST_INDEX(size_t bytes) {
        return (bytes + __ALIGN - 1) / __ALIGN - 1;
    }
    
    static void* refill(size_t n);
    static char* chunk_alloc(size_t size, int& nobjs);
 
public:
    static void* allocate(size_t n);
    static void* reallocate(void* p, size_t old_sz, size_t new_sz);
    static void deallocate(void* p, size_t n);
};
 
template <bool threads, int inst>
char* __default_alloc_template<threads, inst>::start_free = nullptr;
 
template <bool threads, int inst>
char* __default_alloc_template<threads, inst>::end_free = nullptr;
 
template <bool threads, int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;
 
template <bool threads, int inst>
typename __default_alloc_template<threads, inst>::obj* volatile 
__default_alloc_template<threads, inst>::free_list[__NFREELISTS] = { nullptr };
 
template <bool threads, int inst>
void* __default_alloc_template<threads, inst>::allocate(size_t n)
{
    if (n > __MAX_BYTES)
        return malloc_alloc::allocate(n);
    
    obj* volatile * my_free_list = free_list + FREELIST_INDEX(n);
    obj* result = *my_free_list;
    if (result == nullptr)
        return refill(ROUND_UP(n));
    
    *my_free_list = result -> free_list_link;
    return result;
}
 
template <bool threads, int inst>
void __default_alloc_template<threads, inst>::deallocate(void* p, size_t n)
{
    if (n > __MAX_BYTES) {
        malloc_alloc::deallocate(p, n);
        return;
    }
 
    obj* volatile * my_free_list = free_list + FREELIST_INDEX(n);
    obj* q = (obj*)p;
    q -> free_list_link = *my_free_list;
    *my_free_list = q;
}
 
template <bool threads, int inst>
void* __default_alloc_template<threads, inst>::refill(size_t n)
{
    int nobjs = 20;
    char* chunk = chunk_alloc(n, nobjs);
    if (nobjs == 1) return chunk;
 
    obj* volatile * my_free_list = free_list + FREELIST_INDEX(n);
    obj* result = (obj*)chunk;
    obj* next_obj = *my_free_list = (obj*)(chunk + n);
    for (int i = 1; ; i++)
    {
        obj* current_obj = next_obj;
        if (nobjs == i + 1) {
            current_obj -> free_list_link = nullptr;
            break;
        }
        next_obj = (obj*)((char*)next_obj + n);
        current_obj -> free_list_link = next_obj;
    }
    return result;
}
 
template <bool threads, int inst>
char* __default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& nobjs)
{
    size_t total_bytes = size * nobjs;
    size_t bytes_left = end_free - start_free;
 
    if (bytes_left >= total_bytes) {
        char* result = start_free;
        start_free += total_bytes;
        return result;
    }
    if (bytes_left >= size) {
        nobjs = bytes_left >= size;
        total_bytes = size * nobjs;
        char* result = start_free;
        start_free += total_bytes;
        return result;
    }
 
    if (bytes_left > 0) {
        obj* volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
        reinterpret_cast<obj*>(start_free) -> free_list_link = *my_free_list;
        *my_free_list = (obj*)start_free;
    }
    size_t bytes_to_get = total_bytes * 2 + ROUND_UP(heap_size >> 4);
    start_free = (char*)malloc(bytes_to_get);
    if (start_free == nullptr) {
        for (int i = size; i <= __MAX_BYTES; i += __ALIGN) {
            obj* volatile * my_free_list = free_list + FREELIST_INDEX(i);
            obj* p = *my_free_list;
            if (p != nullptr) {
                *my_free_list = p->free_list_link;
                start_free = (char*)p;
                end_free = start_free + i;
                return chunk_alloc(size, nobjs);
            }
        }
    }
 
    start_free = (char*)malloc_alloc::allocate(bytes_to_get);
    end_free = start_free + bytes_to_get;
    heap_size += bytes_to_get;
    return chunk_alloc(size, nobjs);
}
 
// using default_alloc = __default_alloc_template<__NODE_ALLOCATOR_THREADS, 0>;
using default_alloc = __default_alloc_template<true, 0>;
 
// class simple_alloc
 
template <typename T, typename Alloc>
class simple_alloc
{
public:
    static T* allocate(size_t n)
    {
        if (n == 0) return 0;
        return static_cast<T*>(Alloc::allocate(n * sizeof(T)));
    }
    static T* allocate(void)
    {
        return static_cast<T*>(Alloc::allocate(sizeof(T)));
    }
    static void deallocate(T* p, size_t n)
    {
        if (n == 0) return;
        Alloc::deallocate(p, n * sizeof(T));
    }
    static void deallocate(T* p)
    {
        Alloc::deallocate(p, sizeof(T));
    }
};
 
#ifdef __USE_MALLOC
 
using alloc = malloc_alloc;
 
#else
 
using alloc = default_alloc;
 
#endif
 
}
 
#include <new>                  // for placement new
 
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
    void clear() {
        erase(begin(), end());
    }
};
 
template <typename T, typename Alloc>
vector<T, Alloc>::vector(const vector& x)
{
    size_type len = x.size();
    iterator result = data_allocator::allocate(len);
    uninitialized_copy(x.begin(), x.end(), result);
    start = result;
    finish = result + len;
    end_of_storage = result + len;
}
 
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
        data_allocator::deallocate(start, end_of_storage);
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
 
int solve(int n, int m)
{
    Tiny::vector<int> a(n + 1);
    for (int i = 1; i <= n; i++)
        std::cin >> a[i];
    Tiny::vector<Tiny::vector<int>> g(n + 1);
    while (m--) {
        int u, v;
        std::cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for (int i = 1; i <= n; i++)
    {
        g[i].push_back(0);
        std::sort(g[i].begin(), g[i].end(), [&](int x, int y)
            { return a[x] > a[y]; });
    }
 
    Tiny::vector<int> vis(n + 1);
    int ans = 0;
    auto dfs = [&](auto dfs, int s, int u, int dep, int sum) -> void {
        sum += a[u], vis[u] = true;
        ans = std::max(ans, sum);
        if (dep == 3) {
            int n1 = std::min((int)g[s].size(), 4);
            int n2 = std::min((int)g[u].size(), 4);
            for (int i = 0; i < n1; i++)
                for (int j = 0; j < n2; j++)
                {
                    int t1 = g[s][i], t2 = g[u][j];
                    if (t1 == t2 or vis[t1] or vis[t2]) continue;
                    ans = std::max(ans, sum + a[t1] + a[t2]);
                }
        }
        else {
            for (int v : g[u])
                if (!vis[v])
                    dfs(dfs, s, v, dep + 1, sum);
        }
        vis[u] = false;
    };
    for (int i = 1; i <= n; i++)
        dfs(dfs, i, i, 1, 0);
    return ans;
}
 
int main(void)
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
 
    int t = 1, n, m;
    while (t--)
    {
        std::cin >> n >> m;
        std::cout << solve(n, m) << '\n';
    }
}
