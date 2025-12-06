import { sphere, block, cylinder, path, profile, halfSpace } from './sdfPrimitives';
import { type IShaderNode } from '../../grep/shaderNode';
import { Fn, int, min, max, negate, vec2, vec3, array } from 'three/tsl';    

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
    else if (node.type === 'cylinder') {
        const radius = node.parameters?.get('radius') ?? 1.0;
        const depth = node.parameters?.get('depth') ?? 1.0;
        return Fn(([position] : [any]) => {
            return cylinder(position, radius, depth);
        });
    }
    else if (node.type === 'profile') {
        const paths = node.parameters?.get('paths') ?? new Array<Array<[number,number]>>();

        // concatenate the paths into a single array, and keep track of lengths
        const concatenatedPaths: Array<any> = [];
        const pathLengths: Array<any> = [];
        for (let i = 0; i < paths.length; i++) {
            const pathPoints = paths[i];
            const vec2Array: Array<any> = pathPoints.map( (pt: [number,number]) => {
                return vec2(pt[0], pt[1]);
            });
            concatenatedPaths.push(...vec2Array);
            pathLengths.push(int(vec2Array.length));
        }

        //console.log("Concatenated paths:", concatenatedPaths);      // --- DEBUG ---  
        //console.log("Path lengths:", pathLengths);     // --- DEBUG ---
        
        const tslPaths = array(concatenatedPaths);
        const tslPathLengths = array(pathLengths);

        return Fn(([position] : [any]) => {
            return profile(position, tslPaths, tslPathLengths);
        });
    }
    else if (node.type === 'halfSpace') {
        const planePosition = node.parameters?.get('planePosition') ?? [0, 0, 0];
        const planeNormal = node.parameters?.get('planeNormal') ?? [0, 0, 1];
        const p = vec3(planePosition[0], planePosition[1], planePosition[2]);
        const n = vec3(planeNormal[0], planeNormal[1], planeNormal[2]);
        return Fn(([position] : [any]) => {
            return halfSpace(position, p, n);
        });
    }
    else if (node.type === 'union') {
        const childIndices = node.childIndices;
        if (!childIndices || childIndices.length == 0) {
            throw new Error("Union node missing childIndices");
        }
        return Fn(([position] : [any]) => {
            let current = generateNode(childIndices[0], nodes)(position);
            for (let i = 1; i < childIndices.length; i++) {
                const dNext = generateNode(childIndices[i], nodes)(position);
                current = min(current, dNext);
            }
            return current;
        });
    }
    else if (node.type === 'intersection') {
        const childIndices = node.childIndices;
        if (!childIndices || childIndices.length == 0) {
            throw new Error("Intersection node missing childIndices");
        }
        return Fn(([position] : [any]) => {
            let current = generateNode(childIndices[0], nodes)(position);
            for (let i = 1; i < childIndices.length; i++) {
                const dNext = generateNode(childIndices[i], nodes)(position);
                current = max(current, dNext);
            }
            return current;
        });
    }
    else if (node.type === 'complement') {
        const childIndices = node.childIndices;
        if (!childIndices || childIndices.length == 0) {
            throw new Error("Complement node missing childIndices");
        }
        if (childIndices.length != 1) {
            throw new Error("Complement node must have exactly one child");
        }
        return Fn(([position] : [any]) => {
            let current = generateNode(childIndices[0], nodes)(position);
            current = negate(current);
            return current;
        });
    }
    else {
        throw new Error(`Unsupported node type: ${node.type}`);
    }
}

export function generateScene(nodes: IShaderNode[]) {
    try {
        if (nodes.length == 0) {
            throw new Error("No nodes provided to generateScene");
        }
        return generateNode(0, nodes);    // root node is always at index 0, by convention
    }
    catch (error) {
        console.error("Error generating scene:", error);
        return Fn(([position] : [any]) => {
            return sphere(position, 0);   // return a zero-radius sphere on error
        });
    }
}

