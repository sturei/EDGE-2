// Acknowledgements:
// Much of the shader code is adapted from the excellent tutorials by Inigo Quilez (https://iquilezles.org/) 
// and the article "How to Create a Liquid Raymarching Scene Using Three.js Shading Language" 
// by Maryam Hashemi on Codrops (https://tympanus.net/codrops/2024/07/15/how-to-create-a-liquid-raymarching-scene-using-three-js-shading-language/)
// and the TSL Raymarching tutorial (https://sbcode.net/tsl/raymarching/).
// Many thanks to them for sharing their knowledge!


import { useThree } from '@react-three/fiber'
import * as THREE from 'three/webgpu'
import { useFrame } from '@react-three/fiber'
import { useEffect } from 'react'
import { generateScene } from './nodes/sdfScene.ts';

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
    max,
    normalize,
    positionLocal
} from 'three/tsl'

import type { IShaderNode } from '../grep/shaderNode.ts'

const uCameraPosition = uniform(new THREE.Vector3())

function generateRaymarchedScene(nodes: IShaderNode[]) {

    // The scene node is referenced in the raymarching loop and in the normal calculation
    const sceneNode = generateScene(nodes);

    const calcNormal = Fn(([p]: [any]) => {
        const eps = float(0.0001)
        const h = vec2(eps, 0)
        return normalize(     
            vec3(
                sceneNode(p.add(h.xyy)).sub(sceneNode(p.sub(h.xyy))),
                sceneNode(p.add(h.yxy)).sub(sceneNode(p.sub(h.yxy))),
                sceneNode(p.add(h.yyx)).sub(sceneNode(p.sub(h.yyx))),
            ),
        )
    })

    const lighting = Fn(([ro, r] : [any, any]) => {

        const normal = calcNormal(r) 
        const rayVec = r.sub(ro)
        const viewDir = normalize(rayVec)
        const t = length(rayVec);

        If(t.greaterThan(500), () => {
            return vec3(0,0,0)
        })

        // Step 1: Ambient light
        const ambient = vec3(0.2)

        // Step 2: Diffuse lighting - gives our shape a 3D look by simulating how light reflects in all directions
        const lightDir_1 = normalize(vec3(0.75, 0.25, 0.25))
        const lightDir_2 = normalize(vec3(-0.75, 0.25, 0.25))
        const lightColor = vec3(1, 1, 0.9)
        const dp = max(0, dot(lightDir_1, normal)).add(max(0, dot(lightDir_2, normal))).mul(0.5)

        const diffuse = dp.mul(lightColor)

        // Steo 3: Hemisphere light - a mix between a sky and ground colour based on normals
        const skyColor = vec3(0, 0.3, 0.6)
        const groundColor = vec3(0.6, 0.3, 0.1)

        const hemiMix = normal.y.mul(0.5).add(0.5)
        const hemi = mix(groundColor, skyColor, hemiMix)

        // Step 4: Phong specular - Reflective light and highlights
        const ph = normalize(reflect(lightDir_1.negate(), normal))
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

    const raymarch = Fn(() => {
        // Initialize the ray and its direction
        const p = positionLocal;   // positionLocal, on the background node, is in view space I think.
        const rayOrigin = uCameraPosition;
        const rayDirection = normalize(p);

        // Total distance travelled
        const t = float(0).toVar()

        // Calculate the initial position of the ray
        const ray = rayOrigin.add(rayDirection.mul(t)).toVar()

        Loop({ start: 1, end: 80 }, () => {
            const d = sceneNode(ray) // current distance to the scene
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
        return lighting(rayOrigin, ray)
    })

    return raymarch();    
}

export function RaymarchedScene({nodes, guid}: {nodes: IShaderNode[], guid: string}) {
  const camera = useThree((state) => state.camera)
  const scene = useThree((state) => state.scene)    

    // extract the camera position and target to pass to the shader
    useFrame(() => {
        uCameraPosition.value.copy(camera.position)
    })

    useEffect(() => {
        if (!nodes || nodes.length === 0) {
            return;
        }
        // Set the scene background to the raymarched scene
        console.log("Generating sdf nodes for background");
        scene.background = new THREE.Color(0x000000); // Set background color
        scene.backgroundNode = generateRaymarchedScene(nodes);
        return () => {
            scene.background = null; // Clean up on unmount
            scene.backgroundNode = null;
        };
    }, [scene, guid]);

  return null;
}