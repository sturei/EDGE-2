/**
 * This component is a simple panel acknowledging the technologies used in this application, plus a button to demonstrate that React is working.
 * It was derived from the Vite + React + Tailwind + DaisyUI template.
 */

import reactLogo from '../assets/react.svg'
import tailwindLogo from '../assets/tailwindcss-mark.svg'
import daisyuiLogo from '../assets/daisyui-mark-rotating.svg'
import threejsLogo from '../assets/threejs-logo.svg'
import viteLogo from '/vite.svg'
    
export function Acknowledgements() {

    return (
      <>
        <div className="container m-auto grid grid-cols-1">
          <div>
            <p className="text-gray-500 text-center">
              Acknowledgements:
            </p>
          </div>
          <div className="container grid grid-cols-2 px-8">
            <a className="join-item" href="https://vite.dev" target="_blank">
              <div className="tooltip" data-tip="Vite">
                <img src={viteLogo} className="logo" alt="Vite logo" />
              </div>
            </a>
            <a href="https://react.dev" target="_blank">
              <div className="tooltip" data-tip="React">
                <img src={reactLogo} className="logo" alt="React logo" />
              </div>
            </a>
            <a href="https://tailwindcss.com" target="_blank">
              <div className="tooltip" data-tip="Tailwind">
                <img src={tailwindLogo} className="logo" alt="Tailwind CSS logo" />
              </div>
            </a>
            <a href="https://daisyui.com" target="_blank">
              <div className="tooltip" data-tip="daisyUI">
                <img src={daisyuiLogo} className="logo" alt="daisyUI logo" />
              </div>
            </a>
            <a href="https://threejs.org/" target="_blank">
              <div className="tooltip" data-tip="Three.js">
                <img src={threejsLogo} className="logo" alt="three.js logo" />
              </div>
            </a>
          </div>
        </div>
      </>
    )
  }
