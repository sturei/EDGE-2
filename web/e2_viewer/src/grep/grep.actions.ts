import { Document } from "../document/document";
import { Model } from "../document/model";
import { GRepModel } from "../grep/grepModel";
import { Color, type IDrawable } from "../grep/drawable";
import type { IShaderNode } from "./shaderNode";

function ping(_doc: Document, _payload: any): void {
    // This action just writes "pong" to stderr. Useful for testing that the pieces are connected.
    console.log("pong");
}

function addPoint(doc: Document, payload: any): void {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        let sceneModel = model as GRepModel; 
        const size = payload.size ?? 1.0;
        const position = payload.position ?? [0, 0, 0];
        const color = Color.get(payload.color ?? "red");
        const drawable: IDrawable = {
            geometry: {
                type: 'point',
                position: position
            },
            appearance: {
                type: 'point',
                color: color,
                size: size
            }
        };
        sceneModel.addDrawable(drawable);
    });
    console.log("added Point");      // ---DEBUG---
}

function addLine(doc: Document, payload: any): void {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        const start = payload.start ?? [-1, 0, 0];
        const end = payload.end ?? [1, 0, 0];
        const color = Color.get(payload.color ?? "blue");
        let grepModel = model as GRepModel;
        const drawable: IDrawable = {
            geometry: {
                type: 'line',
                start: start,
                end: end
            },
            appearance: {
                type: 'line',
                color: color
            }
        };
        grepModel.addDrawable(drawable);
    });
    console.log("added Line");      // ---DEBUG---
}

function addPolyline(doc: Document, payload: any): void {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        const positions = payload.positions ?? [[-1, 0, 0],[1,0,0]];
        const color = Color.get(payload.color ?? "blue");
        let grepModel = model as GRepModel;
        const drawable: IDrawable = {
            geometry: {
                type: 'polyline',
                positions: positions
            },
            appearance: {
                type: 'line',
                color: color
            }
        };
        grepModel.addDrawable(drawable);
    });
    console.log("added Polyline");      // ---DEBUG---
}


function addPlane(doc: Document, payload: any): void {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        const width = payload.width ?? 1.0;
        const height = payload.height ?? 1.0;
        const z = payload.z ?? 0.0;
        const color = Color.get(payload.color ?? "blue");
        let texture = undefined;
        if (payload.texture) {
            const texPayload = payload.texture;
            const width = texPayload.width;
            const height = texPayload.height;
            const dataArray = texPayload.data;
            const data = dataArray;
            texture = {
                width: width,
                height: height,
                data: data
            };
        }
        
        let grepModel = model as GRepModel;
        const drawable: IDrawable = {
            geometry: {
                type: 'plane',
                width: width,
                height: height,
                z: z
            },
            appearance: {
                type: 'mesh',
                color: color,
                texture: texture
            }

        };
        grepModel.addDrawable(drawable);    
    });
    console.log("added Plane");      // ---DEBUG---
}


function addSphere(doc: Document, payload: any): void {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        const radius = payload.radius ?? 1.0;
        const color = Color.get(payload.color ?? "blue");

        let grepModel = model as GRepModel;
        const drawable: IDrawable = {
            geometry: {
                type: 'sphere',
                radius: radius
            },
            appearance: {
                type: 'mesh',
                color: color
            }
        };
        grepModel.addDrawable(drawable);
    });
    console.log("added Sphere");      // ---DEBUG---
}


function addBlock(doc: Document, payload: any): void {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        const width = payload.width ?? 2.0;      // length in x direction
        const height = payload.height ?? 2.0;    // length in y direction
        const depth = payload.depth ?? 2.0;      // length in z direction
        const color = Color.get(payload.color ?? "green");
        let grepModel = model as GRepModel;
        const drawable: IDrawable = {
            geometry: {
                type: 'block',
                width: width,
                height: height,
                depth: depth
            },
            appearance: {
                type: 'mesh',
                color: color
            }
        };
        grepModel.addDrawable(drawable);
        console.log("added Block");      // ---DEBUG---
    });
}


function addProfile(doc: Document, payload: any): void {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        const paths = payload.paths ?? [[[-1, -1], [1, -1], [1, 1], [-1, 1], [-1, -1]]];
        const color = Color.get(payload.color ?? "green");
        let grepModel = model as GRepModel;
        const drawable: IDrawable = {
            geometry: {
                type: 'profile',
                paths: paths
            },
            appearance: {
                type: 'mesh',
                color: color
            }
        };
        grepModel.addDrawable(drawable);
    });
    console.log("added Profile");      // ---DEBUG---
}

function addSdfNode(doc: Document, payload: any): void {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        let grepModel = model as GRepModel;
        const type = payload.type;
        if (!type) {
            console.error("addSdfNode action requires 'type' in payload");
            return;
        }
        else if (type === 'sphere') {
            const radius = payload.radius ?? 1.0;
            const node: IShaderNode = {
                type: 'sphere',
                pathName: payload.pathName ?? 'sphere',
                parameters: new Map([['radius', radius]]),
                childIndices: undefined
            };
            grepModel.addShaderNode(node);
        }
        else if (type === 'block') {
            const width = payload.width ?? 1.0;
            const height = payload.height ?? 1.0;
            const depth = payload.depth ?? 1.0;
            const node: IShaderNode = {
                type: 'block',
                pathName: payload.pathName ?? 'block',
                parameters: new Map([['width', width], ['height', height], ['depth', depth]]),
                childIndices: undefined
            };
            grepModel.addShaderNode(node);
        }
        else if (type === 'cylinder') {
            const radius = payload.radius ?? 1.0;
            const depth = payload.depth ?? 1.0;
            const node: IShaderNode = {
                type: 'cylinder',
                pathName: payload.pathName ?? 'cylinder',
                parameters: new Map([['radius', radius], ['depth', depth]]),
                childIndices: undefined
            };
            grepModel.addShaderNode(node);
        }
        else if (type === 'union') {
            const node : IShaderNode = {
                type: 'union',
                pathName: payload.pathName ?? 'union',
                parameters: new Map(),
                childIndices: undefined
            };
            grepModel.addShaderNode(node);
        }
        else {
            console.error(`addSdfNode action: unknown type '${type}'`);
        }
    });
    console.log("added SDF Node");      // ---DEBUG---
}

function updateSdfScene(doc: Document, _payload: any): void {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        let grepModel = model as GRepModel;
        // Update the SDF scene GUID to indicate that the SDF scene has changed.
        const newGuid = crypto.randomUUID();
        grepModel.setSdfGuid(newGuid);
        console.log("Updated SDF scene GUID to:", newGuid);
    }
    );
}


export const pingActionDef = { type: "Gfx::ping", function: ping };
export const addPointActionDef = { type: "Gfx::addPoint", function: addPoint };
export const addLineActionDef = { type: "Gfx::addLine", function: addLine };
export const addPolylineActionDef = { type: "Gfx::addPolyline", function: addPolyline };
export const addPlaneActionDef = { type: "Gfx::addPlane", function: addPlane };
export const addSphereActionDef = { type: "Gfx::addSphere", function: addSphere };
export const addBlockActionDef = { type: "Gfx::addBlock", function: addBlock };
export const addProfileActionDef = { type: "Gfx::addProfile", function: addProfile };
export const addSdfNodeActionDef = { type: "Gfx::addSdfNode", function: addSdfNode };
export const updateSdfSceneActionDef = { type: "Gfx::updateSdfScene", function: updateSdfScene };
