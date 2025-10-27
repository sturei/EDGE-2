import { expect, test } from 'vitest'
import { GRepModel } from './grepModel.ts'
import { GPoint } from './gitem.ts';

test ('GRepModel', () => {
    const model = new GRepModel();
    const firstItem = new GPoint(5);
    const secondItem = new GPoint(4);

    expect(model.numGItems()).toBe(0);
    model.addGItem(firstItem);
    expect(model.numGItems()).toBe(1);
    expect(model.gItem(0)).toBe(firstItem);
    model.addGItem(secondItem);
    expect(model.numGItems()).toBe(2);
    expect(model.gItem(1)).toBe(secondItem);
    model.removeGItem(0);
    expect(model.numGItems()).toBe(1);
    expect(model.gItem(0)).toBe(secondItem);
    model.removeGItem(0);
    expect(model.numGItems()).toBe(0);
});


