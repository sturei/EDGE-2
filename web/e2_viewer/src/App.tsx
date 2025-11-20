import { useContext } from 'react'
import { Canvas } from '@react-three/fiber'
import { Acknowledgements } from './components/Acknowledgements'
import { Actions } from './components/Actions.tsx'
import { Scene } from './components/Scene.tsx';
import { DocumentContext } from './Contexts.ts';
import './App.css';

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
    <div className="w-250">
    <DocumentContext value={document}>
      <div className="header bg-blue-300 p-2 w-full">
        <div><Actions /></div>
      </div>
      <div className="flex flex-row">
        <div className="left-sidebar w-15rem flex-none bg-blue-100">
          <div className="grid grid-cols-1 grid-rows-2">
            <div></div>
            <div className="align-bottom">
              <Acknowledgements />
            </div>
          </div>
        </div>
        <div className="graphicsArea h-140 flex-1 bg-gray-200">
          <Canvas>
            <Scene/>
          </Canvas>
        </div>
        <div className="right-sidebar w-10remflex-none bg-blue-100">
          <ToolbarPlaceholder />
        </div> 
      </div>      
    </DocumentContext>
    </div>
  )
}

export default App
