import { GRepModel } from './grepModel.ts'
import type { IDrawable } from './drawable.ts';
import { describe, expect, it } from 'vitest'
import type { IShaderNode } from './shaderNode.ts';

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
        const rootNode: IShaderNode = {
            pathName: "root",
            type: "union",
            parameters: {},
            childIndices: [1, 2]
        };
        const firstNode: IShaderNode = {
            pathName: "root/node1",
            type: "sphere",
            parameters: {radius: 1.0}
        };
        const secondNode: IShaderNode = {
            pathName: "root/node2",
            type: "block",
            parameters: {width: 1.0, height: 2.0, depth: 3.0}
        };

        expect(model.numShaderNodes()).toBe(0);
        model.addShaderNode(rootNode);
        expect(model.numShaderNodes()).toBe(1);
        expect(model.shaderNode(0)).toBe(rootNode);
        model.addShaderNode(firstNode);
        expect(model.numShaderNodes()).toBe(2);
        expect(model.shaderNodes()).toEqual([rootNode, firstNode]);
        expect(model.shaderNode(1)).toBe(firstNode);
        model.addShaderNode(secondNode);
        expect(model.numShaderNodes()).toBe(3);
        expect(model.shaderNodes()).toEqual([rootNode, firstNode, secondNode]);
        expect(model.shaderNode(2)).toBe(secondNode);

        
    });
});


