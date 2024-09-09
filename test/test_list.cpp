#include <iostream>
#include "tiny_list.h"

using namespace Tiny;
using std::cout;
using std::endl;
using std::cin;

int main(void)
{
    list<int> ilist;
    cout << "size = " << ilist.size() << endl;

    ilist.push_back(0);
    ilist.push_back(1);
    ilist.push_back(2);
    ilist.push_front(3);
    ilist.push_front(4);
    cout << "size = " << ilist.size() << endl;

    for (auto it = ilist.begin(); it != ilist.end(); it++)
        cout << *it << ' ';
    cout << endl;
}
