//
// Created by WMJ on 2022/6/12.
//

#ifndef REMOTESIGNALSLOT_NETWORKMANAGER_H
#define REMOTESIGNALSLOT_NETWORKMANAGER_H

#include "SignalSlot.h"

namespace RSL {
    class NetworkManager {
        bool running = false;

        void networkLoop();
    public:
        Signal<SignalEnclosure> remoteSignal;
        Signal<int> networkInited;

        void start();
        void stop();

    };
}


#endif //REMOTESIGNALSLOT_NETWORKMANAGER_H
