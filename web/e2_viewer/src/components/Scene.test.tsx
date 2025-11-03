import { describe, it, expect, vi, beforeEach } from 'vitest';
import { render, screen } from '@testing-library/react';
import '@testing-library/jest-dom';
import { DocumentContext } from '../Contexts';
import { Document } from '../document/document';
import { Store } from '../document/store';
import { Scene } from './Scene';

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

