/** Stores manage the lifecycle of models. 
 * Implementation notes:
 * The interesting part of the design is that the ONLY way to change the state of a model is through the changeState() method of the store that owns it.
 * The store's regular getModel() method returns a readonly view of the model to discourage direct mutation.
 * Could add undo/redo in future. 
 * Could add support for asynchronous state changes in future. (each action would get its own writable copy)
 * Could add logging here in future too.
 * Need to add an "isMutable" flag to the model, to prevent mutation except ubsude a changeState().
 * 
*/

import { Model } from './model.ts';

export class Store {
    private model: Model;
    private postStateChangeCallback: (() => void) | null = null;

    constructor(model: Model, postStateChangeCallback?: () => void) {
        this.model = model;
        this.postStateChangeCallback = postStateChangeCallback || null;
    }

    changeState(stateChangeCallback: (model: Model) => void): void {
        if (this.model) {
            stateChangeCallback(this.model);
            if (this.postStateChangeCallback) {
                this.postStateChangeCallback();
            }
        }
    }

    getModel(): Readonly<Model>  {
        return this.model;
    }

    toString(): string {
        return `Store { model: ${this.model} }`;
    }
}
