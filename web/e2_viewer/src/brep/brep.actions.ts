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

async function addSheetRectangle(doc: Document, payload: any): Promise<void> {

    // create a rectangular sheet with specified lowerLeft and upperRight corners

    const response = await fetch(modellingActions, {
        method: 'POST',
        headers: {
            'Accept': 'application/json',
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({
            type: "addSheetRectangle",
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

export const pingModellerActionDef = { type: "pingModeller", function: pingModeller };
export const addSheetRectangleActionDef = { type: "addSheetRectangle", function: addSheetRectangle };
