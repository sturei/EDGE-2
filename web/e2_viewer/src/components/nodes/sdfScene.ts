import { sphere, block } from './sdfPrimitives';
import { type IShaderNode } from '../../grep/shaderNode';
import { Fn, min } from 'three/tsl';    

function generateNode(nodeIndex: number, nodes: IShaderNode[]) {
    const node = nodes[nodeIndex];
    if (node.type === 'sphere') {
        const radius = node.parameters?.get('radius') ?? 1.0;
        return Fn(([position] : [any]) => {
            return sphere(position, radius);
        });
    }
    else if (node.type === 'block') {
        const width = node.parameters?.get('width') ?? 1.0;
        const height = node.parameters?.get('height') ?? 1.0;
        const depth = node.parameters?.get('depth') ?? 1.0;
        return Fn(([position] : [any]) => {
            return block(position, width, height, depth);
        });
    }
    else if (node.type === 'union') {
        const childIndices = node.childIndices;
        if (!childIndices || childIndices.length == 0) {
            throw new Error("Union node missing childIndices");
        }
        return Fn(([position] : [any]) => {
            let currentUnion = generateNode(childIndices[0], nodes)(position);
            for (let i = 1; i < childIndices.length; i++) {
                const dNext = generateNode(childIndices[i], nodes)(position);
                currentUnion = min(currentUnion, dNext);
            }
            return currentUnion;
        });
    }
    else {
        throw new Error(`Unsupported node type: ${node.type}`);
    }
}

export function generateScene(nodes: IShaderNode[]) {
    return generateNode(0, nodes);    // root node is always at index 0, by convention
}

