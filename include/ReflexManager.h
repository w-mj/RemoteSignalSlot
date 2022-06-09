//
// Created by WMJ on 2022/6/9.
//

#ifndef REMOTESIGNALSLOT_REFLEXMANAGER_H
#define REMOTESIGNALSLOT_REFLEXMANAGER_H

#include <string>

namespace RSL {
    class Reflexible {
    public:
        virtual std::string serialize() = 0;
        virtual void deserialize(const std::string& str) = 0;
    };

    class ReflexManager {

    };

}


#endif //REMOTESIGNALSLOT_REFLEXMANAGER_H
