import { Document } from "../../document/document";
import { Model } from "../../document/model";
import { GRepModel } from "../grepModel";
import type { ISdfNode } from "./sdfNode";

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

export const addSdfNodeActionDef = { type: "Gfx::addSdfNode", function: addSdfNode };
export const updateSdfSceneActionDef = { type: "Gfx::updateSdfScene", function: updateSdfScene };
export const clearSdfSceneActionDef = { type: "Gfx::clearSdfScene", function: clearSdfScene };      