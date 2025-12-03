import { useContext } from 'react'
import { Canvas } from '@react-three/fiber'
import { Acknowledgements } from './components/Acknowledgements'
import { Actions } from './components/Actions'
import { Scene } from './components/Scene';
import { Structure } from './components/Structure';
import { DocumentContext } from './Contexts';

// At present, React Three Fiber does not include types for Three.js WebGPU extensions, hence this manual extension
import { extend,type ThreeToJSXElements } from '@react-three/fiber';
declare module "@react-three/fiber" {
  interface ThreeElements extends ThreeToJSXElements<typeof THREE> {}
}
import * as THREE from "three/webgpu";
extend(THREE as any)
import { type WebGPURendererParameters } from "three/src/renderers/webgpu/WebGPURenderer.js";

import './App.css';

function App() {
  const document = useContext(DocumentContext);

  function ToolbarPlaceholder() {
    return (
      <>
        <p className="text-gray-500 italic text-sm text-center"> Toolbar goes here</p>
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
          <div className="flex flex-col justify-between h-full">
            <div>
              <Structure />
            </div>
            <div>
              <Acknowledgements />
            </div>
          </div>
        </div>
        <div className="graphicsArea h-140 flex-1 bg-slate-800">
          <Canvas
            gl={async (props) => {
              console.warn("WebGPU is supported");
              const renderer = new THREE.WebGPURenderer(
              props as WebGPURendererParameters
            );
            await renderer.init();
            return renderer;
          }}
          >           
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
