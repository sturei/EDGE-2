import { Document } from '../document/document';
import { type IDrawable, type IBoxGeometry } from '../components/Drawable';
import {GBlock, GItem } from './gitem';
import type { GRepModel } from './grepModel';

export interface IReactState {
    drawlist: IDrawable[]
}

export function updateReactStateFromDocument(oldState: IReactState, document: Document) : IReactState {
    const grepStore = document.storeAt('scene');
    if (!grepStore) {
        console.error("updateReactStateFromDocument: no 'scene' store found in document.");
        return oldState;
    }
    const grepModel = grepStore.getModel() as GRepModel;
    //for (const item of grepModel.drawlist) {
    //    console.log("Drawlist item:", item);
   // }

   const newState:IReactState = { ...oldState };
   newState.drawlist = new Array<IDrawable>();

    for (let i = 0; i < grepModel.numGItems(); ++i) {
        const gItem = grepModel.gItem(i);
        console.log(`GItem ${i}:`, gItem);
        if (!gItem) continue;   // TODO: errors policy

        // TODO if drawable has not changed, set it to the saved state. Use a handle stored in GItemBase for this?
        const gBlock = gItem as GBlock
        const drawable: IDrawable = {
            geometry: {
                type: 'box',
                width: gBlock.width(),
                height: gBlock.height(),
                depth: gBlock.depth()
            },
            //appearance: {
            //    type: "StandardAppearance",
            //    color: 0x00ff00
            //}
        };
        newState.drawlist.push(drawable);

        /*
        if (gitem.type() === "GPoint") {
        } else if (gitem.type() === "GLine") {
        } else if (gitem.type() === "GPlane") {
        } else if (gitem.type() === "GSphere") {
        } else if (gitem.type() === "GBlock") {
        } else {
            console.warn(`Unknown GItem type: ${gitem.type()}`);
        } 
            */      
    }

    console.log("React state updated from document changes, new state:", newState);
    return newState;
}