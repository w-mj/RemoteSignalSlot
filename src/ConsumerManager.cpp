//
// Created by WMJ on 2022/6/9.
//

#include "ConsumerManager.h"

void RSL::ConsumerManager::addSignal(RSL::SignalEnclosure&& signal) {
    signalQueue.emplace(std::move(signal));
}

RSL::ConsumerManager &RSL::ConsumerManager::getInstance() {
    static ConsumerManager instance;
    return instance;
}
