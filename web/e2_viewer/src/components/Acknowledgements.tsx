/**
 * This component is a simple panel acknowledging the technologies used in this application, plus a button to demonstrate that React is working.
 * It was derived from the Vite + React + Tailwind + DaisyUI template.
 */

import reactLogo from '../assets/react.svg'
import tailwindLogo from '../assets/tailwindcss-mark.svg'
import daisyuiLogo from '../assets/daisyui-mark-rotating.svg'
import threejsLogo from '../assets/threejs-logo.svg'
import viteLogo from '/vite.svg'
    
import { useState } from 'react';

export function Acknowledgements() {
  const [count, setCount] = useState(0)

    return (
      <>
        <div className="container m-auto grid grid-cols-1">
          <div className="container m-auto grid grid-cols-3">
            <a className="join-item" href="https://vite.dev" target="_blank">
              <div className="tooltip" data-tip="Vite - Build tooling">
                <img src={viteLogo} className="logo" alt="Vite logo" />
              </div>
            </a>
            <a href="https://react.dev" target="_blank">
              <div className="tooltip" data-tip="React - UI framework">
                <img src={reactLogo} className="logo" alt="React logo" />
              </div>
            </a>
            <a href="https://tailwindcss.com" target="_blank">
              <div className="tooltip" data-tip="Tailwind - CSS framework">
                <img src={tailwindLogo} className="logo" alt="Tailwind CSS logo" />
              </div>
            </a>
            <a href="https://daisyui.com" target="_blank">
              <div className="tooltip" data-tip="daisyUI - CSS component library">
                <img src={daisyuiLogo} className="logo" alt="daisyUI logo" />
              </div>
            </a>
            <a href="https://threejs.org/" target="_blank">
              <div className="tooltip" data-tip="three.js - WebGL library">
                <img src={threejsLogo} className="logo" alt="three.js logo" />
              </div>
            </a>
            <a href="https://github.com/pmndrs/react-three-fiber" target="_blank">
              <div className="tooltip" data-tip="react-three-fiber - React + three.js">
                <img src={threejsLogo} className="logo" alt="three.js logo" />
              </div>
            </a>
            <a href="https://github.com/pmndrs/drei" target="_blank">
              <div className="tooltip" data-tip="drei - R3F helper components">
                <img src={threejsLogo} className="logo" alt="three.js logo" />
              </div>
            </a>
          </div>
          <div>
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
        </div>
      </>
    )
  }
