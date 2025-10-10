#pragma once
#include <functional>
#include "document/model.h"

/**
 * Store handles lifecyle events on Models.
 * It takes ownership of the model it is given.
 * It provides read-only access to the model through const accessors, and write access to the model via a state change callback.
 * It provides a notification mechanism (post change callback) to notify observers of changes to the model.
 * In future, it will provide undo/redo functionality, and probably serialization, locking and other lifecycle management functionality.
 * Probably there will be beginTransaction/endTransaction methods to allow batching of multiple state changes into a single undoable action.
 * More about stateChangeCallback:
 * The stateChangeCallback is a function that takes a Model* and returns void. In future it may return a status code or throw an exception to indicate success or failure.
 * The stateChangeCallback should make all the changes it needs to the model, and then return.
 * If context is needed, use a pointer-to-member (simulating a closure in languages that support closures) (TODO: investigate using a lambda or std::bind).
 * The stateChangeCallback is called by the Store's changeState method. This is the ONLY recommended way for clients to gain write-access to the model.
 * In future, changeState will check for locks etc before calling the stateChangeCallback.
 * The postStateChangeCallback is a function that takes no arguments and returns void. It is called by changeState after the stateChangeCallback has returned, to notify observers of the change.
 * Probably Model* model should be a unique_ptr to make ownership clearer.
 * Also start using nullptr rather than 0 for pointers.
 */

namespace e2 {

    class Store {
        public:
            Store(Model* m = 0, std::function<void()> postStateChangeCallback = 0) : m_model(m), m_postStateChangeCallback(postStateChangeCallback) {}
            ~Store() {
                delete m_model;
            };
            void changeState(std::function<void(Model*)> stateChangeCallback) {
                if (m_model) {
                    stateChangeCallback(m_model);
                    if (m_postStateChangeCallback) {
                        m_postStateChangeCallback();
                    }
                }   
            }
            const Model* model() const {
                return m_model;
            }
        private:
            Model* m_model = 0;
            std::function<void()> m_postStateChangeCallback = 0;
    };

};

