#include <iostream>
#include "tiny_vector.h"
#include "tiny_heap.h"

using namespace Tiny;
using std::cout;
using std::cin;
using std::endl;

int main(void)
{
    int ia[9] = { 0, 1, 2, 3, 4, 8, 9, 3, 5 };
    vector<int> a(9);
    for (int i = 0; i < 9; i++)
        a[i] = ia[i];
    
    make_heap(a.begin(), a.end());
    for (int i = 0; i < a.size(); i++)
        cout << a[i] << ' ';
    cout << endl;

    a.push_back(7);
    push_heap(a.begin(), a.end());
    for (int i = 0; i < a.size(); i++)
        cout << a[i] << ' ';
    cout << endl;

    pop_heap(a.begin(), a.end());
    cout << a.back() << endl;
    a.pop_back();

    for (int i = 0; i < a.size(); i++)
        cout << a[i] << ' ';
    cout << endl;

    sort_heap(a.begin(), a.end());
    for (int i = 0; i < a.size(); i++)
        cout << a[i] << ' ';
    cout << endl;
}