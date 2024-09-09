#include <iostream>
#include <string>
#include "tiny_unordered_set.h"

using uset = Tiny::unordered_set<std::string>;

void lookup(const uset& s, const std::string& word)
{
    auto it = s.find(word);
    std::cout << word << ": " << (it == s.end() ? "not found" : "found") << std::endl;
}

int main(void)
{
    uset s;
    s.insert("kiwi");
    s.insert("plum");
    s.insert("apple");
    s.insert("mango");
    s.insert("apricot");
    s.insert("apricot");
    s.insert("banana");

    lookup(s, "mango");
    lookup(s, "apple");
    lookup(s, "durian");

    for (const auto& str : s)
        std::cout << str << ' ';
}