/** This component creates a 3D object in the scene, based on the provided properties. */

// Implementation note: as written, it will create new geometry even if the geometry is unchanged and all that has changed is the appearance or the matrix.
// Also it does not reuse geometries between different drawables with the same geometry.
// To avoid this it's probably possible to create a geometry cache keyed on name or id. Perhaps provide it as a Context.

import  { type IDrawable, Color } from '../grep/drawable';
import {Box, Line, Sphere, Plane} from '@react-three/drei'  
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
export function r3fFromDrawable(drawable: IDrawable) {
    const geometry = drawable.geometry;
    const appearance = drawable.appearance;
    console.log("r3fFromDrawable: geometry:", geometry);

    // uses drei utilities where available - otherwise uses threejs

    // TODO: use-memo, use-ref etc to avoid recreating the underlying geometries unnecessarily (see drei docs)

    if (!geometry) {
        console.warn("r3fFromDrawable: no geometry specified in drawable:", drawable);
    }
    else if (geometry.type === 'box') {
        return (
            <Box args={[geometry.width, geometry.height, geometry.depth]}>
                <meshStandardMaterial color={appearance?.color??Color.Red} />
            </Box>
        );
    }
    else if (geometry.type === 'plane') {
        return (
            <Plane args={[geometry.width, geometry.height]}>
                <meshStandardMaterial color={appearance?.color??Color.Blue} side={THREE.DoubleSide} />
            </Plane>
        );
    }
    else if (geometry.type === 'sphere') {
        console.log("Creating sphere with radius:", geometry.radius);
        return (
            <Sphere args={[geometry.radius, 32, 32]}>
                <meshStandardMaterial color={appearance?.color??Color.Green} />
            </Sphere>
        );
    }
    else if (geometry.type === 'line') {
        return (
            <Line
                points={[geometry.start, geometry.end]} // Array of points
                color={appearance?.color??Color.Blue} // Line color
            />
        );
    }
    else if (geometry.type === 'point') {
        console.log("Creating point at position:", geometry.position);
        const positions = new Float32Array(geometry.position);
        let size = 4;
        let color = Color.Red;
        if (appearance !== undefined && appearance.type === 'point') {
            console.log("    with appearance:", appearance);
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
        return(
            <mesh>
                <shapeGeometry args={[Shape(geometry.points)]} />
                <meshStandardMaterial color={appearance?.color??Color.White} side={THREE.DoubleSide} />
            </mesh> 
        )
    }
}

/** return the jsx that puts the drawable into the scene */
export function Drawable({drawable}: {drawable: IDrawable}) {
    return r3fFromDrawable(drawable);
}