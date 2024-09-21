//
// Created by shane on 21/09/2024.
//

#ifndef KTXCOMPRESSOR_OBSERVABLE_H
#define KTXCOMPRESSOR_OBSERVABLE_H

#include <vector>
#include <functional>
#include <memory>
#include "Subscription.h"

namespace KTXCompressor {

    template<typename T>
    class Observable {
    private:
        T value;
        std::vector<std::function<void(T)>> observers;  // List of observers
    public :
        Observable() {}

        // Subscribe to value changes
        Subscription Subscribe(std::function<void(T)> observer) {
            observers.push_back(observer);

            // Return a Subscription that can be used to unsubscribe
            return Subscription([this, observer]() {
                this->observers.erase(
                        std::remove_if(this->observers.begin(), this->observers.end(),
                                       [&observer](const std::function<void(T)> &obs) {
                                           return obs.template target<void>() == observer.template target<void>();
                                       }),
                        this->observers.end());
            });
        }

        void SetValue(T newValue) {
            if (value != newValue) {
                value = newValue;
                NotifyObservers();
            }
        }

        T GetValue() const {
            return value;
        }

    private:
        void NotifyObservers() {
            for (const auto &observer: observers) {
                observer(value);
            }
        }
    };

} // KTXCompressor

#endif //KTXCOMPRESSOR_OBSERVABLE_H
