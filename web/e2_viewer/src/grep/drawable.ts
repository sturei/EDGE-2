/**
 * Definitions for drawable items.
 * A drawable is simply a piece of geometry together with its appearance.
 * Of course this is just a tiny subset of what can be done with three.js.
 */

/** Appearances */

export const Color = new Map<string, number> ([
    ["red", 0xff0000],              // 16711680 in decimal
    ["green", 0x00ff00],            // 65280 in decimal
    ["blue", 0x0000ff],             // 255 in decimal
    ["white", 0xffffff],            // 16777215 in decimal
    ["gray", 0x808080],             // 8421504 in decimal
    ["yellow", 0xffff00]            // 16776960 in decimal  
]);

/** Defines the appearance of point-like drawables */
export interface IPointAppearance {
    type: 'point';
    color?: number;               // color of the point   
    size?: number;                // size of the point in pixels
    // other point appearance properties can be added here. See three.js PointsMaterial for more options.
}

/** Defines the appearance of line-like drawables */
export interface ILineAppearance {
    type: 'line';
    color?: number;
    // other line appearance properties can be added here. See three.js LineMaterial for more options.
}

/** Defines the appearance of surface- or solid-like drawables */
export interface IMeshAppearance {
    type: 'mesh';
    color?: number;
    texture?: {
        width: number;
        height: number;
        data: number[];
    }
    // other mesh appearance properties can be added here. See three.js MeshStandardMaterial for more options.
}

export type IAnyAppearance = IPointAppearance | ILineAppearance | IMeshAppearance; 

/** Geometries */

/** Block centered on the origin */
export interface IBlockGeometry {
    type: 'block';
    width: number;     // length along X
    height: number;    // length along Y
    depth: number;     // length along Z
}

/** Filled rectangle centered on the origin */
export interface IPlaneGeometry {
    type: 'plane';
    width: number;     // length along X
    height: number;    // length along Y    
    z: number;         // position along Z
}

/** Sphere centered on the origin */
export interface ISphereGeometry {
    type: 'sphere'; 
    radius: number;
}

/** Line defined by start and end points */
export interface ILineGeometry {
    type: 'line';
    start: [number, number, number];
    end: [number, number, number];
}

/** Polyline defined by an array of [x,y,z] triples */
export interface IPolylineGeometry {
    type: 'polyline';
    positions: Array<[number, number, number]>;
}

/** Point at specified position */
export interface IPointGeometry {
    type: 'point';
    position: [number, number, number];
}

/** Filled profile defined by an array of 2D paths. 
 * Each path is an array of [x,y] tuples.
 * Paths must be consistently oriented with the interior on the left. 
 * The union of the paths must form the boundary of a simple polygon. */
export interface IProfileGeometry {
    type: 'profile';
    paths: Array<Array<[number, number]>>;
}

export type IAnyGeometry = IBlockGeometry | IPlaneGeometry | ISphereGeometry | ILineGeometry | IPolylineGeometry | IPointGeometry | IProfileGeometry;

/** Drawable */
export interface IDrawable {
    geometry?: IAnyGeometry;
    appearance?: IAnyAppearance;
 //   matrix?: number[];
}   

