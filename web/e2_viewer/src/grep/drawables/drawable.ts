/**
 * Definitions for drawable items.
 * A drawable is simply a piece of geometry together with its appearance.
 * Of course this is just a tiny subset of what can be done with three.js.
 */

/** Appearances */

export type AppearanceType = 'point' | 'line' | 'mesh';

export interface IBaseAppearance {
    type: AppearanceType;
};

/** A simple color map from color names to hexadecimal color values */

export const Color = new Map<string, number> ([
    ["red", 0xff0000],              // 16711680 in decimal
    ["green", 0x00ff00],            // 65280 in decimal
    ["blue", 0x0000ff],             // 255 in decimal
    ["white", 0xffffff],            // 16777215 in decimal
    ["gray", 0x808080],             // 8421504 in decimal
    ["yellow", 0xffff00]            // 16776960 in decimal  
]);

/** Defines the appearance of point-like drawables */
export interface IPointAppearance extends IBaseAppearance {
    type: 'point';
    color?: number;               // color of the point   
    size?: number;                // size of the point in pixels. Note: size is not honoured in webgpu - points are always 1 pixel. The recommended approach is to use point sprites instead.
    // other point appearance properties can be added here. See three.js PointsMaterial for more options.
}

/** Defines the appearance of line-like drawables */
export interface ILineAppearance extends IBaseAppearance {
    type: 'line';
    color?: number;
    // other line appearance properties can be added here. See three.js LineMaterial for more options.
}

/** Defines the appearance of surface- or solid-like drawables */
export interface IMeshAppearance extends IBaseAppearance {
    type: 'mesh';
    color?: number;
    texture?: {
        width: number;
        height: number;
        data: number[];
    }
    // other mesh appearance properties can be added here. See three.js MeshStandardMaterial for more options.
}

export type IAppearance = IPointAppearance | ILineAppearance | IMeshAppearance; 

/** Geometries */

// "group" could be useful in future?
export type GeometryType = 'sphere' | 'block' | 'cylinder' | 'plane' | 'line' | 'polyline' | 'point' | 'profile' | 'contour';

export interface IBaseGeometry {
    type: GeometryType;
    position?: [number, number, number];
    rotation?: [number, number, number];   // Euler angles in radians
}

/** Sphere centered on the origin */
export interface ISphereGeometry extends IBaseGeometry {
    type: 'sphere'; 
    radius: number;
}

/** Block centered on the origin */
export interface IBlockGeometry extends IBaseGeometry {
    type: 'block';
    width: number;     // length along X
    height: number;    // length along Y
    depth: number;     // length along Z
}

/** Cylinder centered on the origin */
export interface ICylinderGeometry extends IBaseGeometry {
    type: 'cylinder';
    radius: number;    // radius of the cylinder
    depth: number;     // height of the cylinder along the Z axis
}

/** Filled rectangle centered on the origin */
export interface IPlaneGeometry extends IBaseGeometry {
    type: 'plane';
    width: number;     // length along X
    height: number;    // length along Y    
}

/** Line defined by start and end points */
export interface ILineGeometry extends IBaseGeometry {
    type: 'line';
    start: [number, number, number];
    end: [number, number, number];
}

/** Polyline defined by an array of [x,y,z] triples */
export interface IPolylineGeometry extends IBaseGeometry {
    type: 'polyline';
    positions: Array<[number, number, number]>;
}

/** Point at origin */
export interface IPointGeometry extends IBaseGeometry {
    type: 'point';
}

/** 
 * Filled profile defined by its boundary.
 * Boundary is an array of paths. 
 * Each path is an array of [x,y] tuples.
 * Paths must be consistently oriented with the interior on the left.
 * Paths need not be contiguous. 
 */
export interface IProfileGeometry extends IBaseGeometry {
    type: 'profile';
    paths: Array<Array<[number, number]>>;
}

/**
 * Wireframe contour (similar to IProfileGeometry, but renders the boundary, not the interior.
 */
export interface IContourGeometry extends IBaseGeometry {
    type: 'contour';
    paths: Array<Array<[number, number]>>;
    position2D: [number, number];
    rotation2D: number;   // in radians
    zOffset: number;
}

export type IGeometry = ISphereGeometry | IBlockGeometry | ICylinderGeometry | IPlaneGeometry | ILineGeometry | IPolylineGeometry | IPointGeometry | IProfileGeometry | IContourGeometry;

/** Drawable */
export interface IDrawable {
    geometry?: IGeometry;
    appearance?: IAppearance;
}   

