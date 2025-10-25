/** The Document/Store/Model system manages te primary application state.
 * 
 * The core concepts are Document, Store, Model, and Action.
 * 
 * A Document is the top-level container, owner and manager of application state. All state changes are initiated via a Document.
 *
 * Stores lie between Documents and Models. Stores protect Models from direct mutation, and provide controlled access for changing Model state. In future,
 * stores may also provide additional services such as undo/redo, state change logging, and asynchronous state changes.
 * 
 * Models represent the actual application state. Note that Document and Store are not intended to be subclassed. All of the specialization for
 * particular applications is handled by Model subclasses.
 *
 * An Action is a {type, payload} pair that specifies the operation to be performed and the payload data for that operation.
 * The operations (so-called "action functions") are registered with the Document during application initialization.
 * 
 * To initiate a state change, a client dispatches an Action to the Document, which looks up the corresponding action function and
 * invokes it with the supplied payload. The action function then uses the Store's changeState() method to modify the Model state in a controlled way -
 * specifically, the action function does not have direct write access to the Model - instead it must modify the Model inside a callback passed to it by the store's
 * changeState() method.
 *
 * Implementation notes:
 * TODO: add an isMutable flag to Store to prevent changeState calls outside of the callback.
 * TODO: add an "active action" concept to Document to track the currently executing action and prevent state changes outside of an action context.
 */

import { Store } from './store'; // Adjust import path as needed

export interface ActionSpec {
    type: string;
    payload: any;
}

export interface ActionDef {
    type: string;
    function: (document: Document, payload: any) => void;
}

export class Document {
    private stores: Map<string, Store> = new Map();
    private actionFunctions: Map<string, (document: Document, payload: any) => void> = new Map();

    constructor(stores?: Map<string, Store>) {
        if (stores) {
            this.stores = stores;
        }
    }

    storeAt(key: string): Store | undefined {
        return this.stores.get(key);
    }

    registerActionFunction(action: ActionDef): void {
        this.actionFunctions.set(action.type, action.function);
    }

    dispatchAction(action: ActionSpec): boolean {
        const actionFunction = this.actionFunctions.get(action.type);
        if (actionFunction) {
            actionFunction(this, action.payload);
            return true;
        }
        return false;
    }

    toString(): string {
        // Implement string representation as needed
        return `Document with ${this.stores.size} stores`;
    }
}
