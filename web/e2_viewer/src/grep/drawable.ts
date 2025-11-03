

/** Appearances */

export const Color = {
    Red: 0xff0000,              // 16711680 in decimal
    Green: 0x00ff00,            // 65280 in decimal
    Blue: 0x0000ff,             // 255 in decimal
    White: 0xffffff,            // 16777215 in decimal
    Gray: 0x808080              // 8421504 in decimal
};

export interface IStandardAppearance {
    type: 'standard';
    color: number;
}

export interface IPointAppearance {
    type: 'point';
    color?: number;
    size?: number;
}

export type IAnyAppearance = IStandardAppearance | IPointAppearance;           // other types to be added later, like IPhongAppearance etc.

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
    position: [number, number, number];
}

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

