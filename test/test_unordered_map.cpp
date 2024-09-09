#include <iostream>
#include <string>
#include "tiny_unordered_map.h"
using namespace std;

void show(Tiny::unordered_map<string, int>& h, const string& s)
{
    cout << s + " -> " << h[s] << endl;
}

int main(void)
{
    Tiny::unordered_map<string, int> h;
    h["january"] = 31;
    h["february"] = 28;
    h["march"] = 31;
    h["april"] = 30;
    h["may"] = 31;
    h["june"] = 30;
    h["july"] = 31;
    h["august"] = 30;
    h["september"] = 31;
    h["october"] = 30;
    h["november"] = 31;
    h["december"] = 31;

    show(h, "september");
    show(h, "june");
    show(h, "february");
    show(h, "december");

    for (const auto& [s, n] : h)
        cout << s << ' ' << n << endl;
}