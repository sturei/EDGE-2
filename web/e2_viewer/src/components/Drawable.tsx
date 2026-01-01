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

function Point({args, children} : {args: [[number, number, number]], children: JSX.Element}) {
    // todo:use a point sprite so that size can be controlled in webgpu implmentation
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

function Line({args, position, rotation,children} : {args: [[number, number, number], [number, number, number]], position:[number, number, number], rotation:[number, number, number], children: JSX.Element}) {
    const [start, end] = args;
    // Note: drei's Line component generates an error when rendering to the mock dom that I use in the unit tests. Hence this custom Line component.
    const positions = new Float32Array([...start, ...end]);
    const lineGeometry = new LineGeometry();
    lineGeometry.setPositions(positions)
    const line2 = new Line2(lineGeometry);    
    return (
        <primitive object={line2} position={position} rotation={rotation}>
            {children}
        </primitive>
    )
}

function Polyline({args, position, rotation, children} : {args: [Array<[number, number, number]>], position:[number, number, number], rotation:[number, number, number], children: JSX.Element}) {
    const [positionsArray] = args;
    const positions = new Float32Array(positionsArray.flat());
    const lineGeometry = new LineGeometry();
    lineGeometry.setPositions(positions)
    const line2 = new Line2(lineGeometry);    
    return (
        <primitive object={line2} position={position} rotation={rotation}>
            {children}
        </primitive>
    )
}

function Profile({args, position, rotation, children} : {args: [Array<Array<[number, number]>>], position:[number, number, number], rotation:[number, number, number], children: JSX.Element}) {
    const [paths] = args;
    const shape = new THREE.Shape();
    for (const path of paths) {
        shape.moveTo(path[0][0], path[0][1]);
        for (let i = 1; i < path.length; i++) {
            shape.lineTo(path[i][0], path[i][1]);
        }
    }
    return (
        <mesh position={position} rotation={rotation}>
            <shapeGeometry args={[shape]} />
            {children}
        </mesh> 
    )
}

function Contour({args, position, rotation, children} : {args: [Array<Array<[number, number]>>], position:[number, number, number], rotation:[number, number, number], children: JSX.Element}) {
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
                <Polyline key={index} args={[pline]} position={position} rotation={rotation}>
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

    const position = geometry?.position??[0,0,0];
    const rotation = geometry?.rotation??[0,0,0];

    // uses drei utilities where available - otherwise uses threejs directly
    if (!geometry) {
        console.warn("jsxFromDrawable: no geometry specified in drawable:", drawable);
    }   
    else if (geometry.type === 'block') {
        console.log(`Creating block with dimensions width = ${geometry.width}, height = ${geometry.height}, depth = ${geometry.depth}`);
        return (
            <Box args={[geometry.width, geometry.height, geometry.depth]} position={position} rotation={rotation}>
                <meshStandardMaterial color={appearance?.color??Color.get("red")} />
            </Box>
        );
    }
    else if (geometry.type === 'plane') {
        console.log(`Creating plane with dimensions width = ${geometry.width}, height = ${geometry.height}`);
        if (appearance?.type == 'mesh' && appearance?.texture) {
            const {width, height, data} = appearance.texture;
            console.log(`    and a ${width} x ${height} texture`);
            const texture = new THREE.DataTexture( new Uint8Array(data), width, height );
            texture.needsUpdate = true;
            //console.log("data texture created: " + data);          //--- DEBUG ---
            return (
                <Plane args={[geometry.width, geometry.height]} position={position} rotation={rotation}>
                    <meshBasicMaterial map={texture} transparent={true} opacity={1.0} side={THREE.DoubleSide} />
                </Plane>
            );
        }
        else {
            return (
                <Plane args={[geometry.width, geometry.height]} position={position} rotation={rotation}>
                    <meshStandardMaterial color={appearance?.color??Color.get("blue")} side={THREE.DoubleSide} />
                </Plane>
            );
        }
    }
    else if (geometry.type === 'sphere') {
        console.log(`Creating sphere with radius: ${geometry.radius}`);
        return (
            <Sphere args={[geometry.radius, 32, 32]} position={position} rotation={rotation}>
                <meshStandardMaterial color={appearance?.color??Color.get("green")} />
            </Sphere>
        );
    }
    else if (geometry.type === 'cylinder') {
        console.log(`Creating cylinder with radius: ${geometry.radius}, depth: ${geometry.depth}`);
        const threeRotation = new THREE.Euler(rotation[0]+Math.PI/2, rotation[1], rotation[2]);    // align cylinder axis with Z by default
        return (
            <Cylinder args={[geometry.radius, geometry.radius, geometry.depth, 32]} position={position} rotation={threeRotation}>
                <meshStandardMaterial color={appearance?.color??Color.get("yellow")} />
            </Cylinder>
        );
    }
    else if (geometry.type === 'line') {
        console.log(`Creating line from ${geometry.start} to ${geometry.end}`);
        return (
            <Line args={[geometry.start, geometry.end]} position={position} rotation={rotation}>
                <LineAppearance color={appearance?.color??Color.get("blue")} />
            </Line>
        );
    }        
    else if (geometry.type === 'polyline') {
        console.log(`Creating polyline with ${geometry.positions.length} positions`);
        return (
            <Polyline args={[geometry.positions]} position={position} rotation={rotation}>
                <LineAppearance color={appearance?.color??Color.get("blue")} />
            </Polyline>
        );
    }        
    else if (geometry.type === 'point') {
        console.log(`Creating point at position: ${geometry.position}`);
        // Note: size is not honoured in webgpu - points are always 1 pixel. The recommended approach is to use point sprites instead.
        const size = appearance?.type === 'point' ? appearance.size ?? 4 : 4;
        return (
            <Point args={[position]}>
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
            <Profile args={[geometry.paths]} position={position} rotation={rotation}>
                <meshStandardMaterial color={appearance?.color??Color.get("white")} side={THREE.DoubleSide} />
            </Profile>
        )
    }
    else if (geometry.type === 'contour'){
        console.log(`Creating contour with ${geometry.paths.length} paths`);    // --- DEBUG ---
        for (let p = 0; p < geometry.paths.length; p++) {
            console.log(`  Path ${p} with ${geometry.paths[p].length} points`);   // --- DEBUG ---
        }
        console.log(`  position2D: ${geometry.position2D}`);   // --- DEBUG ---
        console.log(`  rotation2D: ${geometry.rotation2D}`);   // --- DEBUG ---
        console.log(`  zOffset: ${geometry.zOffset}`);   // --- DEBUG ---

        // Apply 2D position, rotation and z-offset
        const position2D:[number, number, number] = [geometry.position2D[0], geometry.position2D[1], geometry.zOffset];
        const rotation2D:[number, number, number] = [0, 0, geometry.rotation2D];

        return(
            <group position={position} rotation={rotation}>
                <Contour args={[geometry.paths]} position={position2D} rotation={rotation2D}>
                    <LineAppearance color={appearance?.color??Color.get("blue")} />
                </Contour>
            </group>
        )
    }
    else {
        console.warn("jsxFromDrawable: unknown geometry type in drawable:", drawable);
    }

}

export function Drawable({drawable}: {drawable: IDrawable}) {
    return jsxFromDrawable(drawable);
}
