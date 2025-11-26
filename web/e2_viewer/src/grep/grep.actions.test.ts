import { describe, it, expect, beforeEach, vi } from 'vitest';
import {
    pingActionDef,
    addPointActionDef,
    addLineActionDef,
    addPolylineActionDef,
    addPlaneActionDef,
    addSphereActionDef,
    addBlockActionDef,
    addProfileActionDef
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

    describe("addPoint action", () => {
        it("should add Point with defaults", () => {
            addPointActionDef.function(mockDoc, {});
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });

        it("should add Point with custom size", () => {
            addPointActionDef.function(mockDoc, { size: 2.5 });
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });
    });

    describe("addLine action", () => {
        it("should add Line with defaults", () => {
            addLineActionDef.function(mockDoc, {});
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });

        it("should add Line with custom start and end", () => {
            addLineActionDef.function(mockDoc, { start: [0,0,0], end: [1,1,1] });
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });
    });

    describe("addPolyline action", () => {
        it("should add Polyline with defaults", () => {
            addPolylineActionDef.function(mockDoc, {});
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });

        it("should add Polyline with custom positions", () => {
            addPolylineActionDef.function(mockDoc, { positions: [[0,0,0],[1,1,1],[2,2,2]] });
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });
    });

    describe("addPlane action", () => {
        it("should add Plane with default dimensions", () => {
            addPlaneActionDef.function(mockDoc, {});
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });

        it("should add Plane with custom dimensions", () => {
            addPlaneActionDef.function(mockDoc, { width: 2.0, height: 3.0 });
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });
    });

    describe("addSphere action", () => {
        it("should add Sphere with default radius", () => {
            addSphereActionDef.function(mockDoc, {});
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });

        it("should add Sphere with custom radius", () => {
            addSphereActionDef.function(mockDoc, { radius: 4.0 });
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });
    });

    describe("addBlock action", () => {
        it("should add Block with default dimensions", () => {
            addBlockActionDef.function(mockDoc, {});
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });

        it("should add Block with custom dimensions", () => {
            const payload = { width: 2.0, height: 3.0, depth: 4.0 };
            addBlockActionDef.function(mockDoc, payload);
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });
    });

    describe("addProfile action", () => {
        it("should add Profile with provided paths", () => {
            const paths = [[[0,0],[1,0],[1,1],[0,1],[0,0]]];
            addProfileActionDef.function(mockDoc, { paths });
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });
    });    

    describe("action definitions", () => {
        it("should have correct type for ping action", () => {
            expect(pingActionDef.type).toBe("Gfx::ping");
        });

        it("should have correct type for addPoint action", () => {
            expect(addPointActionDef.type).toBe("Gfx::addPoint");
        });

        it("should have correct type for addLine action", () => {
            expect(addLineActionDef.type).toBe("Gfx::addLine");
        });

        it("should have correct type for addPolyline action", () => {
            expect(addPolylineActionDef.type).toBe("Gfx::addPolyline");
        });

        it("should have correct type for addPlane action", () => {
            expect(addPlaneActionDef.type).toBe("Gfx::addPlane");
        });

        it("should have correct type for addSphere action", () => {
            expect(addSphereActionDef.type).toBe("Gfx::addSphere");
        });

        it("should have correct type for addBlock action", () => {
            expect(addBlockActionDef.type).toBe("Gfx::addBlock");
        });

        it("should have correct type for addProfile action", () => {
            expect(addProfileActionDef.type).toBe("Gfx::addProfile");
        });
    });
});