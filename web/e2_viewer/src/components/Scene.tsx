/** This component sets up the 3D scene, including lights and cameras. 
 * It also creates the Scene store and Model within the Document, and uses that to update scene dynamically in response to 
 * state changes in the Scene store.
 */
  
import { useContext, useEffect, useState } from "react";
import { OrbitControls } from '@react-three/drei'
import { GRepModel } from '../grep/grepModel';
import { Store } from '../document/store' 
import { Document } from '../document/document';
import { GBlock, type IGItem, GItemType, GSphere } from '../grep/gitem';
import { Drawlist } from './Drawlist.tsx'
import { DocumentContext } from '../Contexts.ts';
import { type IDrawable } from "./Drawable";

const emptyDrawlist:ISceneReactState = {drawlist: new Array<IDrawable>()};

interface ISceneReactState {
    drawlist: IDrawable[]
}

function drawableFromGItem(gItem: IGItem): IDrawable {

    let drawable: IDrawable = {};

    if (gItem.type === GItemType.GBlock) {
        const gBlock = gItem as GBlock;
        drawable = {
            geometry: {
                type: 'box',
                width: gBlock.width(),
                height: gBlock.height(),
                depth: gBlock.depth()
            }
        }
    }
    else if (gItem.type === GItemType.GSphere) {
        const gSphere = gItem as GSphere;
        drawable = {
            geometry: {
                type: 'sphere',
                radius: gSphere.radius()
            }
        }
    }
    return drawable;
}

/** this function reconciles the grepModel with the React model whenever the document (or more specifically the scene store within it) changes.
 * The GRep model is independent of any particular graphics library. The React model is specific to react-three-fiber.
 * At present, the GRep model is very simple - just primitive shapes - and so the mapping is trivial. In future, more complex shapes and properties
 * will be added to the GRep model. A single complex GItem could map to several drawables, probably in a group.
 */
function updateReactStateFromDocument(oldState: ISceneReactState, document: Document) : ISceneReactState {
    const sceneStore = document.getStore('scene');
    const grepModel = sceneStore.getModel() as GRepModel;
    const newState:ISceneReactState = { ...oldState };

    // For now, we just clear and rebuild the React drawlist from scratch. 
    // In future, we could optimize by updating only what changed since the last update.
    newState.drawlist = new Array<IDrawable>();

    for (const gItem of grepModel.drawlist()) {
        console.log(`GItem:`, gItem);
        const drawable: IDrawable = drawableFromGItem(gItem);
        newState.drawlist.push(drawable);
    }

    console.log("React state updated from document changes, new state:", newState);
    return newState;
}

export function Scene(){
    const document = useContext(DocumentContext);
    const [reactState, setReactState] = useState(emptyDrawlist);

    /** Sets up the Scene Model and Store */
    useEffect(() => {
        console.log(" Setting up the Scene's Model and Store");
        const grepModel = new GRepModel();
        const sceneStore = new Store(grepModel, postStateChangeCallback);
        document.addStore('scene', sceneStore);
    }, []);

    /** Updates the React state whenever the Scene model changes */
    function postStateChangeCallback() {
        console.log("Scene model state changed. Updating Scene React state.");
        let newReactState = updateReactStateFromDocument(reactState, document);
        setReactState(newReactState);
    }

    /** returns the jsx that puts the drawlist into the scene */
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
