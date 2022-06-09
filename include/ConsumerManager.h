//
// Created by WMJ on 2022/6/9.
//

#ifndef REMOTESIGNALSLOT_CONSUMERMANAGER_H
#define REMOTESIGNALSLOT_CONSUMERMANAGER_H

#include <queue>
#include "SignalEnclosure.h"

namespace RSL {

class ConsumerManager {
    std::queue<SignalEnclosure> signalQueue;
    ConsumerManager() = default;

public:

    // 添加信号，信号一定会被移动，即一个信号只能被添加一次
    void addSignal(SignalEnclosure&& signal);

    static ConsumerManager& getInstance();

};

}


#endif //REMOTESIGNALSLOT_CONSUMERMANAGER_H
