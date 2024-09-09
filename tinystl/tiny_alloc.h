#pragma once

#include <exception> // for bad_alloc
#include <cstdlib>   // for malloc(), free(), exit()

namespace Tiny
{

// class __malloc_alloc_template<inst>

template <int inst>
class __malloc_alloc_template
{
private:
    static constexpr volatile void (*__malloc_alloc_oom_handler)() = nullptr;
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
