//
// Created by shane on 21/09/2024.
//

#ifndef KTXCOMPRESSOR_SUBSCRIPTION_H
#define KTXCOMPRESSOR_SUBSCRIPTION_H

#include <functional>
#include <utility>

namespace KTXCompressor {
    template<typename T>
    class Observable;

    class Subscription {
    public:
        using UnsubscribeFunc = std::function<void()>;

        Subscription(UnsubscribeFunc func) : unsubscribeFunc(std::move(func)) {}

    private:
        UnsubscribeFunc unsubscribeFunc;

    public :
        void Unsubscribe() {
            unsubscribeFunc();
        }
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_SUBSCRIPTION_H
