import { describe, it, expect, vi, beforeEach, type Mock } from 'vitest';
import { render, screen, fireEvent } from '@testing-library/react';
import { Actions } from './Actions';
import { DocumentContext } from '../Contexts';
import * as grepActions from '../grep/grep.actions.ts';
import * as brepActions from '../brep/brep.actions.ts';

/**
 * @vitest-environment jsdom
 */

// Mock the grep actions module
vi.mock('../grep/grep.actions.ts', () => ({
    pingActionDef: { type: 'ping' },
    addGPointActionDef: { type: 'addGPoint' },
    addGLineActionDef: { type: 'addGLine' },
    addGPlaneActionDef: { type: 'addGPlane' },
    addGSphereActionDef: { type: 'addGSphere' },
    addGBlockActionDef: { type: 'addGBlock' },
    addGShapeActionDef: { type: 'addGShape' },
    addPingModellerActionDef: { type: 'pingModeller' }
}));

// Mock the brep actions module
vi.mock('../brep/brep.actions.ts', () => ({
    pingModellerActionDef: { type: 'pingModeller' },
    addSheetRectangleActionDef: { type: 'addSheetRectangle' }
}));

describe('Actions', () => {
    let mockDocument: {
        registerActionFunction: Mock;
        dispatchAction: Mock;
    };

    beforeEach(() => {
        mockDocument = {
            registerActionFunction: vi.fn(),
            dispatchAction: vi.fn()
        };
        vi.clearAllMocks();
    });

    const renderActions = () => {
        return render(
            <DocumentContext.Provider value={mockDocument as any}>
                <Actions />
            </DocumentContext.Provider>
        );
    };

    it('renders the action input form', () => {
        renderActions();
        
        expect(screen.getByPlaceholderText('Next action?')).toBeInTheDocument();
        expect(screen.getByRole('combobox')).toBeInTheDocument();
    });

    it('registers all action functions on mount', () => {
        renderActions();
        
        expect(mockDocument.registerActionFunction).toHaveBeenCalledTimes(9);
        expect(mockDocument.registerActionFunction).toHaveBeenCalledWith(grepActions.pingActionDef);
        expect(mockDocument.registerActionFunction).toHaveBeenCalledWith(grepActions.addGPointActionDef);
        expect(mockDocument.registerActionFunction).toHaveBeenCalledWith(grepActions.addGLineActionDef);
        expect(mockDocument.registerActionFunction).toHaveBeenCalledWith(grepActions.addGPlaneActionDef);
        expect(mockDocument.registerActionFunction).toHaveBeenCalledWith(grepActions.addGSphereActionDef);
        expect(mockDocument.registerActionFunction).toHaveBeenCalledWith(grepActions.addGBlockActionDef);
        expect(mockDocument.registerActionFunction).toHaveBeenCalledWith(grepActions.addGShapeActionDef);
        expect(mockDocument.registerActionFunction).toHaveBeenCalledWith(brepActions.pingModellerActionDef);
        expect(mockDocument.registerActionFunction).toHaveBeenCalledWith(brepActions.addSheetRectangleActionDef);

    });

    it('dispatches valid JSON action when form is submitted', () => {
        renderActions();
        const input = screen.getByPlaceholderText('Next action?');
        const form = input.closest('form')!;
        
        fireEvent.change(input, { target: { value: '{"type": "ping", "payload": {}}' } });
        fireEvent.submit(form);
        
        expect(mockDocument.dispatchAction).toHaveBeenCalledWith({ type: 'ping', payload: {} });
    });

    it('does not dispatch action when input is empty', () => {
        const consoleSpy = vi.spyOn(console, 'log').mockImplementation(() => {});
        renderActions();
        const input = screen.getByPlaceholderText('Next action?');
        const form = input.closest('form')!;
        
        fireEvent.change(input, { target: { value: '' } });
        fireEvent.submit(form);
        
        expect(mockDocument.dispatchAction).not.toHaveBeenCalled();
        expect(consoleSpy).toHaveBeenCalledWith('Blank line - no action dispatched.');
        
        consoleSpy.mockRestore();
    });

    it('does not dispatch action when input is only whitespace', () => {
        const consoleSpy = vi.spyOn(console, 'log').mockImplementation(() => {});
        renderActions();
        const input = screen.getByPlaceholderText('Next action?');
        const form = input.closest('form')!;
        
        fireEvent.change(input, { target: { value: '   ' } });
        fireEvent.submit(form);
        
        expect(mockDocument.dispatchAction).not.toHaveBeenCalled();
        expect(consoleSpy).toHaveBeenCalledWith('Blank line - no action dispatched.');
        
        consoleSpy.mockRestore();
    });

    it('handles invalid JSON gracefully', () => {
        const consoleErrorSpy = vi.spyOn(console, 'error').mockImplementation(() => {});
        renderActions();
        const input = screen.getByPlaceholderText('Next action?');
        const form = input.closest('form')!;
        
        fireEvent.change(input, { target: { value: 'invalid json' } });
        fireEvent.submit(form);
        
        expect(mockDocument.dispatchAction).not.toHaveBeenCalled();
        expect(consoleErrorSpy).toHaveBeenCalledWith(
            expect.stringContaining('Error dispatching action: invalid json')
        );
        
        consoleErrorSpy.mockRestore();
    });

});