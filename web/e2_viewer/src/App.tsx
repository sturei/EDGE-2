import { useState } from 'react'
import reactLogo from './assets/react.svg'
import tailwindLogo from './assets/tailwindcss-mark.svg'
import daisyuiLogo from './assets/daisyui-mark-rotating.svg'
import viteLogo from '/vite.svg'
import './App.css'

function App() {
  const [count, setCount] = useState(0)

  return (
    <>
      <div className="join">
        <a className="join-item" href="https://vite.dev" target="_blank">
          <img src={viteLogo} className="logo" alt="Vite logo" />
        </a>
        <a className="join-item" href="https://react.dev" target="_blank">
          <img src={reactLogo} className="logo" alt="React logo" />
        </a>
        <a className="join-item" href="https://tailwindcss.com" target="_blank">
          <img src={tailwindLogo} className="logo" alt="Tailwind CSS logo" />
        </a>
        <a className="join-item" href="https://daisyui.com" target="_blank">
          <img src={daisyuiLogo} className="logo" alt="DaisyUI logo" />
        </a>
      </div>
      <h1>Vite + React + Tailwind + DaisyUI</h1>
      <div className="basis-64">
        <button className="btn"
          onClick={() => setCount((count) => count + 1)}>
          count is {count}
        </button>
        <p>
          Edit <code>src/App.tsx</code> and save to test HMR
        </p>
      </div>
      <p className="read-the-docs">
        Click on the Vite/React/Tailwind/DaisyUI logos to learn more
      </p>
    </>
  )
}

export default App
