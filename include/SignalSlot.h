//
// Created by WMJ on 2022/6/7.
//

#ifndef REMOTESIGNALSLOT_SIGNALSLOT_H
#define REMOTESIGNALSLOT_SIGNALSLOT_H

#include <string>
#include <functional>
#include <set>
#include "utils.h"

namespace RSL {
    enum class InvokeType {
        IMMEDIATELY,
        ASYNC
    };

    template<typename Ret, typename ...Args>
    class Connection {
        std::function<Ret(Args...)> callback;
        std::string name;
    public:
        Connection(std::function<Ret(Args...)> callback, std::string name):
            callback(std::move(callback)),
            name(std::move(name)) {}

        template<typename ...U>
        Ret operator()(U && ...args) {
            return callback(std::forward<U>(args)...);
        }
    };

    template<typename ...Args>
    class Signal {
    private:
        using ConnectionType = Connection<void, Args...>;
        using CallBackType = std::function<void(Args...)>;

        std::string name;
        std::vector<ConnectionType> connections;
    public:
        Signal() {
            name = generateID("slot", typeid(*this).name());
        }

        ConnectionType & connect(CallBackType func, std::string connectionName="") {
            if (connectionName.empty()) {
                connectionName = generateID(name, "connection");
            }
            connections.emplace_back(func, connectionName);
            return connections[connections.size() - 1];
        }

        template<typename...U>
        void operator()(U && ...args) {
            if (connections.size() == 1) {
                connections[0](std::forward<U>(args)...);
            } else {
                for (auto &conn: connections) {
                    conn(args...);
                }
            }
        }
    };
}

#endif //REMOTESIGNALSLOT_SIGNALSLOT_H
