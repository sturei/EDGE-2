/** Definition of items that will become nodes in an sdf shader */

export type ShaderNodeType = 'sphere' | 'block' | 'cylinder' | 'profile' | 'extrusion' | 'halfSpace' | 'union' | 'intersection' | 'complement';

export interface IShaderNode {
    pathName: string;       // unique identifier for this node
    type: ShaderNodeType;   // type of shader node
    childIndices?: number[]; // indices of child nodes in the model's shader node list
    parameters?: {[key: string]: any}; // parameters specific to this node type. E.g. for 'sphere', { radius: number }. Eventually these will be passed as uniforms to the shader.

    // TODO: consider adding specific IShaderNode sub-interfaces for each node type to enforce parameter types (similar to how IDrawable works).
}
