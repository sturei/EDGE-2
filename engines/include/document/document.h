#pragma once
#include <map>
#include <string>
#include <functional>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>

#include "document/store.h"

using json = nlohmann::json;

namespace e2 {

    class Document;

    struct ActionSpec {
        std::string type;
        json payload;
    };

    struct ActionDef {
        std::string type;
        std::function<void(Document&, const json&)> function;
    };

    enum class ActionResult {
        SUCCESS,
        UNKNOWN_ACTION,
        INVALID_PAYLOAD,
        INTERNAL_ERROR
    };
    
    class Document {
        public:

            Document() {}
            Document(const std::map<std::string, Store*>& stores) : m_stores(stores) {}
            ~Document();
            virtual Store& storeAt(const std::string& key) const;     // it's virtual to make it easy to mock out in tests   
            void registerActionFunction(const ActionDef& action);
            ActionResult dispatchAction(const ActionSpec& action);
            void dispatchClientAction(const ActionSpec& action);
            const std::vector<ActionSpec>& getClientActions() const;
            void clearClientActions();
            friend std::ostream& operator<<(std::ostream& os, const Document& doc);

        private:
            std::map<std::string, Store*> m_stores;                                                     // stores owned by this Document
            std::map<std::string, std::function<void(Document&, const json&)>> m_actionFunctions;       // action functions registered with this Document
            std::vector<ActionSpec> m_clientActions;                                                    // client actions are buffered here
    };
}
