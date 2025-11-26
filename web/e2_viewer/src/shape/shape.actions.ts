import { Document, type ActionSpec } from "../document/document";

const apiServer = 'http://localhost:3000';
const modellingActions = apiServer + '/modelling/actions';

async function postAction(doc: Document, type:string,payload: any): Promise<void> {
    // post the specified action to the modeller server.
    const response = await fetch(modellingActions, {
        method: 'POST',
        headers: {
            'Accept': 'application/json',
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({
            type: type,
            payload: payload
        })
    });

    // dispatch any client actions returned by the modeller.
    const data = await response.json();
    console.log("Modeller response:", data);
    const clientActions: ActionSpec[]= data.response.clientActions as ActionSpec[];
    for (const action of clientActions) {
        doc.dispatchAction(action);
    }
}

async function pingModeller(doc: Document, _payload: any): Promise<void> {
    // Ping the modeller server. 
    // The server posts "pong" on its stderr stream and returns "OK".
    await postAction(doc, "Modeller::ping", {});
}
async function addWireRectangle(doc: Document, payload: any): Promise<void> {
    // Create a rectangular wire with specified lowerLeft and upperRight corners. 
    // The server creates the wire as a sketch in its shape store, and returns client actions to add the appropriate graphics to the scene.
    await postAction(doc, "Sketches::addRectangle", payload);
}
async function addWireRoundRect(doc: Document, payload: any): Promise<void> {
    // Create a rounded-rectangular wire with specified corners and cornerRadius. 
    // The server creates the wire as a sketch in its shape store, and returns client actions to add the appropriate graphics to the scene.
    await postAction(doc, "Sketches::addRoundRect", payload);
}
async function addSheetRectangle(doc: Document, payload: any): Promise<void> {
    // Create a rectangular sheet with specified lowerLeft and upperRight corners. 
    // The server creates the sheet as a profile in its shape store, and returns client actions to add the appropriate graphics to the scene.
    await postAction(doc, "Profiles::addRectangle", payload);
}
async function addSheetRoundRect(doc: Document, payload: any): Promise<void> {
    // Create a rounded rectangular sheet with specified corners and cornerRadius. 
    // The server creates the sheet as a profile in its shape store, and returns client actions to add the appropriate graphics to the scene.
    await postAction(doc, "Profiles::addRoundRect", payload);
}

async function addInfiniteRectangle(doc: Document, payload: any): Promise<void> {
    // Create an infinite rectangular extrusion with spefified lowerLeft and upperRight corners.
    // The server creates the object as frep in its shape store, and returns client actions to add the appropriate graphics to the scene.
    await postAction(doc, "Objects::addInfiniteRectangle", payload);
}

export const pingModellerActionDef = { type: "Modeller::ping", function: pingModeller };
export const addWireRectangleActionDef = { type: "Sketches::addRectangle", function: addWireRectangle };
export const addWireRoundRectActionDef = { type: "Sketches::addRoundRect", function: addWireRoundRect };
export const addSheetRectangleActionDef = { type: "Profiles::addRectangle", function: addSheetRectangle };
export const addSheetRoundRectActionDef = { type: "Profiles::addRoundRect", function: addSheetRoundRect };
export const addRectangleActionDef = { type: "Objects::addInfiniteRectangle", function: addInfiniteRectangle };          
