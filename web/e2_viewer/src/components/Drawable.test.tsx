import { describe, it, expect } from 'vitest';
import { render, screen } from '@testing-library/react';
import { Canvas } from '@react-three/fiber';
import { Drawable, type IDrawable } from './Drawable';
import "@testing-library/jest-dom";
import ResizeObserver from 'resize-observer-polyfill'
window.ResizeObserver = ResizeObserver

/**
 * @vitest-environment jsdom
 */

/** This is pointless until I can get ti actually render the Drawable. Right now it just does nothing below the Canvas.
 *  To be valid tests, one would need to dig into the rendered output, looking for e.g. 'Sphere/ So skipping these tests for now.
 */

// Helper to render component within R3F Canvas
const renderWithCanvas = (drawable: IDrawable) => {
    return render(
        <Canvas>
            <Drawable drawable={drawable} />
        </Canvas>
    );
};

describe.skip('Drawable', () => {
    it('should render box geometry with default color', () => {
        const drawable: IDrawable = {
            geometry: {
                type: 'box',
                width: 1,
                height: 2,
                depth: 3
            }
        };
        
        const { container } = renderWithCanvas(drawable);
        expect(container).toBeInTheDocument();
    });

    it('should render box geometry with custom appearance', () => {
        const drawable: IDrawable = {
            geometry: {
                type: 'box',
                width: 1,
                height: 2,
                depth: 3
            },
            appearance: {
                type: 'standard',
                color: 0x00ff00
            }
        };
        
        const { container } = renderWithCanvas(drawable);
        expect(container).toBeInTheDocument();
    });

    it('should render plane geometry', () => {
        const drawable: IDrawable = {
            geometry: {
                type: 'plane',
                width: 5,
                height: 3
            }
        };
        
        const { container } = renderWithCanvas(drawable);
        expect(container).toBeInTheDocument();
    });

    it('should render sphere geometry', () => {
        const drawable: IDrawable = {
            geometry: {
                type: 'sphere',
                radius: 2.5
            }
        };
        
        const { container } = renderWithCanvas(drawable);
        expect(container).toBeInTheDocument();

        console.log("Created SPHERE?");
        console.log(screen.debug());

    });

    it('should render line geometry', () => {
        const drawable: IDrawable = {
            geometry: {
                type: 'line',
                points: [[0, 0, 0], [1, 1, 1], [2, 0, 2]]
            }
        };
        
        const { container } = renderWithCanvas(drawable);
        expect(container).toBeInTheDocument();
    });

    it('should render point geometry', () => {
        const drawable: IDrawable = {
            geometry: {
                type: 'point',
                position: new Float32Array([0, 0, 0, 1, 1, 1])
            }
        };
        
        const { container } = renderWithCanvas(drawable);
        expect(container).toBeInTheDocument();
    });

    it('should handle drawable without geometry', () => {
        const drawable: IDrawable = {};
        
        const { container } = renderWithCanvas(drawable);
        expect(container).toBeInTheDocument();
    });

    it('should handle drawable with only appearance', () => {
        const drawable: IDrawable = {
            appearance: {
                type: 'standard',
                color: 0xff0000
            }
        };
        
        const { container } = renderWithCanvas(drawable);
        expect(container).toBeInTheDocument();
    });
});
