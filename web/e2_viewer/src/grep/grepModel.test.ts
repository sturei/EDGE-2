import { expect, test } from 'vitest'
import { GRepModel } from './grepModel.ts'
import { GPoint } from './gitem.ts';

test ('GRepModel', () => {
    const model = new GRepModel();
    expect(model.numGItems()).toBe(0);
    const item = new GPoint(5);
    model.addGItem(item);
    expect(model.numGItems()).toBe(1);
    expect(model.gItem(0)).toBe(item);
    model.removeGItem(0);
    expect(model.numGItems()).toBe(0);
});


