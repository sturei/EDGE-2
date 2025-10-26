import { useState } from 'react'
import { Canvas } from '@react-three/fiber'
import { OrbitControls } from '@react-three/drei'
import reactLogo from './assets/react.svg'
import tailwindLogo from './assets/tailwindcss-mark.svg'
import daisyuiLogo from './assets/daisyui-mark-rotating.svg'
import threejsLogo from './assets/threejs-logo.svg'
import viteLogo from '/vite.svg'
import './App.css'

function App() {
  const [count, setCount] = useState(0)

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

  function ActionsPanel() {
    return (
      <>
        <p> Actions go here</p>
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
          <mesh>
            <boxGeometry args={[3, 2, 1]}/>
            <meshStandardMaterial />
          </mesh>
          <OrbitControls />
        </Canvas>
      </>
    )
  }

function CommandsPanel() {
    return (
      <>
        <p> Commands go here</p>
      </>
    )
  }


  return (
    <>
      <div className="container m-auto grid grid-cols-3 grid-rows-1">
        <div className="join-vertical">
          <div className="join-item"><ActionsPanel /></div>
          <div className="join-item"><AcknowledgementsPanel /></div>
        </div>
        <GraphicsPanel />
        <CommandsPanel /> 
      </div>
    </>
  )
}

export default App
