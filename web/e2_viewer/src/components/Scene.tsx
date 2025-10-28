/** This component sets up the 3D scene, including lights and cameras. It updates the scene dynamically from 
 * the drawlist in the document's GRepModel. */
  
  
import { useContext, useEffect, useState } from "react";
import { OrbitControls } from '@react-three/drei'

import { GRepModel } from '../grep/grepModel';
import { Store } from '../document/store' 
import { Drawlist } from './Drawlist.tsx'
import { updateReactStateFromDocument, type IReactState } from '../grep/reactGate.ts'

import { DocumentContext } from '../Contexts.ts';
import { type IDrawable } from "./Drawable";

const emptyDrawlist:IReactState = {drawlist: new Array<IDrawable>()};
  
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
        const sceneStore = document.storeAt('scene');
        const grepModel = sceneStore?.getModel() as GRepModel;

        console.log("Document state changed. Current drawlist:");

        for (let i = 0; i < grepModel.numGItems(); ++i) {
            const gitem = grepModel.gItem(i);
            console.log(` GItem ${i}:`, gitem);
        }
        let newReactState = updateReactStateFromDocument(reactState, document);
        setReactState(newReactState);
    }

    return (
        <>
        <ambientLight color={0x505050} />
        <directionalLight intensity={0.5} position={[0.75, 0.25, 0.25]} />
        <directionalLight intensity={0.5} position={[-0.75, 0.25, 0.25]} />
        {/*<Drawable args={reactState.drawlist[0]} />*/}
        {/*<Drawable args={{geometry: {type: 'box', width: 3, height: 2, depth: 1}}} />*/}
        {<Drawlist drawlist={reactState.drawlist} />}
        <OrbitControls />
        </>
    )
}
