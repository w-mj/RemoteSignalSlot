//
// Created by WMJ on 2022/6/9.
//

#ifndef REMOTESIGNALSLOT_SIGNALENCLOSURE_H
#define REMOTESIGNALSLOT_SIGNALENCLOSURE_H

#include <string>
#include <any>
#include <vector>
#include <tuple>

namespace RSL {
    class SignalEnclosure {
        std::string signalName;
        std::vector<std::any> data;
    public:
        explicit SignalEnclosure(std::string signalName);

        template<typename ...U>
        SignalEnclosure& setArgs(U && ...args) {
            // 这里如果不用Any直接构造vector会多一次对象的复制构造
            std::array<std::any, sizeof...(U)> ta {std::forward<U>(args)...};
            data.clear();
            data.resize(sizeof...(U));
            std::move(ta.begin(), ta.end(), data.begin());
            return *this;
        }
    };
}

#endif //REMOTESIGNALSLOT_SIGNALENCLOSURE_H
