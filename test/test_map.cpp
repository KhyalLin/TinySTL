#include <iostream>
#include <string>
#include "tiny_map.h"

int main(void)
{
    Tiny::map<std::string, int> h;
    h["jjhou"] = 1;
    h["jerry"] = 2;
    h["jason"] = 3;
    h["jimmy"] = 4;
    
    std::pair<std::string, int> v("david", 5);
    h.insert(v);

    for (auto it = h.begin(); it != h.end(); it++)
        std::cout << it->first << ' ' << it->second << std::endl;
    
    int num = h["jjhou"];
    std::cout << num << std::endl;

    auto it = h.find("mchen");
    if (it == h.end())
        std::cout << "mchen not found" << std::endl;
    
    it = h.find("jerry");
    if (it != h.end())
        std::cout << "jerry found" << std::endl;
    it->second = 9;
    num = h["jerry"];
    std::cout << num << std::endl;
}