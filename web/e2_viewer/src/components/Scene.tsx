/** This component sets up the 3D scene, including lights and cameras. 
 * It also creates the Scene store and Model within the Document, and uses that to update scene dynamically in response to 
 * state changes in the Scene store.
 */
  
import { useContext, useEffect, useState } from "react";
import { OrbitControls } from '@react-three/drei'
import { GRepModel } from '../grep/grepModel';
import { Store } from '../document/store' 
import { Drawlist } from './Drawlist.tsx'
import { DocumentContext } from '../Contexts.ts';
import { type IDrawable } from "../grep/drawable";

interface ISceneReactState {
    drawlist: IDrawable[]
}
const emptyDrawlist:ISceneReactState = {drawlist: new Array<IDrawable>()};

export function Scene(){
    const document = useContext(DocumentContext);
    const [reactState, setReactState] = useState(emptyDrawlist);

    /** Sets up the Scene Model and Store */
    useEffect(() => {
        console.log(" Setting up the Scene's Model and Store");
        const sceneModel = new GRepModel();
        const sceneStore = new Store(sceneModel, postStateChangeCallback);
        document.addStore('scene', sceneStore);
    }, []);

    /** Updates the React state whenever the Scene model changes */
    function postStateChangeCallback() {
        console.log("Scene model state changed. Updating react state.");
        const sceneStore = document.getStore('scene');
        const sceneModel = sceneStore.getModel() as GRepModel;
        const newState:ISceneReactState = { ...reactState };
        newState.drawlist = sceneModel.drawlist().slice();
        console.log(" New state:", newState);
        setReactState(newState);
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
