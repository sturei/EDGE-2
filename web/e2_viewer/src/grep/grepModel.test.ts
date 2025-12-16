import { GRepModel } from './grepModel.ts'
import type { IDrawable } from './drawables/drawable.ts';
import { describe, expect, it } from 'vitest'
import type { ISdfNode } from './nodes/sdfNode.ts';

describe('GRepModel', () => {
    it('should handle drawable items correctly', () => {
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
    });
    it('should handle shader nodes correctly', () => {
        const model = new GRepModel();
        const rootNode: ISdfNode = {
            pathName: "root",
            type: "union",
            childIndices: [1, 2]
        };
        const firstNode: ISdfNode = {
            pathName: "root/node1",
            type: "sphere",
            radius: 1.0
        };
        const secondNode: ISdfNode = {
            pathName: "root/node2",
            type: "block",
            width: 1.0, 
            height: 2.0, 
            depth: 3.0
        };

        expect(model.numSdfNodes()).toBe(0);
        model.addSdfNode(rootNode);
        expect(model.numSdfNodes()).toBe(1);
        expect(model.sdfNode(0)).toBe(rootNode);
        model.addSdfNode(firstNode);
        expect(model.numSdfNodes()).toBe(2);
        expect(model.sdfNodes()).toEqual([rootNode, firstNode]);
        expect(model.sdfNode(1)).toBe(firstNode);
        model.addSdfNode(secondNode);
        expect(model.numSdfNodes()).toBe(3);
        expect(model.sdfNodes()).toEqual([rootNode, firstNode, secondNode]);
        expect(model.sdfNode(2)).toBe(secondNode);
        
    });
});


