import { sphere, block, cylinder, profile, halfSpace } from './sdfPrimitives';
import { type IShaderNode } from '../../grep/shaderNode';
import { Fn, int, float, min, max, negate, rotate, sqrt, vec2, vec3, array } from 'three/tsl';    
import { Euler } from 'three/webgpu';

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

        let normals = node.parameters?.get('normals');
        if (!normals) {
            // compute psuedonormals for each point (cross product of tangent and normal Z)
            console.log("Computing profile normals");   // --- DEBUG ---
            const computedNormals: Array<Array<[number,number]>> = [];
            for (let p = 0; p < paths.length; p++) {
                const pathPoints = paths[p];
                const pathNormals: Array<[number,number]> = [];
                const numPoints = pathPoints.length;
                for (let i = 0; i < numPoints - 1; i++) {
                    const p0 = pathPoints[i];
                    const p1 = pathPoints[i+1];
                    const tangent = [p1[0]-p0[0], p1[1]-p0[1]];
                    const normal = [tangent[1], -tangent[0]]; // -90 degree rotation in XY plane
                    const length = Math.sqrt(normal[0]*normal[0] + normal[1]*normal[1]);
                    if (length > 0) {
                        normal[0] /= length;
                        normal[1] /= length;
                    }
                    pathNormals.push( [normal[0], normal[1]] );
                }
                // add the normal for the last point in the current path (same as the previous point)
                if (numPoints > 0) {
                    pathNormals.push(pathNormals[pathNormals.length - 1]);
                }

                // TODO: compute averaged normals at each vertex
                computedNormals.push(pathNormals);
            }
            normals = computedNormals;
        }

        console.log("Profile with " + paths.length + " paths:");       // --- DEBUG ---
        for (let p = 0; p < paths.length; p++) {
            console.log(`  Path ${p} with ${paths[p].length} points`);   // --- DEBUG ---
        }
        console.log("Profile normals:", normals);   // --- DEBUG ---

        // concatenate the paths into a single array, and keep track of lengths
        const concatenatedPaths: Array<any> = [];
        const concatenatedNormals: Array<any> = []; 
        const pathLengths: Array<any> = [];
        for (let i = 0; i < paths.length; i++) {
            const pathPoints = paths[i];
            const vec2Points: Array<any> = pathPoints.map( (pt: [number,number]) => {
                return vec2(pt[0], pt[1]);
            });
            concatenatedPaths.push(...vec2Points);

            pathLengths.push(int(pathPoints.length));

            if (normals) {
                const pathNormals = normals[i];
                const vec2Normals: Array<any> = pathNormals.map( (nrm: [number,number]) => {
                    return vec2(nrm[0], nrm[1]);
                });
                concatenatedNormals.push(...vec2Normals);
            }
        }

        console.log("Concatenated paths has total length:", concatenatedPaths.length);      // --- DEBUG ---  
        //console.log("Path lengths:", pathLengths);     // --- DEBUG ---
        
        const tslPaths = array(concatenatedPaths);
        const tslNormals = array(concatenatedNormals);
        const tslPathLengths = array(pathLengths);

        return Fn(([position] : [any]) => {
            return profile(position, tslPaths, tslNormals, tslPathLengths);
        });
    }
    else if (node.type === 'extrusion') {
        const depth = node.parameters?.get('depth') ?? 2.0;
        const childIndices = node.childIndices;
        if (!childIndices || childIndices.length == 0) {
            throw new Error("Complement node missing childIndices");
        }
        if (childIndices.length != 1) {
            throw new Error("Complement node must have exactly one child");
        }
        const tslDepth = float(depth);
        return Fn(([position] : [any]) => {
            const profileSDF = generateNode(childIndices[0], nodes)(position).toVar();
            const w_x = profileSDF;;
            const w_y = position.z.abs().sub(tslDepth.mul(0.5));
            const w_max = max(w_x, w_y);
            const w_x_pos = max(w_x, 0.0);
            const w_y_pos = max(w_y, 0.0);
            const w_length = sqrt( w_x_pos.mul(w_x_pos).add( w_y_pos.mul(w_y_pos) ) );
            return min(w_max, 0.0).add( w_length );
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
    else if (node.type === 'translation') {
        const translation = node.parameters?.get('translation') ?? [0, 0, 0];
        const t = vec3(translation[0], translation[1], translation[2]);
        const childIndices = node.childIndices;
        if (!childIndices || childIndices.length == 0) {
            throw new Error("Translation node missing childIndices");
        }
        if (childIndices.length != 1) {
            throw new Error("Translation node must have exactly one child");
        }
        return Fn(([position] : [any]) => {
            const translatedPos = position.sub(t);
            return generateNode(childIndices[0], nodes)(translatedPos);
        });
    }
    else if (node.type === 'rotation') {
        const rotation = node.parameters?.get('rotation') ?? [0, 0, 0];     // Euler angles in radians
        const r = vec3(rotation[0], rotation[1], rotation[2]);
        const childIndices = node.childIndices;
        if (!childIndices || childIndices.length == 0) {
            throw new Error("Rotation node missing childIndices");
        }
        if (childIndices.length != 1) {
            throw new Error("Rotation node must have exactly one child");
        }
        return Fn(([position] : [any]) => {
            const rotatedPos = rotate(position, r);
            return generateNode(childIndices[0], nodes)(rotatedPos);
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

