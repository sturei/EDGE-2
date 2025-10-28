import { describe, it, expect, beforeEach } from 'vitest';
import { Document } from '../document/document';
import { Store } from '../document/store';
import { type ActionDef, type ActionSpec } from '../document/document';
import { Model } from '../document/model';  

class ZooModelFixture implements Model {
    animals: string[] = [];    
    toString(): string {
        return `ZooModelFixture(animals=${this.animals.join(',')})`;
    }
}

function addAnimal(doc: Document, payload: { species: string }): void {
        const species = payload.species;
        if (!species) {
            throw new Error("Payload must contain 'species' field");
        }   
        const store = doc.getStore("zoo");
        store.changeState((model) => {
            const zooModel = model as ZooModelFixture;
            zooModel.animals.push(species);
        });
    }
const addAnimalDef: ActionDef = { type: "addAnimal", function: addAnimal };

describe('Document', () => {
    let document: Document;
    
    beforeEach(() => {
        const zooModel = new ZooModelFixture();
        const zooStore = new Store(zooModel);
        document = new Document(new Map([[ 'zoo', zooStore ]]));
    });
    
    it('should construct with default constructor', () => {
        const doc = new Document();
        expect(doc).toBeDefined();
    });
    
    it('should add animal via direct action call', () => {
        const payload = { species: "Elephant" };
        addAnimal(document, payload);
        const store = document.getStore("zoo");
        const zooModel = store.getModel() as ZooModelFixture;
        expect(zooModel.animals).toHaveLength(1);
        expect(zooModel.animals[0]).toBe("Elephant");
    });
    
    it('should register and dispatch action', () => {
        document.registerActionFunction(addAnimalDef);
        const action: ActionSpec = {
            type: "addAnimal",
            payload: { species: "Giraffe" }
        };
        document.dispatchAction(action);
        const store = document.getStore("zoo");
        const zooModel = store.getModel() as ZooModelFixture;
        expect(zooModel.animals).toHaveLength(1);
        expect(zooModel.animals[0]).toBe("Giraffe");
    });

    it('should throw error for unregistered action', () => {
        const action: ActionSpec = {
            type: "nonExistentAction",
            payload: {}
        };
        expect(() => document.dispatchAction(action)).toThrowError("No action function registered for action type: nonExistentAction");
    });
    
    it('should throw error for bad payload', () => {
        document.registerActionFunction(addAnimalDef);
        const action: ActionSpec = {
            type: "addAnimal",
            payload: { anythingBut: "Giraffe" }
        };
        expect(() => document.dispatchAction(action)).toThrowError("Payload must contain 'species' field");
    });

    it('should output correct string representation', () => {
        const docStr = document.toString();
        expect(docStr).toContain("Document with 1 stores");
    });
});
