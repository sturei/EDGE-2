/** This component creates a 3D object in the scene based on the provided properties. 
*/
// Implementation note: as written, it's not efficient from a React point of view. It will create new geometry on every react-three-fiber render.
// Later will come the usual use-memo, use-ref etc.

import  { type IPointGeometry, type IProfileGeometry, type IDrawable, Color } from '../grep/drawable';
import {Box, Sphere, Plane} from '@react-three/drei'  
import { Line2, LineGeometry, LineMaterial } from 'three-stdlib'
import * as THREE from 'three';
import type { JSX } from 'react';

//const identity = [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1];

function Shape(points: number[]) {
    const shape = new THREE.Shape();
    shape.moveTo( points[0], points[1] );
    for ( let i = 2; i < points.length; i += 2 ) {
        shape.lineTo( points[i], points[i + 1] );
    }
    return shape;
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
    // I would use the drei's Line component instead of this home-grown one, but drei's Line generates an error when rendering 
    // to the mock dom that I use in the unit tests.
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

function LineAppearance({color} : {color: number}) {
    // LineMaterial is not part of the main package. The easiest way to use it in r3f seems to be to wrap it like this.
    // If in future I can use drei Line, then this can be removed.
    const lineMaterial = new LineMaterial( { color } );
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
    else if (geometry.type === 'box') {
        console.log(`Creating box with dimensions width = ${geometry.width}, height = ${geometry.height}, depth = ${geometry.depth}`);
        return (
            <Box args={[geometry.width, geometry.height, geometry.depth]}>
                <meshStandardMaterial color={appearance?.color??Color.Red} />
            </Box>
        );
    }
    else if (geometry.type === 'plane') {
        console.log(`Creating plane with dimensions width = ${geometry.width}, height = ${geometry.height}`);
        return (
            <Plane args={[geometry.width, geometry.height]}>
                <meshStandardMaterial color={appearance?.color??Color.Blue} side={THREE.DoubleSide} />
            </Plane>
        );
    }
    else if (geometry.type === 'sphere') {
        console.log(`Creating sphere with radius: ${geometry.radius}`);
        return (
            <Sphere args={[geometry.radius, 32, 32]}>
                <meshStandardMaterial color={appearance?.color??Color.Green} />
            </Sphere>
        );
    }
    else if (geometry.type === 'line') {
        console.log(`Creating line from ${geometry.start} to ${geometry.end}`);
        return (
            <Line args={[geometry.start, geometry.end]}>
                <LineAppearance color={appearance?.color??Color.Blue} />
            </Line>
        );
    }        
    else if (geometry.type === 'point') {
        console.log(`Creating point at position: ${geometry.position}`);
        const size = appearance?.type === 'point' ? appearance.size ?? 4 : 4;
        return (
            <Point args={[geometry.position]}>
                <pointsMaterial color={appearance?.color ?? Color.Green} size={size} sizeAttenuation={false} />
            </Point>
        );
    }
    else if (geometry.type === 'shape'){
        console.log(`Creating shape with points: ${geometry.points}`);        //--- DEBUG ---
        return(
            <mesh>
                <shapeGeometry args={[Shape(geometry.points)]} />
                <meshStandardMaterial color={appearance?.color??Color.White} side={THREE.DoubleSide} />
            </mesh> 
        )
    }
    else if (geometry.type === 'profile'){
        console.log(`Creating profile with ${geometry.paths.length} paths`);    // --- DEBUG ---
        return(
            <Profile args={[geometry.paths]}>
                <meshStandardMaterial color={appearance?.color??Color.White} side={THREE.DoubleSide} />
            </Profile>
        )
    }
}

export function Drawable({drawable}: {drawable: IDrawable}) {
    return jsxFromDrawable(drawable);
}