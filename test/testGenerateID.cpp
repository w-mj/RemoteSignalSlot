//
// Created by WMJ on 2022/6/7.
//
#include "utils.h"
#include <iostream>


int main() {
    std::cout << RSL::generateID("p1", 23, "45") << std::endl;
    std::cout << RSL::generateID("p1", 23.1, "45") << std::endl;
    std::cout << RSL::generateID("p2", 23, std::string("abc")) << std::endl;
}