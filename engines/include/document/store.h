#pragma once
#include <functional>
#include "document/model.h"

namespace e2 {
    class Store {
        public:
            Store(Model* model = nullptr, std::function<void()> postStateChangeCallback = nullptr) : m_model(model), m_postStateChangeCallback(postStateChangeCallback) {}
            ~Store();
            virtual void changeState(std::function<void(Model*)> stateChangeCallback);    // virtual to make it easy to mock out in tests
            const Model* model() const { return m_model; }
            friend std::ostream& operator<<(std::ostream& os, const Store& store);
        private:
            Model* m_model = nullptr;    // Store owns the model
            std::function<void()> m_postStateChangeCallback = nullptr;
    };
};

