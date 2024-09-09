#include "tiny_hash_fun.h"
#include "tiny_hashtable.h"
#include <iostream>
#include <cstring>
#include <functional>

int main(void)
{
    Tiny::hashtable<int, int, Tiny::hash<int>, std::_Identity<int>, std::equal_to<int>, Tiny::alloc>
            ht(50, Tiny::hash<int>(), std::equal_to<int>());
    std::cout << ht.size() << std::endl;
    std::cout << ht.bucket_count() << std::endl;
    std::cout << ht.max_bucket_count() << std::endl;

    ht.insert_unique(59);
    ht.insert_unique(63);
    ht.insert_unique(108);
    ht.insert_unique(2);
    ht.insert_unique(53);
    ht.insert_unique(53);
    std::cout << ht.size() << std::endl;

    for (auto x : ht)
        std::cout << x << ' ';
    std::cout << std::endl;

    for (int i = 0; i < ht.bucket_count(); i++)
    {
        int n = ht.elems_in_bucket(i);
        if (n == 0) continue;
        std::cout << "bucket[" << i << "] has " << n << " elems." << std::endl;
    }

    for (int i = 0; i <= 53; i++)
        ht.insert_equal(i);
    std::cout << ht.size() << std::endl;
    std::cout << ht.bucket_count() << std::endl;

    for (int i = 0; i < ht.bucket_count(); i++)
    {
        int n = ht.elems_in_bucket(i);
        if (n == 0) continue;
        std::cout << "bucket[" << i << "] has " << n << " elems." << std::endl;
    }

    for (auto it = ht.begin(); it != ht.end(); it++)
        std::cout << *it << ' ';
    std::cout << std::endl;

    std::cout << *ht.find(2) << std::endl;
    std::cout << ht.count(2) << std::endl;

    ht.erase(2);
    //std::cout << *ht.find(2) << std::endl;
    std::cout << ht.count(2) << std::endl;

    ht.erase(ht.find(11));
    std::cout << ht.count(11) << std::endl;
    std::cout << ht.count(108) << std::endl;
}