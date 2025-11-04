/** This component creates a 3D object in the scene based on the provided properties. 
*/
// Implementation note: as written, it's not efficient from a React point of view. It will create new geometry on every react-three-fiber render.
// Later will come the usual use-memo, use-ref etc.

import  { type IDrawable, Color } from '../grep/drawable';
import {Box, Sphere, Plane} from '@react-three/drei'  
import { Line2, LineGeometry, LineMaterial } from 'three-stdlib'
import * as THREE from 'three';

//const identity = [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1];

function Shape(points: number[]) {
    const shape = new THREE.Shape();
    shape.moveTo( points[0], points[1] );
    for ( let i = 2; i < points.length; i += 2 ) {
        shape.lineTo( points[i], points[i + 1] );
    }
    return shape;
}

/** Outputs the required react-three-fiber jsx for the specified drawable. Exported for testing */
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
        const positions = new Float32Array([
            ...geometry.start,
            ...geometry.end
        ]);
        const lineGeometry = new LineGeometry();
        lineGeometry.setPositions(positions);
        const lineMaterial = new LineMaterial( { color: appearance?.color??Color.Blue } );
        const line2 = new Line2(lineGeometry, lineMaterial);
        
        return (
            <primitive object={line2} >
            </primitive>
        );
    }
    else if (geometry.type === 'point') {
        console.log(`Creating point at position: ${geometry.position}`);
        const positions = new Float32Array(geometry.position);
        let size = 4;
        let color = Color.Red;
        if (appearance !== undefined && appearance.type === 'point') {
            console.log(`    with appearance: ${appearance}`);
            size = appearance.size ?? 4;
            color = appearance.color ?? Color.Red;
        }
        return (
            <points> 
                <bufferGeometry>
                    <bufferAttribute
                        attach={'attributes-position'}
                        args={[positions, 3]} 
                    />
                </bufferGeometry>
                <pointsMaterial
                    size={size}
                    sizeAttenuation={false}
                    color={color}
                />
            </points>
        );
    }
    else if (geometry.type === 'shape'){
        console.log(`Creating shape with points: ${geometry.points}`);
        return(
            <mesh>
                <shapeGeometry args={[Shape(geometry.points)]} />
                <meshStandardMaterial color={appearance?.color??Color.White} side={THREE.DoubleSide} />
            </mesh> 
        )
    }
}

export function Drawable({drawable}: {drawable: IDrawable}) {
    return jsxFromDrawable(drawable);
}