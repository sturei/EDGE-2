import { Document } from "../../document/document";
import { Model } from "../../document/model";
import { GRepModel } from "../grepModel";
import type { ISdfNode } from "./sdfNode";

// Adds the specified SDF node to the GRepModel's SDF scene. 
// The application must build the node tree top-down. Children are attached to their parents automatically.
// The application must call updateSdfScene() once the modifications to the tree are complete. 
async function addSdfNode(doc: Document, payload: any): Promise<void> {
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
            grepModel.addSdfNode(node);
        }
        else if (type === 'block') {
            const node: ISdfNode = {
                type: 'block',
                pathName: payload.pathName ?? 'block',
                width: payload.width ?? 1.0,
                height: payload.height ?? 1.0,
                depth: payload.depth ?? 1.0
            };
            grepModel.addSdfNode(node);
        }
        else if (type === 'cylinder') {
            const node: ISdfNode = {
                type: 'cylinder',
                pathName: payload.pathName ?? 'cylinder',
                radius: payload.radius ?? 1.0,
                depth: payload.depth ?? 1.0
            };
            grepModel.addSdfNode(node);
        }
        else if (type === 'circle') {
            const node: ISdfNode = {
                type: 'circle',
                pathName: payload.pathName ?? 'circle',
                radius: payload.radius ?? 1.0
            };
            grepModel.addSdfNode(node);
        }
        else if (type === 'rectangle') {
            const node: ISdfNode = {
                type: 'rectangle',
                pathName: payload.pathName ?? 'rectangle',
                width:  payload.width ?? 1.0,
                height: payload.height ?? 1.0
            };
            grepModel.addSdfNode(node);
        }
        else if (type === 'roundRect') {
            const node: ISdfNode = {
                type: 'roundRect',
                pathName: payload.pathName ?? 'roundRect',
                width:  payload.width ?? 1.0,
                height: payload.height ?? 1.0,
                cornerRadius: payload.cornerRadius ?? 0.1
            };
            grepModel.addSdfNode(node);
        }
        else if (type === 'profile') {
            const node: ISdfNode = {
                type: 'profile',
                pathName: payload.pathName ?? 'profile',
                paths: payload.paths ?? [[[-1, -1], [1, -1], [1, 1], [-1, 1], [-1, -1]]],
            };
            grepModel.addSdfNode(node);
        }
        else if (type === 'extrusion') {
            const node : ISdfNode = {
                type: 'extrusion',
                pathName: payload.pathName ?? 'extrusion',
                depth: payload.depth ?? 2.0
            };
            grepModel.addSdfNode(node);
        }
        else if (type === 'halfSpace') {
            const node : ISdfNode = {
                type: 'halfSpace',
                pathName: payload.pathName ?? 'halfSpace',
            };
            grepModel.addSdfNode(node);
        }
        else if (type === 'union') {
            const node : ISdfNode = {
                type: 'union',
                pathName: payload.pathName ?? 'union'
            };
            grepModel.addSdfNode(node);
        }
        else if (type === 'intersection') {
            const node : ISdfNode = {
                type: 'intersection',
                pathName: payload.pathName ?? 'intersection'
            };
            grepModel.addSdfNode(node);
        }
        else if (type === 'complement') {
            const node : ISdfNode = {
                type: 'complement',
                pathName: payload.pathName ?? 'complement'
            };
            grepModel.addSdfNode(node);
        }
        else if (type === 'translation') {
            const node : ISdfNode = {
                type: 'translation',
                pathName: payload.pathName ?? 'translation',
                position: payload.position ?? [0, 0, 0]   
            };
            grepModel.addSdfNode(node);
        }
        else if (type === 'rotation') {
            const node : ISdfNode = {
                type: 'rotation',
                pathName: payload.pathName ?? 'rotation',
                angles: payload.angles ?? [0, 0, 0]
            };
            grepModel.addSdfNode(node);
        }
        else if (type === 'gyroid') {
            const node : ISdfNode = {
                type: 'gyroid',
                pathName: payload.pathName ?? 'gyroid',
                cellSize: payload.cellSize ?? 1.0
            };
            grepModel.addSdfNode(node);
        }
        else if (type === 'repetition') {
            const node : ISdfNode = {
                type: 'repetition',
                pathName: payload.pathName ?? 'repetition',
                cellSize: payload.cellSize ?? 1.0
            };
            grepModel.addSdfNode(node);
        }
        else {
            console.error(`addSdfNode action: unknown type '${type}'`);
        }
    });
    console.log("added SDF Node");      // ---DEBUG---
}

// This triggers the regeneration of the SDF scene in Threejs and thence in the GPU.
async function updateSdfScene(doc: Document, _payload: any): Promise<void> {
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

// This clears all SDF nodes from the GRepModel's SDF scene.
async function clearSdfScene(doc: Document, _payload: any): Promise<void> {
    const store = doc.getStore("scene");
    store.changeState((model: Model) => {
        let grepModel = model as GRepModel;
        grepModel.clearSdfNodes();
        console.log("Cleared SDF scene nodes.");
    }
    );
}

export const addSdfNodeActionDef = { type: "Gfx::addSdfNode", function: addSdfNode };
export const updateSdfSceneActionDef = { type: "Gfx::updateSdfScene", function: updateSdfScene };
export const clearSdfSceneActionDef = { type: "Gfx::clearSdfScene", function: clearSdfScene };      