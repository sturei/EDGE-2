import { Document } from "../document/document";
import { Model } from "../document/model";
import { GRepModel } from "../grep/grepModel";
import { GPoint, GLine, GPlane, GSphere, GBlock } from "./gitem";

function ping(_doc: Document, _payload: any): void {
    // This action just writes "pong" to stderr. Useful for testing that the pieces are connected.
    console.log("pong");
}

function addGPoint(doc: Document, payload: any): void {
    const store = doc.storeAt("grep");
    store.changeState((model: Model) => {
        let grepModel = model as GRepModel; 
        const size = payload.size ?? 1.0;    // size of the point
        console.log(`Adding GPoint of size: ${size}`);
        const point = new GPoint(size);
        grepModel.addGItem(point);
    });
    console.log("added GPoint");      // ---IGNORE---
}

function addGLine(doc: Document, payload: any): void {
    const store = doc.storeAt("grep");
    store.changeState((model: Model) => {
        const length = payload.length ?? 1.0;    // length of the line
        let grepModel = model as GRepModel;
        const line = new GLine(length);
        grepModel.addGItem(line);
    });
    console.log("added GLine");      // ---IGNORE---
}

function addGPlane(doc: Document, payload: any): void {
    const store = doc.storeAt("grep");
    store.changeState((model: Model) => {
        const width = payload.width ?? 1.0;      // width of the plane
        const height = payload.height ?? 1.0;    // height of the plane
        let grepModel = model as GRepModel;
        const plane = new GPlane(width, height);
        grepModel.addGItem(plane);
    });
    console.log("added GPlane");      // ---IGNORE---
}

function addGSphere(doc: Document, payload: any): void {
    const store = doc.storeAt("grep");
    store.changeState((model: Model) => {
        const radius = payload.radius ?? 1.0;    // radius of the sphere
        let grepModel = model as GRepModel;
        const sphere = new GSphere(radius);
        grepModel.addGItem(sphere);
    });
    console.log("added GSphere");      // ---IGNORE---
}

function addGBlock(doc: Document, payload: any): void {
    const store = doc.storeAt("grep");
    store.changeState((model: Model) => {
        const width = payload.width ?? 2.0;      // length in x direction
        const height = payload.height ?? 2.0;    // length in y direction
        const depth = payload.depth ?? 2.0;      // length in z direction
        let grepModel = model as GRepModel;
        const block = new GBlock(width, height, depth);
        grepModel.addGItem(block);
    });
    console.log("added GBlock");      // ---IGNORE---
}


export const pingActionDef = { type: "ping", function: ping };
export const addGPointActionDef = { type: "addGPoint", function: addGPoint };
export const addGLineActionDef = { type: "addGLine", function: addGLine };
export const addGPlaneActionDef = { type: "addGPlane", function: addGPlane };
export const addGSphereActionDef = { type: "addGSphere", function: addGSphere };
export const addGBlockActionDef = { type: "addGBlock", function: addGBlock };



