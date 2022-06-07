#include <iostream>
#include "SignalSlot.h"



struct MyClass {
    int val;
    MyClass(int n) {
        val = n;
        std::cout << "call constructor " << val << " " << this << std::endl;
    }
    MyClass(const MyClass& ano) {
        val = ano.val;
        std::cout << "call copy constructor " << val << " " << &ano << "->" << this << std::endl;
    }
    MyClass(MyClass&& ano) {
        val = ano.val;
        ano.val = 0;
        std::cout << "call move constructor " << val << " " << &ano << "->" << this << std::endl;
    }
};

void slot_func(int a, MyClass b) {
    std::cout << "slot_func " << a << " " << b.val << std::endl;
}

int main() {
    auto lambda = [](int a, MyClass c) {
        std::cout << "lambda " << a << " " << c.val << std::endl;
    };
    RSL::Signal<int, MyClass> sig;
    sig.connect(slot_func);

    MyClass c(10);
    sig(1, std::move(c));

    std::cout << std::endl;

    MyClass d(20);
    sig.connect(lambda);
    sig(2, std::move(d));
    return 0;
}
