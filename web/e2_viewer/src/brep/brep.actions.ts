import { Document, type ActionSpec } from "../document/document";

const apiServer = 'http://localhost:3000';
const modellingActions = apiServer + '/modelling/actions';

async function pingModeller(_doc: Document, _payload: any): Promise<void> {

    // ping the modeller server. All we expect is a JSON "OK" response. (TODO - put "pong" in the response body? )

    const response = await fetch(modellingActions, {
        method: 'POST',
        headers: {
            'Accept': 'application/json',
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({
            type: "ping",
            payload: {}
        })
    });
    const data = await response.json();
    console.log("Modeller response:", data);

}

async function executeModellingAction(doc: Document, type:string,payload: any): Promise<void> {

    // create a rectangular sheet with specified lowerLeft and upperRight corners

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
    const data = await response.json();
    console.log("Modeller response:", data);
    const clientActions: ActionSpec[]= data.response.clientActions as ActionSpec[];
    for (const action of clientActions) {
        doc.dispatchAction(action);
    }
}


async function addSheetRectangle(doc: Document, payload: any): Promise<void> {
    // create a rectangular sheet with specified lowerLeft and upperRight corners
    await executeModellingAction(doc, "addSheetRectangle", payload);
}
async function addSheetRoundRect(doc: Document, payload: any): Promise<void> {
    // create a rectangular sheet with specified lowerLeft and upperRight corners, and cornerRadius
    await executeModellingAction(doc, "addSheetRoundRect", payload);
}

export const pingModellerActionDef = { type: "pingModeller", function: pingModeller };
export const addSheetRectangleActionDef = { type: "addSheetRectangle", function: addSheetRectangle };
export const addSheetRoundRectActionDef = { type: "addSheetRoundRect", function: addSheetRoundRect };
