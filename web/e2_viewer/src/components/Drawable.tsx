//
// Implentation note: I think this will create new geometry even if the geometry is unchanged and all that has changed is the appearance or the matrix.
// To avoid this it's probably possible to create a geometry cache keyed on name or id. Cache would be owned by the application and passed in as a callback to access it.

// eventually, other types of appearance like IPhongAppearance, IWireframeAppearance, IFlatAppearance, IVertexColorAppearance etc.
export interface IStandardAppearance {
    type: 'standard';
    color: number;
}
export interface IBoxGeometry {
    type: 'box';
    width: number;
    height: number;
    depth: number;
}

// eventually, IBoxGeometry | IMeshGeometry | ISphereGeometry etc. 
// Similarly for appearance.

export interface IDrawable {
    geometry: IBoxGeometry;
    appearance?: IStandardAppearance;
 //   matrix?: number[];
}   

//const identity = [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1];

export function Drawable({args}: {args: IDrawable}) {
    console.log("Drawable: rendering args:", args);
    return (
        <>
          <mesh>
            <boxGeometry args={[args.geometry.width, args.geometry.height, args.geometry.depth]}/>
            <meshStandardMaterial color={args.appearance?.color}/>
          </mesh>
        </>
    )
}