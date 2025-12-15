import { describe, it, expect, vi } from 'vitest';
import { render, screen } from '@testing-library/react';
import { Drawlist } from './Drawlist';
import type { IDrawable } from '../grep/drawables/drawable';
import "@testing-library/jest-dom";

/**
 * @vitest-environment jsdom
 */

interface IMockDrawable extends IDrawable {
    id?: string;
}

// Mock the Drawable component
vi.mock('./Drawable', () => ({
    Drawable: ({ drawable }: { drawable: IMockDrawable }) => (
        <div data-testid="drawable-item">{drawable.id || 'drawable'}</div>
    )
}));

describe('Drawlist', () => {
    it('renders empty list when drawlist is empty', () => {
        render(<Drawlist drawlist={[]} />);
        expect(screen.queryByTestId('drawable-item')).not.toBeInTheDocument();
    });

    it('renders single drawable item', () => {
        const mockDrawable: IMockDrawable = { id: 'test-1' };
        render(<Drawlist drawlist={[mockDrawable]} />);
        
        expect(screen.getByTestId('drawable-item')).toBeInTheDocument();
        expect(screen.getByText('test-1')).toBeInTheDocument();
    });

    it('renders multiple drawable items', () => {
        const mockDrawables: IMockDrawable[] = [
            { id: 'test-1' },
            { id: 'test-2' },
            { id: 'test-3' }
        ];
        render(<Drawlist drawlist={mockDrawables} />);
        
        const drawableItems = screen.getAllByTestId('drawable-item');
        expect(drawableItems).toHaveLength(3);
        expect(screen.getByText('test-1')).toBeInTheDocument();
        expect(screen.getByText('test-2')).toBeInTheDocument();
        expect(screen.getByText('test-3')).toBeInTheDocument();
    });

    it('passes correct drawable prop to each Drawable component', () => {
        const mockDrawables: IMockDrawable[] = [
            { id: 'item-1' },
            { id: 'item-2' }
        ];
        render(<Drawlist drawlist={mockDrawables} />);
        
        expect(screen.getByText('item-1')).toBeInTheDocument();
        expect(screen.getByText('item-2')).toBeInTheDocument();
    });
});