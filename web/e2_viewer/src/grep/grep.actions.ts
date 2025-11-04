import { Document } from "../document/document";
import { Model } from "../document/model";
import { GRepModel } from "../grep/grepModel";
import { Color, type IDrawable } from "../grep/drawable";

function ping(_doc: Document, _payload: any): void {
    // This action just writes "pong" to stderr. Useful for testing that the pieces are connected.
    console.log("pong");
}

function addGPoint(doc: Document, payload: any): void {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        let sceneModel = model as GRepModel; 
        const size = payload.size ?? 1.0;
        const position = payload.position ?? [0, 0, 0];
        const color = payload.color ?? Color.Red;
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
    console.log("added GPoint");      // ---IGNORE---
}

function addGLine(doc: Document, payload: any): void {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        const start = payload.start ?? [-1, 0, 0];
        const end = payload.end ?? [1, 0, 0];
        const color = payload?.color ?? Color.Blue;
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
    console.log("added GLine");      // ---IGNORE---
}

function addGPlane(doc: Document, payload: any): void {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        const width = payload.width ?? 1.0;
        const height = payload.height ?? 1.0;
        const color = payload?.color ?? Color.Blue;

        let grepModel = model as GRepModel;
        const drawable: IDrawable = {
            geometry: {
                type: 'plane',
                width: width,
                height: height
            },
            appearance: {
                type: 'mesh',
                color: color
            }

        };
        grepModel.addDrawable(drawable);    
    });
    console.log("added GPlane");      // ---IGNORE---
}

function addGSphere(doc: Document, payload: any): void {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        const radius = payload.radius ?? 1.0;
        const color = payload?.color ?? Color.Blue;

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
    console.log("added GSphere");      // ---IGNORE---
}

function addGBlock(doc: Document, payload: any): void {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        const width = payload.width ?? 2.0;      // length in x direction
        const height = payload.height ?? 2.0;    // length in y direction
        const depth = payload.depth ?? 2.0;      // length in z direction
        const color = payload?.color ?? Color.Green;
        let grepModel = model as GRepModel;
        const drawable: IDrawable = {
            geometry: {
                type: 'box',
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
        console.log("added GBlock");      // ---IGNORE---
    });
}

function addGShape(doc: Document, payload: any): void {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        const points = payload.points ?? [];
        let grepModel = model as GRepModel;
        const drawable: IDrawable = {
            geometry: {
                type: 'shape',
                points: points
            }
        };
        grepModel.addDrawable(drawable);
    });
    console.log("added GShape");      // ---IGNORE---
}


export const pingActionDef = { type: "ping", function: ping };
export const addGPointActionDef = { type: "addGPoint", function: addGPoint };
export const addGLineActionDef = { type: "addGLine", function: addGLine };
export const addGPlaneActionDef = { type: "addGPlane", function: addGPlane };
export const addGSphereActionDef = { type: "addGSphere", function: addGSphere };
export const addGBlockActionDef = { type: "addGBlock", function: addGBlock };
export const addGShapeActionDef = { type: "addGShape", function: addGShape };

