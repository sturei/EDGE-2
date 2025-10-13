#pragma once
#include <map>
#include <string>
#include <nlohmann/json.hpp>

#include "document/store.h"

using json = nlohmann::json;

/**
 * The Document class provides access to the application data model.
 * State changes to the data model are made via actions.
 * Each action is dispatched to the Document as a Plain Old Data (POD) structure {type, payload*} pair.
 * The document in turn invokes a registered "action function" corresponding to the action type. The action function takes the payload
 * as input and performs some operations that modify the application state via a state change callback on one or more stores.
 * Implementation notes:
 * The Document class contains a map of Stores, each Store containing a Model. Stores handle lifecycle events on Models.
 * The Document class takes ownerahip of the stores it is given.
 * Actions are intended to be asynchronous and stores are intended to be independently lockable. Not implemented yet!
 * Various levels of error checking, logging etc will be added later.
 * Actions are chainable (one action function can call another action function via the appropriate interface on the Document). Not implemented yet!
 * A series of actions is intended to be replayable - not implemented yet!
 * Consider moving ActionSpec inside Document, because it is specific to Document.
 */

namespace e2 {
    class ActionSpec {
        public:
            std::string type;
            json payload;
    };
    class Document {
        public:
            Document() {}
            Document(const std::map<std::string, Store*>& stores) : m_stores(stores) {}
            ~Document() {
                for (auto& pair : m_stores) {
                    delete pair.second;
                }
            }
            Store* storeAt(const std::string& key) {
                return m_stores.at(key);
            }
            void registerActionFunction(const std::string& actionType, std::function<void(Document*, const json&)> actionFunction) {
                m_actionFunctions[actionType] = actionFunction;
            }
            void dispatchAction(const ActionSpec& action) {
                auto it = m_actionFunctions.find(action.type);
                if (it != m_actionFunctions.end()) {
                    it->second(this, action.payload);
                }
            }
            friend std::ostream& operator<<(std::ostream& os, const Document& doc) {
                os << "Document with " << doc.m_stores.size() << " stores." << std::endl;
                for (const auto& pair : doc.m_stores) {
                    os << "Store key: " << pair.first << std::endl;
                    os << *(pair.second) << std::endl;
                }
                return os;
            }   
        private:
            std::map<std::string, Store*> m_stores; // Document takes ownership of the stores
            std::map<std::string, std::function<void(Document*, const json&)>> m_actionFunctions;
    };
};  