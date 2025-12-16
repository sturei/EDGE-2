#include <map>
#include <string>
#include <functional>
#include <iostream>
#include <nlohmann/json.hpp>

#include "document/document.h"

using json = nlohmann::json;

/**
 * The Document class holds the primary application state.
 * The primary concepts are Document, Store, Model, and Action.
 * A Document contains one or more Stores. 
 * Each Store contains a Model (the data).
 * State changes to the Model are made via Actions.
 * An application is expected to have a single Document instance. 
 * Some applications will employ just a single Store and Model within their single Document to store all of their state.
 * Other applications may choose to have multiple Stores and Models, each holding a different aspect of the state.
 * Client code subclasses Model to implement application-specific data structures.
 * Documents and Stores are not subclassed. They are designed to be generic, able to hold any type of Model.
 * Stores are concerned with lifecycle management of Models - things like undo/redo, 
 * persistence, locking, logging etc. (Most of these are not implemented yet).
 * Document provides the orchestration of Actions - registering action functions, dispatching actions to 
 * the appropriate function etc.
 * So, how does a client make a state change?
 * Firstly, note that all state changes are made in so-called "action functions". 
 * Action functions are regular functions, written by the application developer and registered
 * with the document at application startup time.
 * To invoke an action function, the client dispatches an Action to the Document. An Action is simply a {type, payload} pair.
 * The type is a string that identifies the action function to be invoked. The payload is a JSON object that contains any
 * parameters needed by the action function.
 * The Document looks up the action function registered for that action type, and invokes it with the payload.
 * Each action function then makes state changes to the Model via a state change callback on the Store that contains the Model.
 * It is this callback mechanism that enables Stores to manage lifecycle events, and to prevent uncontrolled state changes to Models.
 * As a convenience, the document also provides a post-state-change callback that gets invoked after each state change.
 * This can be used to trigger UI updates or other operations that are common to all state changes in an application.
 * Actions are chainable (one action function can dispatch another action function via the appropriate interface on the Document)
 * Actions are replayable (the Document could log all actions to a file, and then replay them later to restore state)
 * Actions are simple (just JSON data - no function pointers or other complexities)
 * Action can be dispatched across process boundaries by serializing to JSON text, and sending via stdin/stdout, sockets etc.
 * There is a class called DocumentService that provides a simple stdin/stdout IPC mechanism for dispatching actions to a Document 
 * and receiving actions in return.
 */

namespace e2 {

    Document::~Document() {
        for (auto& pair : m_stores) {
            delete pair.second;
        }
    }

    Store& Document::storeAt(const std::string& key) const {
        return *m_stores.at(key);
    }

    void Document::registerActionFunction(const ActionDef& action) {
        m_actionFunctions[action.type] = action.function;
    }

    ActionResult Document::dispatchAction(const ActionSpec& action) {
        try {
            //std::cerr << "Dispatching action of type: " << action.type << " with payload: " << action.payload.dump() << std::endl;  //--- DEBUG ---
            auto it = m_actionFunctions.find(action.type);
            if (it == m_actionFunctions.end()) {
                // std::cerr << "Unknown action type: " << action.type << std::endl;   <--- DEBUG ---
                return ActionResult::UNKNOWN_ACTION;
            }
            auto actionFunction = it->second;
            actionFunction(*this, action.payload);
        } catch (const json::exception& e) {
            std::cerr << "Error dispatching action: " << action.type << " - " << e.what() << std::endl;  // <--- DEBUG ---
            return ActionResult::INVALID_PAYLOAD;
        } catch (const std::exception& e) {
            // std::cerr << "Error dispatching action: " << action.type << " - " << e.what() << std::endl; // <--- DEBUG ---
            return ActionResult::INTERNAL_ERROR;
        }
        return ActionResult::SUCCESS;
    }

    void Document::dispatchClientAction(const ActionSpec& action) {
        m_clientActions.push_back(action);
    }

    const std::vector<ActionSpec>& Document::getClientActions() const {
        return m_clientActions;
    }

    void Document::clearClientActions() {
        m_clientActions.clear();
    }

    std::ostream& operator<<(std::ostream& os, const Document& doc) {
        os << "Document with " << doc.m_stores.size() << " stores." << std::endl;
        for (const auto& pair : doc.m_stores) {
            os << "Store key: " << pair.first << std::endl;
            os << *(pair.second) << std::endl;
        }
        return os;
    }   
};  