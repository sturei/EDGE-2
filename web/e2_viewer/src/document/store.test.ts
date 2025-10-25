import { describe, expect, beforeEach, test } from 'vitest';
import { Model } from '../document/model';
import { Store } from '../document/store';

class ZooModelFixture implements Model {
    animals: string[] = [];
    toString(): string {
        return `ZooModelFixture(animals=${this.animals.join(',')})`;
    }
}

describe('Store', () => {
    let store: Store;
    
    beforeEach(() => {
        const zooModel = new ZooModelFixture();
        store = new Store(zooModel);
    });
    
    test('ChangeState', () => {
        let callbackCalled = false;
        const animal = 'Lion';
        
        store.changeState((model: Model) => {
            callbackCalled = true;
            const zooModel = model as ZooModelFixture;
            zooModel.animals.push(animal);
        });
        
        expect(callbackCalled).toBe(true);
        const zooModel = store.getModel() as ZooModelFixture;
        expect(zooModel.animals).toHaveLength(1);
        expect(zooModel.animals[0]).toBe('Lion');
    });
});

