import { describe, it, expect, beforeEach, vi } from 'vitest';
import { GPoint, GLine, GPlane, GSphere, GBlock } from "./gitem";
import {
    pingActionDef,
    addGPointActionDef,
    addGLineActionDef,
    addGPlaneActionDef,
    addGSphereActionDef,
    addGBlockActionDef
} from "./grep.actions";

// Mock dependencies
vi.mock("../document/document");
vi.mock("../grep/grepModel");
vi.mock("./gitem");

describe("grep.actions", () => {
    let mockDoc: any;
    let mockStore: any;
    let mockGRepModel: any;

    beforeEach(() => {
        mockGRepModel = {
            addGItem: vi.fn()
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

    describe("addGPoint action", () => {
        it("should add GPoint with default size", () => {
            addGPointActionDef.function(mockDoc, {});
            expect(GPoint).toHaveBeenCalledWith(1.0);
            expect(mockGRepModel.addGItem).toHaveBeenCalled();
        });

        it("should add GPoint with custom size", () => {
            addGPointActionDef.function(mockDoc, { size: 2.5 });
            expect(GPoint).toHaveBeenCalledWith(2.5);
            expect(mockGRepModel.addGItem).toHaveBeenCalled();
        });
    });

    describe("addGLine action", () => {
        it("should add GLine with default length", () => {
            addGLineActionDef.function(mockDoc, {});
            expect(GLine).toHaveBeenCalledWith(1.0);
            expect(mockGRepModel.addGItem).toHaveBeenCalled();
        });

        it("should add GLine with custom length", () => {
            addGLineActionDef.function(mockDoc, { length: 3.0 });
            expect(GLine).toHaveBeenCalledWith(3.0);
            expect(mockGRepModel.addGItem).toHaveBeenCalled();
        });
    });

    describe("addGPlane action", () => {
        it("should add GPlane with default dimensions", () => {
            addGPlaneActionDef.function(mockDoc, {});
            expect(GPlane).toHaveBeenCalledWith(1.0, 1.0);
            expect(mockGRepModel.addGItem).toHaveBeenCalled();
        });

        it("should add GPlane with custom dimensions", () => {
            addGPlaneActionDef.function(mockDoc, { width: 2.0, height: 3.0 });
            expect(GPlane).toHaveBeenCalledWith(2.0, 3.0);
            expect(mockGRepModel.addGItem).toHaveBeenCalled();
        });
    });

    describe("addGSphere action", () => {
        it("should add GSphere with default radius", () => {
            addGSphereActionDef.function(mockDoc, {});
            expect(GSphere).toHaveBeenCalledWith(1.0);
            expect(mockGRepModel.addGItem).toHaveBeenCalled();
        });

        it("should add GSphere with custom radius", () => {
            addGSphereActionDef.function(mockDoc, { radius: 4.0 });
            expect(GSphere).toHaveBeenCalledWith(4.0);
            expect(mockGRepModel.addGItem).toHaveBeenCalled();
        });
    });

    describe("addGBlock action", () => {
        it("should add GBlock with default dimensions", () => {
            addGBlockActionDef.function(mockDoc, {});
            expect(GBlock).toHaveBeenCalledWith(2.0, 2.0, 2.0);
            expect(mockGRepModel.addGItem).toHaveBeenCalled();
        });

        it("should add GBlock with custom dimensions", () => {
            const payload = { width: 2.0, height: 3.0, depth: 4.0 };
            addGBlockActionDef.function(mockDoc, payload);
            expect(GBlock).toHaveBeenCalledWith(2.0, 3.0, 4.0);
            expect(mockGRepModel.addGItem).toHaveBeenCalled();
        });
    });

    describe("action definitions", () => {
        it("should have correct type for ping action", () => {
            expect(pingActionDef.type).toBe("ping");
        });

        it("should have correct type for addGPoint action", () => {
            expect(addGPointActionDef.type).toBe("addGPoint");
        });

        it("should have correct type for addGLine action", () => {
            expect(addGLineActionDef.type).toBe("addGLine");
        });

        it("should have correct type for addGPlane action", () => {
            expect(addGPlaneActionDef.type).toBe("addGPlane");
        });

        it("should have correct type for addGSphere action", () => {
            expect(addGSphereActionDef.type).toBe("addGSphere");
        });

        it("should have correct type for addGBlock action", () => {
            expect(addGBlockActionDef.type).toBe("addGBlock");
        });
    });
});