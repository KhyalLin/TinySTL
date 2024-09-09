#include <iostream>
#include <algorithm>
#include "tiny_deque.h"

using namespace Tiny;
using std::cin;
using std::cout;
using std::endl;

int main(void)
{
    deque<int, alloc, 32> ideq(20, 9);
    cout << "size = " << ideq.size() << endl;

    for (int i = 0; i < ideq.size(); i++)
        ideq[i] = i;
    for (int i = 0; i < ideq.size(); i++)
        cout << ideq[i] << ' ';
    cout << endl;
    cout << "size = " << ideq.size() << endl;

    ideq.push_front(99);
    for (int i = 0; i < ideq.size(); i++)
        cout << ideq[i] << ' ';
    cout << endl;
    cout << "size = " << ideq.size() << endl; 
    
    ideq.push_front(98);
    ideq.push_front(97);
    for (int i = 0; i < ideq.size(); i++)
        cout << ideq[i] << ' ';
    cout << endl;
    cout << "size = " << ideq.size() << endl; 
}