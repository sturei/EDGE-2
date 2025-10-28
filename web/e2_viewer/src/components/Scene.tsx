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

/** this function reconciles the grepModel with the React model whenever the document (or more specifically the scene store within it) changes.
 * The GRep model is independent of any particular graphics library. The React model is specific to react-three-fiber.
 * At present, the GRep model is very simple - just primitive shapes - and so the mapping is trivial. In future, more complex shapes and properties 
 * will be added to the GRep model. These would map to composite objects in react-three-fiber.
 */
function updateReactStateFromDocument(oldState: ISceneReactState, document: Document) : ISceneReactState {
    const sceneStore = document.getStore('scene');
    const grepModel = sceneStore.getModel() as GRepModel;
    const newState:ISceneReactState = { ...oldState };

    // For now, we just clear and rebuild the React drawlist from scratch. 
    // In future, we could optimize by updating only what changed since the last update.
    newState.drawlist = new Array<IDrawable>();

    for (let i = 0; i < grepModel.numGItems(); ++i) {
        const gItem = grepModel.gItem(i);
        console.log(`GItem ${i}:`, gItem);
        if (!gItem) continue;   // TODO: errors policy

        const gBlock = gItem as GBlock
        const drawable: IDrawable = {
            geometry: {
                type: 'box',
                width: gBlock.width(),
                height: gBlock.height(),
                depth: gBlock.depth()
            },
        };
        newState.drawlist.push(drawable);

        /*
        TODO:
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
