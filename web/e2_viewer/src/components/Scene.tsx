/** This component sets up the 3D scene, including lights and cameras. It updates the scene dynamically from 
 * the drawlist in the document's GRepModel. */
  
  
import { useContext, useEffect, useState } from "react";
import { OrbitControls } from '@react-three/drei'
import { GRepModel } from '../grep/grepModel';
import { Store } from '../document/store' 
import { Document } from '../document/document';
import { GBlock } from '../grep/gitem';
import { Drawlist } from './Drawlist.tsx'

import { DocumentContext } from '../Contexts.ts';
import { type IDrawable } from "./Drawable";

interface ISceneReactState {
    drawlist: IDrawable[]
}

const emptyDrawlist:ISceneReactState = {drawlist: new Array<IDrawable>()};

function updateReactStateFromDocument(oldState: ISceneReactState, document: Document) : ISceneReactState {
    const sceneStore = document.storeAt('scene');
    if (!sceneStore) {
        console.error("updateReactStateFromDocument: no 'scene' store found in document.");
        return oldState;
    }
    const grepModel = sceneStore.getModel() as GRepModel;
    //for (const item of grepModel.drawlist) {
    //    console.log("Drawlist item:", item);
   // }

   const newState:ISceneReactState = { ...oldState };
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

export function Scene(){
    const document = useContext(DocumentContext);
    const [reactState, setReactState] = useState(emptyDrawlist);

    useEffect(() => {
        // Sets up the Document and Store for the GRepModel. The GRepModel holds the drawlist.

        console.log(" Setting up the Scene's Model and Store");
        const grepModel = new GRepModel();
        const sceneStore = new Store(grepModel, postStateChangeCallback);
        document.addStore('scene', sceneStore);

    }, []);

    /** Updates the React state whenever the drawlist changes */
    function postStateChangeCallback() {
        console.log("Document state changed. Updating Scene React state.");
        let newReactState = updateReactStateFromDocument(reactState, document);
        setReactState(newReactState);
    }

    return (
        <>
        <ambientLight color={0x505050} />
        <directionalLight intensity={0.5} position={[0.75, 0.25, 0.25]} />
        <directionalLight intensity={0.5} position={[-0.75, 0.25, 0.25]} />
        {<Drawlist drawlist={reactState.drawlist} />}
        <OrbitControls />
        </>
    )
}
