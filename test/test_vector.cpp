#include <iostream>
#include "tiny_vector.h"

using namespace Tiny;
using std::cin;
using std::cout;
using std::endl;

void display(const vector<int>& a)
{
    cout << "size = " << a.size() << endl;
    cout << "capacity = " << a.capacity() << endl;
}

void show(const vector<int>& a)
{
    for (int i = 0; i < a.size(); i++)
        cout << a[i] << ' ';
    cout << endl;
}

int main(void)
{
    vector<int> a(2, 9);
    display(a), show(a);

    for (int i = 1; i <= 4; i++)
    {
        a.push_back(i);
        display(a);
    }
    show(a);

    a.push_back(5);
    display(a), show(a);

    a.push_back(6);
    display(a), show(a);

    a.pop_back();
    a.pop_back();
    display(a);
    
    a.insert(a.begin() + 1, 3, 7);
    display(a), show(a);
}