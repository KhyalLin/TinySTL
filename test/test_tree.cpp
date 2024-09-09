#include "tiny_tree.h"
#include <iostream>

class Comp
{
public:
    const int& operator()(const int& x) { return x; }
};

using namespace Tiny;
using tree = rb_tree<int, int, Comp, std::less<int>>;

void show(const tree& t)
{
    std::cout << "size = " << t.size() << std::endl;
    for (auto it = t.begin(); it != t.end(); it++)
        std::cout << *it << ' ';
    std::cout << std::endl;
}

int main(void)
{
    tree t;
    
    t.insert_unique(10);
    show(t);
    t.insert_unique(7);
    show(t);
    t.insert_unique(8);
    show(t);
    t.insert_unique(15);
    show(t);
    t.insert_unique(5);
    show(t);
    t.insert_unique(6);
    show(t);
    t.insert_unique(11);
    show(t);
    t.insert_unique(13);
    show(t);
    t.insert_unique(12);
    show(t);
    t.erase(12);
    show(t);
    t.erase(8);
    show(t);
    t.insert_unique(0);
    show(t);
    t.insert_unique(1);
    show(t);
    t.insert_equal(0);
    show(t);
    t.erase(0);
    show(t);
}