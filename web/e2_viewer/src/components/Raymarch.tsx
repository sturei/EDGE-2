import { useThree } from '@react-three/fiber'
import { MeshBasicNodeMaterial } from 'three/webgpu' 
import * as THREE from 'three'
import { useFrame } from '@react-three/fiber'
import { useEffect } from 'react'
//import { sdfScene } from './nodes/sdfScene.ts';


import {    
    float,
    Fn,
  uniform,
  Loop,
    If,
    Break,
  length,
  vec3,
  vec2,
    dot,
    reflect,
    mix,
    min,
    max,
    abs,
    oscSine,
    normalize,
    positionLocal,
    time
} from 'three/tsl'

import type { IShaderNode } from '../grep/shaderNode.ts'
import type { IShader } from 'three-stdlib'
import { sdfScene } from './nodes/sdfScene.ts'

// use this when rendering to a plane instead of the background
//const raymarchMaterial = new MeshBasicNodeMaterial()

const cameraPosition = uniform(new THREE.Vector3())
const cameraTarget = uniform(new THREE.Vector3())

// from https://tympanus.net/codrops/2024/07/15/how-to-create-a-liquid-raymarching-scene-using-three-js-shading-language/
const calcNormal = Fn(([p, nodes]: [any, IShaderNode[]]) => {
  const eps = float(0.0001)
  const h = vec2(eps, 0)
  return normalize(
    vec3(
      sdfSceneTest(p.add(h.xyy), nodes).sub(sdfSceneTest(p.sub(h.xyy), nodes)),
      sdfSceneTest(p.add(h.yxy), nodes).sub(sdfSceneTest(p.sub(h.yxy), nodes)),
      sdfSceneTest(p.add(h.yyx), nodes).sub(sdfSceneTest(p.sub(h.yyx), nodes)),
    ),
  )
})

/* a fancier one from Inigo Quilez
vec3 calcNormal( in vec3 ) // for function f(p)
{
    const float h = 0.0001; // replace by an appropriate value
    const vec2 k = vec2(1,-1);
    return normalize( k.xyy*f( p + k.xyy*h ) + 
                      k.yyx*f( p + k.yyx*h ) + 
                      k.yxy*f( p + k.yxy*h ) + 
                      k.xxx*f( p + k.xxx*h ) );
}
*/

/* from https://tympanus.net/codrops/2024/07/15/how-to-create-a-liquid-raymarching-scene-using-three-js-shading-language/ 
*/
const lighting = Fn(([ro, r, nodes] : [any, any, IShaderNode[]]) => {
  const normal = calcNormal(r, nodes)
  const rayVec = r.sub(ro)
  const viewDir = normalize(rayVec)
  const t = length(rayVec);

  If(t.greaterThan(500), () => {
    return vec3(0,0,0)
  })

  // Step 1: Ambient light
  const ambient = vec3(0.2)

  // Step 2: Diffuse lighting - gives our shape a 3D look by simulating how light reflects in all directions
  const lightDir = normalize(vec3(1, 1, 1))
  const lightColor = vec3(1, 1, 0.9)
  const dp = max(0, dot(lightDir, normal))

  const diffuse = dp.mul(lightColor)

  // Steo 3: Hemisphere light - a mix between a sky and ground colour based on normals
  const skyColor = vec3(0, 0.3, 0.6)
  const groundColor = vec3(0.6, 0.3, 0.1)

  const hemiMix = normal.y.mul(0.5).add(0.5)
  const hemi = mix(groundColor, skyColor, hemiMix)

  // Step 4: Phong specular - Reflective light and highlights
  const ph = normalize(reflect(lightDir.negate(), normal))
  const phongValue = max(0, dot(viewDir, ph)).pow(32)

  const specular = vec3(phongValue).toVar()

  // Step 5: Fresnel effect - makes our specular highlight more pronounced at different viewing angles
  const fresnel = float(1)
    .sub(max(0, dot(viewDir, normal)))
    .pow(2)

  specular.mulAssign(fresnel)

  // Lighting is a mix of ambient, hemi, diffuse, then specular added at the end
  // We're multiplying these all by different values to control their intensity

  // Step 1
  const lighting = ambient.mul(0.1)

  // Step 2
  lighting.addAssign(diffuse.mul(0.5))

  // Step 3
  lighting.addAssign(hemi.mul(0.2))

  const finalColor = vec3(0.1, 0.2, 0.3).mul(lighting).toVar()

  // Step 4 & 5
  finalColor.addAssign(specular)

  return finalColor
})

const Sphere = Fn(([position, radius]: [any, any]) => {
  return length(position).sub(radius)
})

const Box = Fn(([position, dimensions] : [any, any]) => {
        const distance = abs(position).sub(dimensions)
        //return length(max(distance, 0.0))  // better if you don't need internal calculations. E.g, intersect or subtraction
        return length(max(distance, 0.0)).add(
          min(max(distance.x, max(distance.y, distance.z)), 0.0)
        )
})

const SmoothMinimum = Fn(([a, b, k]: [any, any, any]) => {
  const h = max(k.sub(abs(a.sub(b))), 0).div(k)
  return min(a, b).sub(h.mul(h).mul(k).mul(0.25))
})


const sdfSceneTest = Fn(([position, _nodes_]: [any, IShaderNode[]]) => {
    const t = time.mul(0.2)

    const sphere = Sphere(position.sub(vec3(0, 0, 0)), 1)
    const translatedPos = position.add(vec3(oscSine(t).mul(4).sub(2), 0, 0)); // oscSine returns [0,1]
    const movingSphere = Sphere(translatedPos.sub(vec3(0, 1, 0)), 0.5)
    const box = Box(position.sub(vec3(0, 0, 0)), vec3(1,2,3).mul(0.5))
    const distance = SmoothMinimum(sphere, movingSphere, 0.5);
    distance.assign(min(distance, box));

    return distance
})


const raymarch = Fn(() => {

  // Use a static scene for now. Later we will pass this in as prop.
  const nodes : IShaderNode[] = [
    {
      type: 'sphere',
      pathName: 'shape0',
      parameters: new Map([['radius', 1.0]]),
      childIndices: []
    }
  ];

  // Initialize the ray and its direction
  const p = positionLocal;   // I assume positionLocal, on the background node, is in view space
  const rayOrigin = cameraPosition;
  const rayDirection = normalize(p);

  // Total distance travelled
  const t = float(0).toVar()

  // Calculate the initial position of the ray - this var is declared here so we can use it in lighting calculations later
  const ray = rayOrigin.add(rayDirection.mul(t)).toVar()

  Loop({ start: 1, end: 80 }, () => {
    const d = sdfSceneTest(ray, nodes) // current distance to the scene

    t.addAssign(d) // "march" the ray

    ray.assign(rayOrigin.add(rayDirection.mul(t))) // position along the ray

    // If we're close enough, it's a hit, so we can do an early return
    If(d.lessThan(0.001), () => {
      Break()
    })

    // If we've travelled too far, we can return now and consider that this ray didn't hit anything
    If(t.greaterThan(100), () => {
      Break()
    })
  })

  // add lighting
  return lighting(rayOrigin, ray, nodes)

})()

function generateRaymarch() {
    const nodes : IShaderNode[] = [
        {
        type: 'sphere',
        pathName: 'shape0',
        parameters: new Map([['radius', 1.0]]),
        childIndices: []
        }];

    const sceneNode = sdfSceneTest;     // scene generator
    const raymarchInner = Fn(() => {

        // Initialize the ray and its direction
        const p = positionLocal;   // I assume positionLocal, on the background node, is in view space
        const rayOrigin = cameraPosition;
        const rayDirection = normalize(p);

        // Total distance travelled
        const t = float(0).toVar()

        // Calculate the initial position of the ray - this var is declared here so we can use it in lighting calculations later
        const ray = rayOrigin.add(rayDirection.mul(t)).toVar()

        Loop({ start: 1, end: 80 }, () => {
//            const d = sdfSceneTest(ray, nodes) // current distance to the scene
            const d = sceneNode(ray, nodes) // current distance to the scene

            t.addAssign(d) // "march" the ray

            ray.assign(rayOrigin.add(rayDirection.mul(t))) // position along the ray

            // If we're close enough, it's a hit, so we can do an early return
            If(d.lessThan(0.001), () => {
            Break()
            })

            // If we've travelled too far, we can return now and consider that this ray didn't hit anything
            If(t.greaterThan(100), () => {
            Break()
            })
        })

        // add lighting
        return lighting(rayOrigin, ray, nodes)

    })()

    return raymarchInner;    
}

// use this when rendering to a plane
//raymarchMaterial.colorNode = raymarch

export function Raymarch() {
  const camera = useThree((state) => state.camera)
  const scene = useThree((state) => state.scene)    

// extract the camera position and target to pass to the shader
useFrame(() => {
    cameraPosition.value.copy(camera.position)
    cameraTarget.value.copy(new THREE.Vector3(0,0,0)) // for now, looking at origin
})

useEffect(() => {
    console.log("Generating sdf shaders for background");

    scene.background = new THREE.Color(0xabcdef); // Set background color
    //scene.backgroundNode = generateRaymarch();                 // Set the background to use our raymarch shader
    scene.backgroundNode = raymarch;                 // Set the background to use our raymarch shader
    return () => {
        scene.background = null; // Clean up on unmount
        scene.backgroundNode = null;
    };
  }, [scene]);

  return null;

  /** use this to draw on a plane instead of the background */
  /*
  return (
    <mesh scale={[width, height, 1]}>
      <planeGeometry args={[1, 1]} />
      <primitive object={raymarchMaterial} attach='material' />
    </mesh>
  )
    */
}