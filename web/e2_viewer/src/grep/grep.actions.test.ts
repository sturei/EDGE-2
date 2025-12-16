import { describe, it, expect, beforeEach, vi } from 'vitest';
import {
    pingActionDef
} from "./grep.actions";

// Mock dependencies
vi.mock("../document/document");
vi.mock("../grep/grepModel");

describe("grep.actions", () => {
    let mockDoc: any;
    let mockStore: any;
    let mockGRepModel: any;

    beforeEach(() => {
        mockGRepModel = {
            addDrawable: vi.fn()
        };

        mockStore = {
            changeState: vi.fn((callback) => callback(mockGRepModel))
        };

        mockDoc = {
            getStore: vi.fn().mockReturnValue(mockStore)
        };

        vi.clearAllMocks();
        console.log = vi.fn();
    });

    describe("ping action", () => {
        it("should log 'pong' to console", () => {
            pingActionDef.function(mockDoc, {});
            expect(console.log).toHaveBeenCalledWith("pong");
        });
    });


    describe("action definitions", () => {
        it("should have correct type for ping action", () => {
            expect(pingActionDef.type).toBe("Gfx::ping");
        });

    });
});