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
    if (data.response.status === "ERROR") {
        console.warn(`Error from modeller server: ${data.response.reason}`);
    }
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

async function addProfile(doc: Document, payload: any): Promise<void> {
    await postAction(doc, "Profiles::addPrimitive", payload);
}

async function addPrimitive(doc: Document, payload: any): Promise<void> {
    await postAction(doc, "Features::addPrimitive", payload);
}

async function addExtrusion(doc: Document, payload: any): Promise<void> {
    await postAction(doc, "Features::addExtrusion", payload);
}

async function triggerClientUpdate(doc: Document, payload: any): Promise<void> {
    await postAction(doc, "Features::ping", payload);
}

export const pingModellerActionDef = { type: "Modeller::ping", function: pingModeller };

export const addProfileActionDef = { type: "Profiles::addPrimitive", function: addProfile };
export const addPrimitiveActionDef = { type: "Features::addPrimitive", function: addPrimitive };
export const addExtrusionActionDef = { type: "Features::addExtrusion", function: addExtrusion };
export const triggerClientUpdateActionDef = { type: "Features::ping", function: triggerClientUpdate };


