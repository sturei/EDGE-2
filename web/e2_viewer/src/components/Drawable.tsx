/** This component creates a 3D object in the scene, based on the provided properties. */

// Implementation note: as written, it will create new geometry even if the geometry is unchanged and all that has changed is the appearance or the matrix.
// Also it does not reuse geometries between different drawables with the same geometry.
// To avoid this it's probably possible to create a geometry cache keyed on name or id. Perhaps provide it as a Context.

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

export interface ISphereGeometry {
    type: 'sphere'; 
    radius: number;
}

export type IAnyGeometry = IBoxGeometry | ISphereGeometry;

/** Drawable */
export interface IDrawable {
    geometry?: IAnyGeometry;
    appearance?: IStandardAppearance;
 //   matrix?: number[];
}   

//const identity = [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1];

/** Creates a react-three-fiber mesh from a drawable that describes its properties */
function meshFromDrawable(drawable: IDrawable) {
    const geometry = drawable.geometry;
    const appearance = drawable.appearance;
    console.log("meshFromDrawable: geometry:", geometry);
    
    if (!geometry) {
        console.warn("meshFromDrawable: no geometry specified in drawable:", drawable);
    }
    else if (geometry.type === 'box') {
        return (
            <mesh>
                <boxGeometry args={[geometry.width, geometry.height, geometry.depth]}/>
                <meshStandardMaterial color={appearance?.color}/>
            </mesh>
        );
    }
    else if (geometry.type === 'sphere') {
        return (
            <mesh>
                <sphereGeometry args={[geometry.radius, 32, 32]}/>
                <meshStandardMaterial color={appearance?.color}/>
            </mesh>
        );
    }

    return (<></>);
}

/** return the jsx that puts the mesh into the scene */
export function Drawable({drawable}: {drawable: IDrawable}) {
    console.log("Drawable: rendering args:", drawable);
    return meshFromDrawable(drawable);
}