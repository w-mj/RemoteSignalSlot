//
// Created by WMJ on 2022/6/7.
//

#ifndef REMOTESIGNALSLOT_UTILS_H
#define REMOTESIGNALSLOT_UTILS_H

#include <string>
#include <sstream>
#include <mutex>
#include <type_traits>
#include <memory>

namespace RSL {
    std::string generateUUID(const std::string& prefix="");

    unsigned inc();

    template<typename ...Args>
    std::string generateID(Args... args) {
        static_assert(sizeof...(Args) > 0);
        std::stringstream ss;
        auto wrapper = [&ss](const auto& arg) {
            ss << arg;
            return "-";
        };
        (ss << ... << wrapper(args));
        ss << inc();
        return ss.str();
    }

    template<typename ...Args>
    struct Dummy {};
}

#endif //REMOTESIGNALSLOT_UTILS_H
