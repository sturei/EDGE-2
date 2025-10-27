import { useState } from 'react'
import { Canvas } from '@react-three/fiber'
import { OrbitControls } from '@react-three/drei'
import reactLogo from './assets/react.svg'
import tailwindLogo from './assets/tailwindcss-mark.svg'
import daisyuiLogo from './assets/daisyui-mark-rotating.svg'
import threejsLogo from './assets/threejs-logo.svg'
import viteLogo from '/vite.svg'
import './App.css'

import {GRepModel} from './grep/grepModel';
import { Store } from './document/store'
import { Document } from './document/document'
import * as grepActions from './grep/grep.actions.ts'  

import { Drawable, type IDrawable } from './components/Drawable.tsx'
import { updateReactStateFromDocument, type IReactState } from './grep/reactGate.ts'

function App() {
  const [count, setCount] = useState(0)
  const initialReactState:IReactState = {drawlist: new Array<IDrawable>()};
  const [reactState, setReactState] = useState(initialReactState);

  // Set up the Document and Store for the GRepModel. The GRepModel holds the drawlist.

  console.log(" Setting up GRepModel, Store, and Document");
  
  const grepModel = new GRepModel();
  const grepStore = new Store(grepModel, postStateChangeCallback);
  const grepDocument = new Document(new Map([['grep', grepStore]]));

  // Register actions with the document. These actions are the only valid way for the application to manipulate the drawlist.
  grepDocument.registerActionFunction(grepActions.pingActionDef);
  grepDocument.registerActionFunction(grepActions.addGPointActionDef);
  grepDocument.registerActionFunction(grepActions.addGLineActionDef);
  grepDocument.registerActionFunction(grepActions.addGPlaneActionDef);
  grepDocument.registerActionFunction(grepActions.addGSphereActionDef);
  grepDocument.registerActionFunction(grepActions.addGBlockActionDef);

  function postStateChangeCallback() {
    console.log("Document state changed. Current drawlist:");
    for (let i = 0; i < grepModel.numGItems(); ++i) {
      const gitem = grepModel.gItem(i);
      console.log(` GItem ${i}:`, gitem);
    }
    let newReactState = updateReactStateFromDocument(reactState, grepDocument);
    setReactState(newReactState);
  }

  function AcknowledgementsPanel() {
    return (
      <>
        <div>
          <div className="join">
            <a className="join-item" href="https://vite.dev" target="_blank">
              <div className="tooltip" data-tip="Vite - Build tooling">
                <img src={viteLogo} className="logo" alt="Vite logo" />
              </div>
            </a>
            <a className="join-item" href="https://react.dev" target="_blank">
              <div className="tooltip" data-tip="React - UI framework">
              <img src={reactLogo} className="logo" alt="React logo" />
              </div>
            </a>
            <a className="join-item" href="https://tailwindcss.com" target="_blank">
              <div className="tooltip" data-tip="Tailwind - CSS framework">
              <img src={tailwindLogo} className="logo" alt="Tailwind CSS logo" />
              </div>
            </a>
            <a className="join-item" href="https://daisyui.com" target="_blank">
              <div className="tooltip" data-tip="daisyUI - CSS component library">
              <img src={daisyuiLogo} className="logo" alt="daisyUI logo" />
              </div>
            </a>
            <a className="join-item" href="https://threejs.org/" target="_blank">
              <div className="tooltip" data-tip="three.js - WebGL library">
              <img src={threejsLogo} className="logo" alt="three.js logo" />
              </div>
            </a>
            <a className="join-item" href="https://github.com/pmndrs/react-three-fiber" target="_blank">
              <div className="tooltip" data-tip="react-three-fiber - React + three.js">
              <img src={threejsLogo} className="logo" alt="three.js logo" />
              </div>
            </a>
            <a className="join-item" href="https://github.com/pmndrs/drei" target="_blank">
              <div className="tooltip" data-tip="drei - R3F helper components">
              <img src={threejsLogo} className="logo" alt="three.js logo" />
              </div>
            </a>
          </div>
          <p className="read-the-docs">
            Click on the logos above to learn more about the technologies used in this application.
          </p>
          <div className="basis-64">
            <button className="btn"
              onClick={() => setCount((count) => count + 1)}>
              Does React react? {count}
            </button>
          </div>
        </div>
      </>
    )
  }

  function dispatchAction(formData: FormData) {
    const actionText = formData.get("actionInput") as string;
    try {

      console.log(`Dispatching action: ${actionText}`);

      const action = JSON.parse(actionText);
      const dispatched = grepDocument.dispatchAction(action);
      if (!dispatched) {
        console.error(`Action type "${action.type}" not recognized. Was it registered correctly?`);
      }
    } catch (error) {
      console.error("Failed to parse action input as JSON:", error);
    }
  }

  const actionSuggestions = [
    '{"type": "ping", "payload": {} }',
    '{"type":"addGPoint", "payload":{"size":0.2}}',
    '{"type":"addGLine", "payload":{"length":3}}',
    '{"type":"addGPlane", "payload":{"width":3,"height":2}}',
    '{"type":"addGSphere", "payload":{"radius":3}}',
    '{"type":"addGBlock", "payload":{"width":1,"height":2,"depth":3}}'
  ];
 
  function ActionsPanel() {
    return (
      <>
        <form action={dispatchAction}>
          <input name="actionInput" type="text" className="input" placeholder="Next action?" list="suggestions" />
          <datalist id="suggestions">
            {actionSuggestions.map((suggestion, index) => (
              <option key={index} value={suggestion}></option>
            ))}
          </datalist>
        </form>   
      </>
    )
  }

  function GraphicsPanel() {
    return (
      <>
        <Canvas>
          <ambientLight color={0x505050} />
          <directionalLight intensity={0.5} position={[0.75, 0.25, 0.25]} />
          <directionalLight intensity={0.5} position={[-0.75, 0.25, 0.25]} />
          <Drawable args={{geometry: {type: 'box', width: 3, height: 2, depth: 1}}} />
          <OrbitControls />
        </Canvas>
      </>
    )
  }

  function GraphicsPanelOld() {
    return (
      <>
        <Canvas>
          <ambientLight color={0x505050} />
          <directionalLight intensity={0.5} position={[0.75, 0.25, 0.25]} />
          <directionalLight intensity={0.5} position={[-0.75, 0.25, 0.25]} />
          <mesh>
            <boxGeometry args={[3, 2, 1]}/>
            <meshStandardMaterial />
          </mesh>
          <OrbitControls />
        </Canvas>
      </>
    )
  }

  function ToolbarPanel() {
      return (
        <>
          <p> Toolbar goes here</p>
        </>
      )
    }


  return (
    <>
      <div className="container m-auto grid grid-cols-3 grid-rows-1">
        <div className="container m-auto grid grid-cols-1 grid-rows-2">
          <div><ActionsPanel /></div>
          <div><AcknowledgementsPanel /></div>
        </div>
        <GraphicsPanel />
        <ToolbarPanel /> 
      </div>
    </>
  )
}

export default App
