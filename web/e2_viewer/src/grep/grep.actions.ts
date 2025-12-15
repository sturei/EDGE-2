import { Document } from "../document/document";
import { Model } from "../document/model";
import { GRepModel } from "../grep/grepModel";
import { Color, type IDrawable } from "./drawables/drawable";
import type { ISdfNode } from "./nodes/sdfNode";

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
        
function addCylinder(doc: Document, payload: any): void {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        const radius = payload.radius ?? 1.0;      // radius of the cylinder
        const depth = payload.depth ?? 2.0;      // length in z direction
        const color = Color.get(payload.color ?? "yellow");
        let grepModel = model as GRepModel;
        const drawable: IDrawable = {
            geometry: {
                type: 'cylinder',
                radius: radius,
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

function addContour(doc: Document, payload: any): void {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        const paths = payload.paths ?? [[[-1, -1], [1, -1], [1, 1], [-1, 1], [-1, -1]]];
        const color = Color.get(payload.color ?? "green");
        let grepModel = model as GRepModel;
        const drawable: IDrawable = {
            geometry: {
                type: 'contour',
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

        if (type === 'sphere') {
            const node: ISdfNode = {
                type: 'sphere',
                pathName: payload.pathName ?? 'sphere',
                radius: payload.radius ?? 1.0,
            };
            grepModel.addShaderNode(node);
        }
        else if (type === 'block') {
            const node: ISdfNode = {
                type: 'block',
                pathName: payload.pathName ?? 'block',
                width: payload.width ?? 1.0,
                height: payload.height ?? 1.0,
                depth: payload.depth ?? 1.0
            };
            grepModel.addShaderNode(node);
        }
        else if (type === 'cylinder') {
            const node: ISdfNode = {
                type: 'cylinder',
                pathName: payload.pathName ?? 'cylinder',
                radius: payload.radius ?? 1.0,
                depth: payload.depth ?? 1.0
            };
            grepModel.addShaderNode(node);
        }
        else if (type === 'circle') {
            const node: ISdfNode = {
                type: 'circle',
                pathName: payload.pathName ?? 'circle',
                radius: payload.radius ?? 1.0
            };
            grepModel.addShaderNode(node);
        }
        else if (type === 'rectangle') {
            const node: ISdfNode = {
                type: 'rectangle',
                pathName: payload.pathName ?? 'rectangle',
                width:  payload.width ?? 1.0,
                height: payload.height ?? 1.0
            };
            grepModel.addShaderNode(node);
        }
        else if (type === 'roundRect') {
            const node: ISdfNode = {
                type: 'roundRect',
                pathName: payload.pathName ?? 'roundRect',
                width:  payload.width ?? 1.0,
                height: payload.height ?? 1.0,
                cornerRadius: payload.cornerRadius ?? 0.1
            };
            grepModel.addShaderNode(node);
        }
        else if (type === 'profile') {
            const node: ISdfNode = {
                type: 'profile',
                pathName: payload.pathName ?? 'profile',
                paths: payload.paths ?? [[[-1, -1], [1, -1], [1, 1], [-1, 1], [-1, -1]]],
            };
            grepModel.addShaderNode(node);
        }
        else if (type === 'extrusion') {
            const node : ISdfNode = {
                type: 'extrusion',
                pathName: payload.pathName ?? 'extrusion',
                depth: payload.depth ?? 2.0
            };
            grepModel.addShaderNode(node);
        }
        else if (type === 'halfSpace') {
            const node : ISdfNode = {
                type: 'halfSpace',
                pathName: payload.pathName ?? 'halfSpace',
                position: payload.position ?? [0, 0, 0],
                normal: payload.normal ?? [0, 0, 1]
            };
            grepModel.addShaderNode(node);
        }
        else if (type === 'union') {
            const node : ISdfNode = {
                type: 'union',
                pathName: payload.pathName ?? 'union'
            };
            grepModel.addShaderNode(node);
        }
        else if (type === 'intersection') {
            const node : ISdfNode = {
                type: 'intersection',
                pathName: payload.pathName ?? 'intersection'
            };
            grepModel.addShaderNode(node);
        }
        else if (type === 'complement') {
            const node : ISdfNode = {
                type: 'complement',
                pathName: payload.pathName ?? 'complement'
            };
            grepModel.addShaderNode(node);
        }
        else if (type === 'translation') {
            // TODO: combine into a single transform node that can do translation and rotation (rotation first)
            const node : ISdfNode = {
                type: 'translation',
                pathName: payload.pathName ?? 'translation',
                translation: payload.translation ?? [0, 0, 0]   
            };
            grepModel.addShaderNode(node);
        }
        else if (type === 'rotation') {
            const node : ISdfNode = {
                type: 'rotation',
                pathName: payload.pathName ?? 'rotation',
                rotation: payload.rotation ?? [0, 0, 0]
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

function clearSdfScene(doc: Document, _payload: any): void {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        let grepModel = model as GRepModel;
        grepModel.clearShaderNodes();
        console.log("Cleared SDF scene nodes.");
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
export const addCylinderActionDef = { type: "Gfx::addCylinder", function: addCylinder };
export const addProfileActionDef = { type: "Gfx::addProfile", function: addProfile };
export const addContourActionDef = { type: "Gfx::addContour", function: addContour };
export const addSdfNodeActionDef = { type: "Gfx::addSdfNode", function: addSdfNode };
export const updateSdfSceneActionDef = { type: "Gfx::updateSdfScene", function: updateSdfScene };
export const clearSdfSceneActionDef = { type: "Gfx::clearSdfScene", function: clearSdfScene };      