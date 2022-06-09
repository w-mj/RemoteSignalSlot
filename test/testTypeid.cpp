//
// Created by WMJ on 2022/6/9.
//
#include "fmt/format.h"
#include <iostream>
#include <vector>

int main() {
    using namespace std;
    std::cout << typeid(std::string).name() << std::endl;
    std::cout << typeid(string).name() << std::endl;
    std::cout << typeid(vector<int>).name() << std::endl;
}