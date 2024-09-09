#include <iostream>
#include "tiny_priority_queue.h"

using namespace Tiny;
using std::cin;
using std::cout;
using std::endl;

int main(void)
{
    int ia[9] = { 0, 1, 2, 3, 4, 8, 9, 3, 5 };
    priority_queue<int> q;
    for (int x : ia) q.push(x);
    
    cout << "size = " << q.size() << endl;
    cout << q.top() << endl;

    while (!q.empty()) {
        cout << q.top() << ' ';
        q.pop();
    }
}