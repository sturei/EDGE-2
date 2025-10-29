/** This component creates a 3D object in the scene, based on the provided properties. */

// Implementation note: as written, it will create new geometry even if the geometry is unchanged and all that has changed is the appearance or the matrix.
// Also it does not reuse geometries between different drawables with the same geometry.
// To avoid this it's probably possible to create a geometry cache keyed on name or id. Perhaps provide it as a Context.

import {Box, Line, Sphere, Plane} from '@react-three/drei'  
import * as THREE from 'three';

/** Appearances */
export interface IStandardAppearance {
    type: 'standard';
    color: number;
}

export type IAnyAppearance = IStandardAppearance;           // other types to be added later, like IPhongAppearance etc.

/** Geometries */
export interface IBoxGeometry {
    type: 'box';
    width: number;
    height: number;
    depth: number;
}

export interface IPlaneGeometry {
    type: 'plane';
    width: number;
    height: number;
}

export interface ISphereGeometry {
    type: 'sphere'; 
    radius: number;
}

export interface ILineGeometry {
    type: 'line';
    points: Array<[number, number, number]>;
}

export interface IPointGeometry {
    type: 'point';
    position: Float32Array;
}

export type IAnyGeometry = IBoxGeometry | IPlaneGeometry | ISphereGeometry | ILineGeometry | IPointGeometry;

/** Drawable */
export interface IDrawable {
    geometry?: IAnyGeometry;
    appearance?: IStandardAppearance;
 //   matrix?: number[];
}   

//const identity = [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1];

const red = 0xff0000;
const green = 0x00ff00;
const blue = 0x0000ff;
const white = 0xffffff;
//const gray = 0x808080;  

/** Outputs the required react-three-fiber jsx for the specified drawable */
function r3fFromDrawable(drawable: IDrawable) {
    const geometry = drawable.geometry;
    const appearance = drawable.appearance;
    console.log("r3fFromDrawable: geometry:", geometry);

    // uses drei utilities where available - otherwise uses threejs

    // TODO: use-memo to avoid recreating the underlying geometries unnecessarily (see drei docs)

    if (!geometry) {
        console.warn("r3fFromDrawable: no geometry specified in drawable:", drawable);
    }
    else if (geometry.type === 'box') {
        return (
            <Box args={[geometry.width, geometry.height, geometry.depth]}>
                <meshStandardMaterial color={appearance?.color??red} />
            </Box>
        );
    }
    else if (geometry.type === 'plane') {
        return (
            <Plane args={[geometry.width, geometry.height]}>
                <meshStandardMaterial color={appearance?.color??blue} side={THREE.DoubleSide} />
            </Plane>
        );
    }
    else if (geometry.type === 'sphere') {
        console.log("Creating sphere with radius:", geometry.radius);
        return (
            <Sphere args={[geometry.radius, 32, 32]}>
                <meshStandardMaterial color={appearance?.color??green} />
            </Sphere>
        );
    }
    else if (geometry.type === 'line') {
        return (
            <Line
                points={geometry.points}
                color={appearance?.color??blue}
            />
        );
    }
    else if (geometry.type === 'point') {
        return (
            <points> 
                <bufferGeometry>
                    <bufferAttribute
                        attach={'attributes-position'}
                        args={[geometry.position, 3]} 
                    />
                </bufferGeometry>
                <pointsMaterial
                    size={4}
                    sizeAttenuation={false}
                    color={appearance?.color??red}
                />
            </points>
        );
    }
}

/** return the jsx that puts the drawable into the scene */
export function Drawable({drawable}: {drawable: IDrawable}) {
    console.log("++++++++++++++++++++++++++++");
    return r3fFromDrawable(drawable);
}