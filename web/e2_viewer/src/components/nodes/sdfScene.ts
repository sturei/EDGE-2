import {    
    float,
    vec3,
    Fn
} from 'three/tsl'
import type { IShaderNode } from '../../grep/shaderNode';
import { block, complement, intersection, sphere, union } from './sdfPrimitives';


const evaluateNode = Fn(([position, node, sdfScene]: [any, IShaderNode, IShaderNode[]]) => {
    const type = node.type;
    let result = float(0.0).toVar();

    if (type == 'sphere') {
        const radius = node.parameters?.get('radius') ?? 1.0;
        result.assign(sphere(position, radius)); 
    }
    else if (type == 'block') {
        const dimensions = node.parameters?.get('dimensions') ?? vec3(1.0, 1.0, 1.0);
        result.assign(block(position, dimensions)); 
    }
    else if (type == 'union') {
        // TODO: move this logic to unionMultiple in sdfPrimitives.ts?
        const childIndices = node.childIndices;
        if (!childIndices || childIndices.length == 0) {
            throw new Error("Union node missing childIndices");
        }
        if (childIndices.length == 1) {
            const d1 = evaluateNode(position, sdfScene[childIndices[0]], sdfScene);
            result.assign(d1);
        }
        else if (childIndices.length == 2) {
            const d1 = evaluateNode(position, sdfScene[childIndices[0]], sdfScene);
            const d2 = evaluateNode(position, sdfScene[childIndices[1]], sdfScene);
            result.assign(union(d1, d2));
        }
        else {
            // more than 2 children, do pairwise union. 
            let currentUnion = evaluateNode(position, sdfScene[childIndices[0]], sdfScene);
            for (let i = 1; i < childIndices.length; i++) {
                const dNext = evaluateNode(position, sdfScene[childIndices[i]], sdfScene);
                currentUnion = union(currentUnion, dNext);
            }
            result.assign(currentUnion);
        }
    }
    else if (type == 'intersection') {
        const childIndices = node.childIndices;
        if (!childIndices || childIndices.length == 0) {
            throw new Error("Intersection node missing childIndices");
        }
        if (childIndices.length == 1) {
            const d1 = evaluateNode(position, sdfScene[childIndices[0]], sdfScene);
            result.assign(d1);
        }
        else if (childIndices.length == 2) {
            const d1 = evaluateNode(position, sdfScene[childIndices[0]], sdfScene);
            const d2 = evaluateNode(position, sdfScene[childIndices[1]], sdfScene);
            result.assign(intersection(d1, d2));
        }
        else {
            // more than 2 children, do pairwise intersection. 
            let currentIntersection = evaluateNode(position, sdfScene[childIndices[0]], sdfScene);
            for (let i = 1; i < childIndices.length; i++) {
                const dNext = evaluateNode(position, sdfScene[childIndices[i]], sdfScene);
                currentIntersection = intersection(currentIntersection, dNext);
            }
            result.assign(currentIntersection);
        }
    }
    else if (type == 'complement') {
        const childIndices = node.childIndices;
        if (!childIndices || childIndices.length != 1) {
            throw new Error("Complement node missing childIndices");
        }
        const d1 = evaluateNode(position, sdfScene[childIndices[0]], sdfScene);
        result.assign(complement(d1));
    }
    return result;
});

export const sdfScene = Fn(([position, nodes]: [any, any]) => {
    //const rootNode = nodes.get(0);   // by convention, root node is at index 0
    //return evaluateNode(position, rootNode, nodes);
    return float(2.0); // placeholder until we re-enable sdfSceneTest   
})


