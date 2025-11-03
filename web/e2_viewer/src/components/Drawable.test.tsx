import { describe, it, expect } from 'vitest';
import { r3fFromDrawable } from './Drawable';
import type { IDrawable } from '../grep/drawable';
import "@testing-library/jest-dom";
import { prettyDOM, render } from '@testing-library/react';
import ResizeObserver from 'resize-observer-polyfill'

window.ResizeObserver = ResizeObserver

/**
 * @vitest-environment jsdom
 */

describe('r3fFromDrawable', () => {
    it('should return undefined when no geometry is provided', () => {
        const drawable: IDrawable = {};
        const result = r3fFromDrawable(drawable);
        expect(result).toBeUndefined();
    });

    it('should render box geometry with default color when no appearance is provided', () => {
        const drawable: IDrawable = {
            geometry: { type: 'box', width: 2, height: 3, depth: 4 }
        };
        const result = r3fFromDrawable(drawable);
        expect(result).toBeDefined();

        // Implementation note on the alternatives I tried already:
        // 1. react-test-renderer is deprecated. 
        // 2. ReactThreeTestRenderer is poorly documented and lacks toJSON() method.
        // 3. RTF assumes we can find elements by text, role etc. This is false with most 3D elements.
        // Hence we just use render and prettyDOM from @testing-library/react to get a string representation of the tree, and then inspect that.
        // render() generates a bunch of warnings about camelCase elements (which are non-standard in React, but standard in R3F) but otherwise seems to work.
        // Tried wrapping in <Canvas> to see if it suppressed the warnings but it didn't work. Not surpising - it will bypass anything that would
        // render to the DOM - including rendering to the virtual DOM that we are using for testing here.

        render(result);                       
        const tree = prettyDOM();

        console.log("Box geometry render tree:", tree);

        expect(tree).toContain('box');
        expect(tree).toContain('2,3,4');

    });

    it('should render box geometry with custom color', () => {
        const drawable: IDrawable = {
            geometry: { type: 'box', width: 2, height: 3, depth: 4 },
            appearance: { type: 'standard', color: 0x00ff00 }
        };
        const result = r3fFromDrawable(drawable);
        expect(result).toBeDefined();
        render(result);
        const tree = prettyDOM();
        expect(tree).toContain(Number('0x00ff00'));
    });

    it('should render plane geometry with default color', () => {
        const drawable: IDrawable = {
            geometry: { type: 'plane', width: 5, height: 6 }
        };
        const result = r3fFromDrawable(drawable);
        expect(result).toBeDefined();
        render(result);
        const tree = prettyDOM();
        expect(tree).toContain('plane');
        expect(tree).toContain('5,6');
    });

    it('should render sphere geometry with custom color', () => {
        const drawable: IDrawable = {
            geometry: { type: 'sphere', radius: 2.5 },
            appearance: { type: 'standard', color: 0xffffff }
        };
        const result = r3fFromDrawable(drawable);
        expect(result).toBeDefined();
        render(result);
        const tree = prettyDOM();
        expect(tree).toContain('sphere');
        expect(tree).toContain('2.5');
    });

    // oops - Line seems to use a Hook internally, and that triggers an error (something about only working inside Canvas).
    it.skip('should render line geometry with start and end', () => {
        const drawable: IDrawable = {
            geometry: { 
                type: 'line', 
                points: [[0, 0, 0], [1, 1, 1]] 
            },
            appearance: { type: 'standard', color: 0xff0000 }
        };
        const result = r3fFromDrawable(drawable);
        expect(result).toBeDefined();
        render(result);
        const tree = prettyDOM();
        //console.log("Line geometry render tree:", tree);
        expect(tree).toContain('line');
        expect(tree).toContain('0,0,0,1,1,1');
    });

    it('should render point geometry with position data', () => {
        const drawable: IDrawable = {
            geometry: { 
                type: 'point', 
                position: new Float32Array([5, 2, 4]) 
            },
            appearance: { type: 'standard', color: 0x00ff00 }
        };
        const result = r3fFromDrawable(drawable);
        expect(result).toBeDefined();
        render(result);
        const tree = prettyDOM();
        //console.log("Point geometry render tree:", tree);
        expect(tree).toContain('point');
        expect(tree).toContain('5,2,4');
    });
});

