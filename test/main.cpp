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

    void call(int a, MyClass self) {
        std::cout << "member func " << a << " " << self.val << std::endl;
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
    sig(0, c);
    sig(1, std::move(c));
    std::cout << c.val << std::endl;

    std::cout << std::endl;

    MyClass d(20);
    auto conn = sig.connect(lambda);
    sig(2, std::move(d));

    sig.disconnect(conn);

    std::cout << std::endl;
    sig.connect(&c, &MyClass::call);

    MyClass e(30);
    sig(3, e);

    std::array<std::any, 2> args = {100, e};
    sig.emitWithAny(args);
    return 0;
}
