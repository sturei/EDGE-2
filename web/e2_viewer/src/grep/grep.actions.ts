import { Document } from "../document/document";

function ping(_doc: Document, _payload: any): void {
    // This action just writes "pong" to stderr. Useful for testing that the pieces are connected.
    console.log("pong");
}

export const pingActionDef = { type: "Gfx::ping", function: ping };
