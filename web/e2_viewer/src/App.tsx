import { useState } from 'react'
import reactLogo from './assets/react.svg'
import tailwindLogo from './assets/tailwindcss-mark.svg'
import daisyuiLogo from './assets/daisyui-mark-rotating.svg'
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
              <div className="tooltip" data-tip="DaisyUI - CSS component library">
              <img src={daisyuiLogo} className="logo" alt="DaisyUI logo" />
              </div>
            </a>
          </div>
          <p className="read-the-docs">
            This application uses Vite + React + Tailwind + DaisyUI.
          </p>
          <p className="read-the-docs">
            Click on the logos to learn more
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
        <p> Graphics go here</p>
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
