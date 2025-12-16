import { Document } from "../../document/document";
import { Model } from "../../document/model";
import { GRepModel } from "../../grep/grepModel";
import { Color, type IDrawable } from "../drawables/drawable";

function addPoint(doc: Document, payload: any): void {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        const position = payload.position ?? [0, 0, 0];
        let sceneModel = model as GRepModel; 
        const size = payload.size ?? 1.0;
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
        const position = payload.position ?? [0, 0, 0];
        const rotation = payload.rotation ?? [0, 0, 0];
        const start = payload.start ?? [-1, 0, 0];
        const end = payload.end ?? [1, 0, 0];
        const color = Color.get(payload.color ?? "blue");
        let grepModel = model as GRepModel;
        const drawable: IDrawable = {
            geometry: {
                type: 'line',
                start: start,
                end: end,
                position: position,
                rotation: rotation
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
        const position = payload.position ?? [0, 0, 0];
        const rotation = payload.rotation ?? [0, 0, 0];
        const positions = payload.positions ?? [[-1, 0, 0],[1,0,0]];
        const color = Color.get(payload.color ?? "blue");
        let grepModel = model as GRepModel;
        const drawable: IDrawable = {
            geometry: {
                type: 'polyline',
                positions: positions,
                position: position,
                rotation: rotation
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
        const position = payload.position ?? [0, 0, 0];
        const rotation = payload.rotation ?? [0, 0, 0];
        const width = payload.width ?? 1.0;
        const height = payload.height ?? 1.0;
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
                position: position,
                rotation: rotation
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
        const position = payload.position ?? [0, 0, 0];
        const rotation = payload.rotation ?? [0, 0, 0];
        const radius = payload.radius ?? 1.0;
        const color = Color.get(payload.color ?? "blue");

        let grepModel = model as GRepModel;
        const drawable: IDrawable = {
            geometry: {
                type: 'sphere',
                radius: radius,
                position: position,
                rotation: rotation
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
        const position = payload.position ?? [0, 0, 0];
        const rotation = payload.rotation ?? [0, 0, 0];
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
                depth: depth,
                position: position,
                rotation: rotation
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
        const position = payload.position ?? [0, 0, 0];
        const rotation = payload.rotation ?? [0, 0, 0];
        const radius = payload.radius ?? 1.0;      // radius of the cylinder
        const depth = payload.depth ?? 2.0;      // length in z direction
        const color = Color.get(payload.color ?? "yellow");
        let grepModel = model as GRepModel;
        const drawable: IDrawable = {
            geometry: {
                type: 'cylinder',
                radius: radius,
                depth: depth,
                position: position,
                rotation: rotation
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
        const position = payload.position ?? [0, 0, 0];
        const rotation = payload.rotation ?? [0, 0, 0];
        const paths = payload.paths ?? [[[-1, -1], [1, -1], [1, 1], [-1, 1], [-1, -1]]];
        const color = Color.get(payload.color ?? "green");
        let grepModel = model as GRepModel;
        const drawable: IDrawable = {
            geometry: {
                type: 'profile',
                paths: paths,
                position: position,
                rotation: rotation
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
        const position = payload.position ?? [0, 0, 0];
        const rotation = payload.rotation ?? [0, 0, 0];
        const paths = payload.paths ?? [[[-1, -1], [1, -1], [1, 1], [-1, 1], [-1, -1]]];
        const color = Color.get(payload.color ?? "green");
        let grepModel = model as GRepModel;
        const drawable: IDrawable = {
            geometry: {
                type: 'contour',
                paths: paths,
                position: position,
                rotation: rotation
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
export const addPointActionDef = { type: "Gfx::addPoint", function: addPoint };
export const addLineActionDef = { type: "Gfx::addLine", function: addLine };
export const addPolylineActionDef = { type: "Gfx::addPolyline", function: addPolyline };
export const addPlaneActionDef = { type: "Gfx::addPlane", function: addPlane };
export const addSphereActionDef = { type: "Gfx::addSphere", function: addSphere };
export const addBlockActionDef = { type: "Gfx::addBlock", function: addBlock };
export const addCylinderActionDef = { type: "Gfx::addCylinder", function: addCylinder };
export const addProfileActionDef = { type: "Gfx::addProfile", function: addProfile };
export const addContourActionDef = { type: "Gfx::addContour", function: addContour };
