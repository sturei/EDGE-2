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
 * If context is needed, use a pointer-to-member of a class containing the relevant context, or use a lambda that captures the relevant context.
 * The stateChangeCallback is called by the Store's changeState method. This is the ONLY recommended way for clients to gain write-access to the model.
 * Implementation notes:
 * The intent of the design is that each store is an independent resource, that can be locked/unlocked independently of the other stores.
 * In future, changeState will check for locks etc before calling the stateChangeCallback.
 * The postStateChangeCallback is a function that takes no arguments and returns void. It is called by changeState after the stateChangeCallback has returned, to notify observers of the change.
 */

namespace e2 {

    class Store {
        public:
            Store(Model* model, std::function<void()> postStateChangeCallback = nullptr) : m_model(model), m_postStateChangeCallback(postStateChangeCallback) {}
            ~Store() {
                delete m_model;
            }
            // changeState provides write access to the model via a callback function.
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
            Model* m_model = nullptr;    // Store owns the model
            std::function<void()> m_postStateChangeCallback = nullptr;
    };

};

