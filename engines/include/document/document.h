#pragma once
#include <map>
#include <string>
#include <functional>
#include <iostream>
#include <nlohmann/json.hpp>

#include "document/store.h"

using json = nlohmann::json;

namespace e2 {
    
    class Document {
        public:

            struct ActionSpec {
                std::string type;
                json payload;
            };

            struct ActionDef {
                std::string type;
                std::function<void(Document*, const json&)> function;
            };
    
            Document() {}
            Document(const std::map<std::string, Store*>& stores) : m_stores(stores) {}
            ~Document();
            Store* storeAt(const std::string& key);
            void registerActionFunction(const ActionDef& action);
            bool dispatchAction(const ActionSpec& action);
            friend std::ostream& operator<<(std::ostream& os, const Document& doc);

        private:
            std::map<std::string, Store*> m_stores; // Document takes ownership of the stores
            std::map<std::string, std::function<void(Document*, const json&)>> m_actionFunctions;
    };
};  