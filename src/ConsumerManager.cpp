//
// Created by WMJ on 2022/6/9.
//


#include <thread>
#include "ConsumerManager.h"

void RSL::ConsumerManager::addSignal(RSL::SignalEnclosure&& signal) {
    signalQueue.emplace(std::move(signal));
}

RSL::ConsumerManager &RSL::ConsumerManager::getInstance() {
    static ConsumerManager instance;
    return instance;
}

void RSL::ConsumerManager::networkLoop() {
;

}

void RSL::ConsumerManager::stop() {
    running = false;
}
