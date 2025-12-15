/** Definition of nodes in the sdf scene */

export type SdfNodeType = 'sphere' | 'block' | 'cylinder' | 'circle' | 'rectangle' | 'roundRect' | 'profile' | 'extrusion' | 'halfSpace' | 'union' | 'intersection' | 'complement' | 'translation' | 'rotation';

export interface ISdfNode {
    pathName: string;       // unique identifier for this node
    type: SdfNodeType;   // type of shader node
    childIndices?: number[]; // indices of child nodes in the model's shader node list
    parameters?: {[key: string]: any}; // parameters specific to this node type. E.g. for 'sphere', { radius: number }. Eventually these will be passed as uniforms to the shader.

    // TODO: consider adding specific ISdfNode sub-interfaces for each node type to enforce parameter types (similar to how IDrawable works).
}
