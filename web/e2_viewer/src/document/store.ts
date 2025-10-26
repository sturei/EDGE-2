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
