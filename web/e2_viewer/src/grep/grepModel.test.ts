import { expect, test } from 'vitest'
import { GRepModel } from './grepModel.ts'
import type { IDrawable } from './drawable.ts';

test ('GRepModel', () => {
    const model = new GRepModel();
    const firstItem:IDrawable = {
        geometry: {
            type: 'point',
            position: [1,2,3]
        }
    };
    const secondItem:IDrawable = {
        geometry: {
            type: 'point',
            position: [4,5,6]
        }
    };

    expect(model.numDrawables()).toBe(0);
    model.addDrawable(firstItem);
    expect(model.numDrawables()).toBe(1);
    expect(model.drawable(0)).toBe(firstItem);
    model.addDrawable(secondItem);
    expect(model.numDrawables()).toBe(2);
    expect(model.drawlist()).toEqual([firstItem, secondItem]);
    expect(model.drawable(1)).toBe(secondItem);
    model.removeDrawable(0);
    expect(model.numDrawables()).toBe(1);
    expect(model.drawable(0)).toBe(secondItem);
    model.removeDrawable(0);
    expect(model.numDrawables()).toBe(0);
});


