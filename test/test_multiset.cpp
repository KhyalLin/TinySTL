#include "tiny_multiset.h"
#include <iostream>

int main(void)
{
    int a[] = { 0, 1, 2, 3, 4 };
    Tiny::multiset<int> s;
    for (int x : a)
        s.insert(x);
    
    using namespace std;
    cout << "size = " << s.size() << endl;
    cout << "3 count = " << s.count(3) << endl;
    s.insert(3);
    cout << "size = " << s.size() << endl;
    cout << "3 count = " << s.count(3) << endl;
    s.insert(5);
    cout << "size = " << s.size() << endl;
    cout << "3 count = " << s.count(3) << endl;
    s.erase(1);
    cout << "size = " << s.size() << endl;
    cout << "3 count = " << s.count(3) << endl;
    cout << "1 count = " << s.count(1) << endl;

    for (auto it = s.begin(); it != s.end(); it++)
        cout << *it << ' ';
    cout << endl;
}