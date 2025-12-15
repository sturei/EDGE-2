/** This component creates a 3D object in the scene based on the provided properties. 
*/
// Implementation note: as written, it's not efficient from a React point of view. It will create new geometry on every react-three-fiber render.
// Later will come the usual use-memo, use-ref etc.

import  { type IDrawable, Color } from '../grep/drawables/drawable';
import { Box, Sphere, Cylinder, Plane } from '@react-three/drei'  
import { LineGeometry } from 'three/addons/lines/LineGeometry.js'
import { Line2NodeMaterial } from 'three/webgpu';
import { Line2 } from 'three/addons/lines/webgpu/Line2.js';
import * as THREE from 'three/webgpu';
import type { JSX } from 'react';


//const identity = [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1];

function Point({args, children} : {args: [[number, number, number]], children: JSX.Element}) {
    const [position] = args;
    const positions = new Float32Array(position);
    return (
        <points> 
            <bufferGeometry>
                <bufferAttribute attach={'attributes-position'} args={[positions, 3]} />
            </bufferGeometry>
            {children}  
        </points>
    )
}

function Line({args, children} : {args: [[number, number, number], [number, number, number]], children: JSX.Element}) {
    const [start, end] = args;
    // Note: drei's Line component generates an error when rendering to the mock dom that I use in the unit tests. Hence this custom Line component.
    const positions = new Float32Array([...start, ...end]);
    const lineGeometry = new LineGeometry();
    lineGeometry.setPositions(positions)
    const line2 = new Line2(lineGeometry);    
    return (
        <primitive object={line2} >
            {children}
        </primitive>
    )
}

function Polyline({args, children} : {args: [Array<[number, number, number]>], children: JSX.Element}) {
    const [positionsArray] = args;
    const positions = new Float32Array(positionsArray.flat());
    const lineGeometry = new LineGeometry();
    lineGeometry.setPositions(positions)
    const line2 = new Line2(lineGeometry);    
    return (
        <primitive object={line2} >
            {children}
        </primitive>
    )
}

function Profile({args, children} : {args: [Array<Array<[number, number]>>], children: JSX.Element}) {
    const [paths] = args;
    const shape = new THREE.Shape();
    for (const path of paths) {
        shape.moveTo(path[0][0], path[0][1]);
        for (let i = 1; i < path.length; i++) {
            shape.lineTo(path[i][0], path[i][1]);
        }
    }
    return (
        <mesh>
            <shapeGeometry args={[shape]} />
            {children}
        </mesh> 
    )
}

function Contour({args, children} : {args: [Array<Array<[number, number]>>], children: JSX.Element}) {
    const [paths] = args;
    const polylines = new Array<Array<[number, number, number]>>();
    for (const path of paths) {
        const polyline = new Array<[number, number, number]>();
        for (let i = 0; i < path.length; i++) {
            polyline.push([path[i][0], path[i][1], 0]);
        }
        polylines.push(polyline);
    }
    return (
        <group>
            {polylines.map( (pline, index) => (
                <Polyline key={index} args={[pline]}>
                    {children}
                </Polyline>
            ))}
        </group>
    )
}

function LineAppearance({color} : {color?: number}) {
    const lineMaterial = new Line2NodeMaterial( { color } );
    return (
        <primitive object={lineMaterial} />
    )
}

/** Outputs the required react-three-fiber jsx for the specified drawable. (Exported for testing purposes) */
export function jsxFromDrawable(drawable: IDrawable) {
    const geometry = drawable.geometry;
    const appearance = drawable.appearance;

    // uses drei utilities where available - otherwise uses threejs directly
    if (!geometry) {
        console.warn("jsxFromDrawable: no geometry specified in drawable:", drawable);
    }   
    else if (geometry.type === 'block') {
        console.log(`Creating block with dimensions width = ${geometry.width}, height = ${geometry.height}, depth = ${geometry.depth}`);
        return (
            <Box args={[geometry.width, geometry.height, geometry.depth]}>
                <meshStandardMaterial color={appearance?.color??Color.get("red")} />
            </Box>
        );
    }
    else if (geometry.type === 'plane') {
        console.log(`Creating plane with dimensions width = ${geometry.width}, height = ${geometry.height}, z=${geometry.z}`);
        if (appearance?.type == 'mesh' && appearance?.texture) {
            const {width, height, data} = appearance.texture;
            console.log(`    and a ${width} x ${height} texture`);
            const texture = new THREE.DataTexture( new Uint8Array(data), width, height );
            texture.needsUpdate = true;
            //console.log("data texture created: " + data);          //--- DEBUG ---
            return (
                <Plane args={[geometry.width, geometry.height]} position={[0,0,geometry.z]}>
                    <meshBasicMaterial map={texture} transparent={true} opacity={1.0} side={THREE.DoubleSide} />
                </Plane>
            );
        }
        else {
            return (
                <Plane args={[geometry.width, geometry.height]} position={[0,0,geometry.z]}>
                    <meshStandardMaterial color={appearance?.color??Color.get("blue")} side={THREE.DoubleSide} />
                </Plane>
            );
        }
    }
    else if (geometry.type === 'sphere') {
        console.log(`Creating sphere with radius: ${geometry.radius}`);
        return (
            <Sphere args={[geometry.radius, 32, 32]}>
                <meshStandardMaterial color={appearance?.color??Color.get("green")} />
            </Sphere>
        );
    }
    else if (geometry.type === 'cylinder') {
        console.log(`Creating cylinder with radius: ${geometry.radius}, height: ${geometry.depth}`);
        return (
            <Cylinder args={[geometry.radius, geometry.radius, geometry.depth, 32]} rotation={[Math.PI/2,0,0]}>
                <meshStandardMaterial color={appearance?.color??Color.get("yellow")} />
            </Cylinder>
        );
    }
    else if (geometry.type === 'line') {
        console.log(`Creating line from ${geometry.start} to ${geometry.end}`);
        return (
            <Line args={[geometry.start, geometry.end]}>
                <LineAppearance color={appearance?.color??Color.get("blue")} />
            </Line>
        );
    }        
    else if (geometry.type === 'polyline') {
        console.log(`Creating polyline with ${geometry.positions.length} positions`);
        return (
            <Polyline args={[geometry.positions]}>
                <LineAppearance color={appearance?.color??Color.get("Blue")} />
            </Polyline>
        );
    }        
    else if (geometry.type === 'point') {
        console.log(`Creating point at position: ${geometry.position}`);
        // Note: size is not honoured in webgpu - points are always 1 pixel. The recommended approach is to use point sprites instead.
        const size = appearance?.type === 'point' ? appearance.size ?? 4 : 4;
        return (
            <Point args={[geometry.position]}>
                <pointsMaterial color={appearance?.color ?? Color.get("green")} size={size} sizeAttenuation={false} />
            </Point>
        );
    }
    else if (geometry.type === 'profile'){
        console.log(`Creating profile with ${geometry.paths.length} paths`);    // --- DEBUG ---
        for (let p = 0; p < geometry.paths.length; p++) {
            console.log(`  Path ${p} with ${geometry.paths[p].length} points`);   // --- DEBUG ---
        }
        return(
            <Profile args={[geometry.paths]}>
                <meshStandardMaterial color={appearance?.color??Color.get("white")} side={THREE.DoubleSide} />
            </Profile>
        )
    }
    else if (geometry.type === 'contour'){
        console.log(`Creating contour with ${geometry.paths.length} paths`);    // --- DEBUG ---
        for (let p = 0; p < geometry.paths.length; p++) {
            console.log(`  Path ${p} with ${geometry.paths[p].length} points`);   // --- DEBUG ---
        }
        return(
            <Contour args={[geometry.paths]}>
                <LineAppearance color={appearance?.color??Color.get("Blue")} />
            </Contour>
        )
    }
    else {
        console.warn("jsxFromDrawable: unknown geometry type in drawable:", drawable);
    }

}

export function Drawable({drawable}: {drawable: IDrawable}) {
    return jsxFromDrawable(drawable);
}
