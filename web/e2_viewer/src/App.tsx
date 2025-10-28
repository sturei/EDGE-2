import { useContext } from 'react'
import { Canvas } from '@react-three/fiber'
import { Acknowledgements } from './components/Acknowledgements'
import { Actions } from './components/Actions.tsx'
import { Scene } from './components/Scene.tsx';
import { DocumentContext } from './Contexts.ts';
import './App.css'

function App() {
  const document = useContext(DocumentContext);

  function ToolbarPlaceholder() {
      return (
        <>
          <p> Toolbar goes here</p>
        </>
      )
    }

  return (
    <>
    <DocumentContext value={document}>
      <div className="flex flex-row">
        <div className="left-sidebar flex-none bg-blue-100">
          <div className="grid grid-cols-1 grid-rows-2">
            <div><Actions /></div>
            <div><Acknowledgements /></div>
          </div>
        </div>
        <div className="graphicsArea flex-1 bg-gray-200">
          <Canvas>
            <Scene/>
          </Canvas>
        </div>
        <div className="right-sidebar flex-none bg-blue-100">
          <ToolbarPlaceholder />
        </div> 
      </div>      
    </DocumentContext>
    </>
  )
}

export default App
