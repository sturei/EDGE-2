import { describe, it, expect, beforeEach, vi } from 'vitest';
import {
    pingActionDef,
    addGPointActionDef,
    addGLineActionDef,
    addGPolylineActionDef,
    addGPlaneActionDef,
    addGSphereActionDef,
    addGBlockActionDef,
    addGProfileActionDef
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

    describe("addGPoint action", () => {
        it("should add GPoint with defaults", () => {
            addGPointActionDef.function(mockDoc, {});
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });

        it("should add GPoint with custom size", () => {
            addGPointActionDef.function(mockDoc, { size: 2.5 });
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });
    });

    describe("addGLine action", () => {
        it("should add GLine with defaults", () => {
            addGLineActionDef.function(mockDoc, {});
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });

        it("should add GLine with custom start and end", () => {
            addGLineActionDef.function(mockDoc, { start: [0,0,0], end: [1,1,1] });
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });
    });

    describe("addGPolyline action", () => {
        it("should add GPolyline with defaults", () => {
            addGPolylineActionDef.function(mockDoc, {});
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });

        it("should add GPolyline with custom positions", () => {
            addGPolylineActionDef.function(mockDoc, { positions: [[0,0,0],[1,1,1],[2,2,2]] });
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });
    });

    describe("addGPlane action", () => {
        it("should add GPlane with default dimensions", () => {
            addGPlaneActionDef.function(mockDoc, {});
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });

        it("should add GPlane with custom dimensions", () => {
            addGPlaneActionDef.function(mockDoc, { width: 2.0, height: 3.0 });
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });
    });

    describe("addGSphere action", () => {
        it("should add GSphere with default radius", () => {
            addGSphereActionDef.function(mockDoc, {});
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });

        it("should add GSphere with custom radius", () => {
            addGSphereActionDef.function(mockDoc, { radius: 4.0 });
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });
    });

    describe("addGBlock action", () => {
        it("should add GBlock with default dimensions", () => {
            addGBlockActionDef.function(mockDoc, {});
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });

        it("should add GBlock with custom dimensions", () => {
            const payload = { width: 2.0, height: 3.0, depth: 4.0 };
            addGBlockActionDef.function(mockDoc, payload);
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });
    });

    describe("addGProfile action", () => {
        it("should add GProfile with provided paths", () => {
            const paths = [[[0,0],[1,0],[1,1],[0,1],[0,0]]];
            addGProfileActionDef.function(mockDoc, { paths });
            expect(mockGRepModel.addDrawable).toHaveBeenCalled();
        });
    });    

    describe("action definitions", () => {
        it("should have correct type for ping action", () => {
            expect(pingActionDef.type).toBe("Gfx::ping");
        });

        it("should have correct type for addGPoint action", () => {
            expect(addGPointActionDef.type).toBe("Gfx::addGPoint");
        });

        it("should have correct type for addGLine action", () => {
            expect(addGLineActionDef.type).toBe("Gfx::addGLine");
        });

        it("should have correct type for addGPolyline action", () => {
            expect(addGPolylineActionDef.type).toBe("Gfx::addGPolyline");
        });

        it("should have correct type for addGPlane action", () => {
            expect(addGPlaneActionDef.type).toBe("Gfx::addGPlane");
        });

        it("should have correct type for addGSphere action", () => {
            expect(addGSphereActionDef.type).toBe("Gfx::addGSphere");
        });

        it("should have correct type for addGBlock action", () => {
            expect(addGBlockActionDef.type).toBe("Gfx::addGBlock");
        });

        it("should have correct type for addGProfile action", () => {
            expect(addGProfileActionDef.type).toBe("Gfx::addGProfile");
        });
    });
});