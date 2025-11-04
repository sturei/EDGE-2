/**
 * Definitions for drawable geometries and appearances.
 * The drawlist in a GRepModel is just a vector of IDrawable objects.
 * This is a tiny subset of what can be done with three.js/react-three-fiber/drei. Small is simple!
 */

/** Appearances */

export const Color = {
    Red: 0xff0000,              // 16711680 in decimal
    Green: 0x00ff00,            // 65280 in decimal
    Blue: 0x0000ff,             // 255 in decimal
    White: 0xffffff,            // 16777215 in decimal
    Gray: 0x808080,             // 8421504 in decimal
    Yellow: 0xffff00            // 16776960 in decimal
};

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
    // other mesh appearance properties can be added here. See three.js MeshStandardMaterial for more options.
}

export type IAnyAppearance = IPointAppearance | ILineAppearance | IMeshAppearance; 

/** Geometries */

/** Box centered on the origin */
export interface IBoxGeometry {
    type: 'box';
    width: number;     // length along X
    height: number;    // length along Y
    depth: number;     // length along Z
}

/** Filled rectangle centered on the origin */
export interface IPlaneGeometry {
    type: 'plane';
    width: number;     // length along X
    height: number;    // length along Y    
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

/** Point at specified position */
export interface IPointGeometry {
    type: 'point';
    position: [number, number, number];
}

/** Shape defined by a list of 2D points (x1,y1,x2,y2,...) TODO: "Profile defined by paths...*/
export interface IShapeGeometry {
    type: 'shape';
    points: number[];
}

export type IAnyGeometry = IBoxGeometry | IPlaneGeometry | ISphereGeometry | ILineGeometry | IPointGeometry | IShapeGeometry;

/** Drawable */
export interface IDrawable {
    geometry?: IAnyGeometry;
    appearance?: IAnyAppearance;
 //   matrix?: number[];
}   

