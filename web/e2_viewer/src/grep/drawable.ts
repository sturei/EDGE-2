

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

export interface IShapeGeometry {
    type: 'shape';
    points: number[];
}

export type IAnyGeometry = IBoxGeometry | IPlaneGeometry | ISphereGeometry | ILineGeometry | IPointGeometry | IShapeGeometry;

/** Drawable */
export interface IDrawable {
    geometry?: IAnyGeometry;
    appearance?: IStandardAppearance;
 //   matrix?: number[];
}   

