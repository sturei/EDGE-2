/** This component sets up the 3D graphics scene, including lights and cameras. 
 * It also creates the Scene store and Model within the Document, and uses that to update scene dynamically in response to 
 * state changes in the Scene store.
 */
  
import { useContext, useEffect, useState } from "react";
import { OrbitControls } from '@react-three/drei'
import { GRepModel } from '../grep/grepModel';
import { Store } from '../document/store' 
import { Drawlist } from './Drawlist'
import { DocumentContext } from '../Contexts';
import { type IDrawable } from "../grep/drawable";
import { RaymarchedScene } from "./Raymarch";
import type { IShaderNode } from '../grep/shaderNode.ts'    

interface ISceneReactState {
    drawlist: IDrawable[],
    nodes: IShaderNode[],
    guid: string
}
const initialState:ISceneReactState = {drawlist: new Array<IDrawable>(), nodes: new Array<IShaderNode>(), guid: "Initial State"};

export function Scene(){
    const document = useContext(DocumentContext);
    const [reactState, setReactState] = useState(initialState);

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
        newState.drawlist = sceneModel.drawlist().slice();   // Copy to force React to see it as changed in case elements have been added/removed/changed. Alternatives exist e.g. make it immutable etc.
        if (sceneModel.sdfGuid() !== newState.guid) {
            newState.guid = sceneModel.sdfGuid();
            newState.nodes = sceneModel.shaderNodes();   // Don't need to copy because the Guid has changed.
            console.log(" SDF scene updated, new GUID:", newState.guid);
        }
        console.log(" New state:", newState);
        setReactState(newState);
    }

    return (
        <>
        <ambientLight color={0x505050} />
        <directionalLight intensity={0.5} position={[0.75, 0.25, 0.25]} />
        <directionalLight intensity={0.5} position={[-0.75, 0.25, 0.25]} />
        <Drawlist drawlist={reactState.drawlist} />
        <RaymarchedScene nodes={reactState.nodes} guid={reactState.guid} />
        <OrbitControls />
        </>
    )
}
