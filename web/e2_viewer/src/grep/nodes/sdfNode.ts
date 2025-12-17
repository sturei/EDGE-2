/** Definition of nodes in the sdf scene */

export type SdfNodeType = 
    'sphere' | 'block' | 'cylinder' | 'circle' | 'rectangle' | 'roundRect' | 'profile' | 'extrusion' | 
    'halfSpace' | 'union' | 'intersection' | 'complement' | 'translation' | 'rotation' | 'gyroid';

export interface IBaseSdfNode {
    type: SdfNodeType;
    pathName: string;
    childIndices?: number[];
}

export interface ISphereSdfNode extends IBaseSdfNode {
    type: 'sphere';
    radius: number;
};

export interface IBlockSdfNode extends IBaseSdfNode {
    type: 'block';
    width: number;
    height: number;
    depth: number;
};

export interface ICylinderSdfNode extends IBaseSdfNode {
    type: 'cylinder';
    radius: number;
    depth: number;
};

export interface ICircleSdfNode extends IBaseSdfNode {
    type: 'circle';
    radius: number;
};

export interface IRectangleSdfNode extends IBaseSdfNode {
    type: 'rectangle';
    width: number;
    height: number;
};

export interface IRoundRectSdfNode extends IBaseSdfNode {
    type: 'roundRect';
    width: number;
    height: number;
    cornerRadius: number;
};

export interface IProfileSdfNode extends IBaseSdfNode {
    type: 'profile';
    paths: Array<Array<[number, number]>>; // array of paths, each path is an array of [x,y] tuples
}

export interface IExtrusionSdfNode extends IBaseSdfNode {
    type: 'extrusion';
    depth: number;
};

export interface IHalfSpaceSdfNode extends IBaseSdfNode {
    type: 'halfSpace';
};

export interface IUnionSdfNode extends IBaseSdfNode {
    type: 'union';
};

export interface IIntersectionSdfNode extends IBaseSdfNode {
    type: 'intersection';
};

export interface IComplementSdfNode extends IBaseSdfNode {
    type: 'complement';
};

export interface ITranslationSdfNode extends IBaseSdfNode {
    type: 'translation';
    position: [number, number, number];
};

export interface IRotationSdfNode extends IBaseSdfNode {
    type: 'rotation';
    angles: [number, number, number]; // Euler angles in radians
};

export interface IGyroidSdfNode extends IBaseSdfNode {
    type: 'gyroid';
    cellLength: number;
};

export type ISdfNode = 
    ISphereSdfNode | IBlockSdfNode | ICylinderSdfNode | ICircleSdfNode | IRectangleSdfNode | IRoundRectSdfNode | IProfileSdfNode |  IExtrusionSdfNode | 
    IHalfSpaceSdfNode | IUnionSdfNode | IIntersectionSdfNode | IComplementSdfNode | ITranslationSdfNode | IRotationSdfNode | IGyroidSdfNode;