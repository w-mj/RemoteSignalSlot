#include <any>
#include <vector>
#include <array>
#include <iostream>

using namespace std;

struct MyClass {
    int val;
    explicit MyClass(int n) {
        val = n;
        std::cout << "call constructor " << val << " " << this << std::endl;
    }
    MyClass(const MyClass& ano) {
        val = ano.val;
        std::cout << "call copy constructor " << val << " " << &ano << "->" << this << std::endl;
    }
    MyClass(MyClass&& ano)  noexcept {
        val = ano.val;
        ano.val = 0;
        std::cout << "call move constructor " << val << " " << &ano << "->" << this << std::endl;
    }
};

template<typename...U>
void buildFunc(U && ...args) {
    array<any, 2> va = {std::forward<U>(args)...};
    vector<any> vc;
    vc.resize(2);
    move(va.begin(), va.end(), vc.begin());
    for (auto& i: vc) {
        cout << i.type().name() << endl;
    }
}

int main() {
    MyClass mc(10);
    buildFunc(1, mc);
    buildFunc(2, std::move(mc));
}
