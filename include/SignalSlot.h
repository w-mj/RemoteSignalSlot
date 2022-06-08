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

    template<typename Ret, typename ...Args>
    class SlotBase {
        std::string name;
    public:
        SlotBase(std::string name): name(std::move(name)) {
        }

        const std::string& getName() const {
            return name;
        }
        virtual Ret operator()(Args...) = 0;
    };

    template<typename Ret, typename ...Args>
    class SlotFunc: public SlotBase<Ret, Args...> {
        std::function<Ret(Args...)> callback;
    public:
        SlotFunc(std::function<Ret(Args...)> callback, std::string name):
            SlotBase<Ret, Args...>(std::move(name)), callback(std::move(callback)){}

        Ret operator()(Args ...args) {
            return callback(args...);
        }
    };

    template<typename Class, typename Ret, typename ...Args>
    class SlotMember: public SlotBase<Ret, Args...> {
        using FuncType = Ret(Class::*)(Args...);
        Class *obj;
        FuncType func;
    public:
        SlotMember(Class* obj, FuncType func, std::string name):
            SlotBase<Ret, Args...>(name), obj(obj), func(func) {

        }

        Ret operator()(Args ...args) {
            return ((*obj).*func)(args...);
        }
    };


    class Connection {
        std::string signalName;
        std::string slotName;
    public:
        Connection(std::string signalName, std::string slotName):
        signalName(std::move(signalName)),
        slotName(std::move(slotName)) {}

        [[nodiscard]] const std::string& getSignalName() const {
            return signalName;
        }

        [[nodiscard]] const std::string& getSlotName() const {
            return slotName;
        }
    };

    template<typename... A>
    class Signal: public Signal<void(A...)>{};

    template<typename Ret, typename ...Args>
    class Signal<Ret(Args...)> {
    private:
        using SlotType = SlotBase<Ret, Args...>;
        using CallBackType = std::function<Ret(Args...)>;

        std::string name;
        std::vector<std::unique_ptr<SlotBase<Ret, Args...>>> connections;
    public:
        Signal() {
            name = generateID("slot", typeid(*this).name());
        }

        Connection connect(CallBackType func, std::string slotName="") {
            if (slotName.empty()) {
                slotName = generateID(name, "connection");
            }
            connections.push_back(std::make_unique<SlotFunc<Ret, Args...>>(func, slotName));

            return {name, slotName};
        }

        template<typename Class>
        Connection connect(Class *obj, Ret(Class::*func)(Args...), std::string slotName="") {
            if (slotName.empty()) {
                slotName = generateID(name, "connection");
            }
            connections.push_back(std::make_unique<SlotMember<Class, Ret, Args...>>(obj, func, slotName));

            return {name, slotName};
        }

        void disconnect(const Connection& conn) {
            if (conn.getSignalName() != name) {
                return;
            }

            connections.erase(std::remove_if(connections.begin(), connections.end(),
                           [&conn](const auto& ptr){
               return ptr->getName() == conn.getSlotName();
            }), connections.end());
        }

        template<typename...U>
        void operator()(U && ...args) {
            if (connections.size() == 1) {
                connections[0]->operator()(std::forward<U>(args)...);
            } else {
                for (auto &conn: connections) {
                    conn->operator()(args...);
                }
            }
        }
    };
}

#endif //REMOTESIGNALSLOT_SIGNALSLOT_H
