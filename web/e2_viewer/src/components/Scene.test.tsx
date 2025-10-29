import { describe, it, expect, vi, beforeEach } from 'vitest';
import { render, screen } from '@testing-library/react';
import '@testing-library/jest-dom';
import { Scene, drawableFromGItem } from './Scene';
import { DocumentContext } from '../Contexts';
import { Document } from '../document/document';
import { Store } from '../document/store';
import { GBlock, GSphere, GLine, GPoint, GPlane } from '../grep/gitem';

/**
 * @vitest-environment jsdom
 */


// Mock react-three-fiber and drei
vi.mock('@react-three/fiber', () => ({
    Canvas: ({ children }: { children: React.ReactNode }) => <div data-testid="canvas">{children}</div>
}));

vi.mock('@react-three/drei', () => ({
    OrbitControls: () => <div data-testid="orbit-controls" />
}));

// Mock the Drawlist component
vi.mock('./Drawlist.tsx', () => ({
    Drawlist: ({ drawlist }: { drawlist: any[] }) => (
        <div data-testid="drawlist" data-count={drawlist.length} />
    )
}));

describe('Scene', () => {
    let mockDocument: Document;

    beforeEach(() => {
        mockDocument = new Document();
        vi.clearAllMocks();
    });

    it('renders scene with lights and orbit controls', () => {
        render(
            <DocumentContext.Provider value={mockDocument}>
                <Scene />
            </DocumentContext.Provider>
        );

        expect(screen.getByTestId('drawlist')).toBeInTheDocument();
        expect(screen.getByTestId('orbit-controls')).toBeInTheDocument();
    });

    it('creates scene store and model on mount', () => {
        const addStoreSpy = vi.spyOn(mockDocument, 'addStore');
        
        render(
            <DocumentContext.Provider value={mockDocument}>
                <Scene />
            </DocumentContext.Provider>
        );

        expect(addStoreSpy).toHaveBeenCalledWith('scene', expect.any(Store));
    });

    it('starts with empty drawlist', () => {
    render(
        <DocumentContext.Provider value={mockDocument}>
            <Scene />
        </DocumentContext.Provider>
    );

    expect(screen.getByTestId('drawlist')).toHaveAttribute('data-count', '0');
    });
});

describe('drawableFromGItem', () => {  
    it('converts GBlock to drawable with correct geometry', () => {
        const block = new GBlock(10, 20, 30);
        const drawable = drawableFromGItem(block);

        expect(drawable.geometry).toEqual({
            type: 'box',
            width: 10,
            height: 20,
            depth: 30
        });
    });

    it('converts GSphere to drawable with correct geometry', () => {
        const sphere = new GSphere(5);
        const drawable = drawableFromGItem(sphere);

        expect(drawable.geometry).toEqual({
            type: 'sphere',
            radius: 5
        });
    });

    it('converts GLine to drawable with correct geometry', () => {
        const line = new GLine(100);
        const drawable = drawableFromGItem(line);

        expect(drawable.geometry).toEqual({
            type: 'line',
            points: [[-50, 0, 0], [50, 0, 0]]
        });
    });

    it('converts GPoint to drawable with correct geometry', () => {
        const point = new GPoint(3.0);
        const drawable = drawableFromGItem(point);

        expect(drawable.geometry).toEqual({
            type: 'point',
            position: new Float32Array([0, 0, 0])
        });
    });

    it('converts GPlane to drawable with correct geometry', () => {
        const plane = new GPlane(50, 75);
        const drawable = drawableFromGItem(plane);

        expect(drawable.geometry).toEqual({
            type: 'plane',
            width: 50,
            height: 75
        });
    });
});
