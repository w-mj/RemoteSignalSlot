//
// Created by WMJ on 2022/6/7.
//

#ifndef REMOTESIGNALSLOT_SIGNALSLOT_H
#define REMOTESIGNALSLOT_SIGNALSLOT_H

#include <string>
#include <functional>
#include <set>
#include <array>
#include <any>
#include "utils.h"

namespace RSL {

    template<typename Ret, typename ...Args>
    class SlotBase {
        std::string name;

        template<size_t ...S>
        Ret callFunc(std::index_sequence<S...>, const std::array<std::any, sizeof...(Args)>& args) {
            return invoke(std::any_cast<typename std::tuple_element<S, std::tuple<Args...>>::type>(args[S])...);
        }
    public:
        SlotBase(std::string name): name(std::move(name)) {
        }

        [[nodiscard]] const std::string& getName() const {
            return name;
        }
        virtual Ret invoke(Args...) = 0;

        Ret invokeWithAny(const std::array<std::any, sizeof...(Args)>& arg) {
            return callFunc(std::make_index_sequence<sizeof...(Args)>{}, arg);
        }

    };

    template<typename Ret, typename ...Args>
    class SlotFunc: public SlotBase<Ret, Args...> {
        std::function<Ret(Args...)> callback;
    public:
        SlotFunc(std::function<Ret(Args...)> callback, std::string name):
            SlotBase<Ret, Args...>(std::move(name)), callback(std::move(callback)){}


        Ret invoke(Args ...args) {
            return callback(std::move(args)...);
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

        Ret invoke(Args ...args) {
            return ((*obj).*func)(std::move(args)...);
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
        static constexpr size_t argSize = sizeof...(Args);

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
        void emit(U && ...args) {
            if (connections.size() == 1) {
                connections[0]->invoke(std::forward<U>(args)...);
            } else {
                for (auto &conn: connections) {
                    conn->invoke(args...);
                }
            }
        }

        template<typename ...U>
        void operator()(U && ...args) {
            return emit(std::forward<U>(args)...);
        }

        template<typename A>
        std::enable_if_t<std::is_same_v<std::remove_reference_t<A>, std::array<std::any, argSize>>>
        emitWithAny(A && args) {
            if (connections.size() == 1) {
                connections[0]->invokeWithAny(std::forward<A>(args));
            } else {
                for (auto &conn: connections) {
                    conn->invokeWithAny(args);
                }
            }
        }
    };
}

#endif //REMOTESIGNALSLOT_SIGNALSLOT_H
