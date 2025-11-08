import { describe, it, expect, vi, beforeAll, beforeEach, afterEach } from 'vitest';
import { ChildProcess, spawn } from 'child_process';
import { dispatchAction, modellingService } from './actions.js';

// Mock child_process

vi.mock('child_process', () => {
    let mockChildProcess: Partial<ChildProcess>;
    let mockStdout: any;
    let mockStdin: any;
    let mockStderr: any;
    mockStdout = {
        on: vi.fn()
    };
    mockStdin = {
        write: vi.fn()
    };
    mockStderr = {
        on: vi.fn(),
        setEncoding: vi.fn()
    };
    mockChildProcess = {
        stdout: mockStdout,
        stdin: mockStdin,
        stderr: mockStderr,
        on: vi.fn(),
        pid: 12345
    };

    return {
        spawn: vi.fn(() => mockChildProcess)
    }
});


describe('actions', () => {
    describe('modellingService', () => {
        it('should be spawned with correct path', () => {
            expect(spawn).toHaveBeenCalledWith('../../engines/build/e2_modellingService');
        });

        it('should export the spawned process', () => {
            expect(modellingService.pid).toEqual(12345);
        });
    });

    // ideally there should be a test for dispatchAction. 
    // Probably that would need a mock of stdin.on that saves the callback that's registered in the real code, and a mock of
    // stdin.write that calls it with the mocked data.

});

