import { sphere, block, cylinder, profile, halfSpace, circle, rectangle, roundRect, extrusion, gyroid } from './sdfPrimitives';
import { type ISdfNode } from './sdfNode';
import { Fn, int, min, max, mod, negate, rotate, vec2, vec3, array } from 'three/tsl';    

// compute outward-pointing normals for profile
function computeProfileNormals(paths: Array<Array<[number, number]>>): Array<Array<[number,number]>> {
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
    return computedNormals;
}

// generate the TSL code for a given node
function generateNode(nodeIndex: number, nodes: ISdfNode[]) {
    const node = nodes[nodeIndex];
    if (node.type === 'sphere') {
        return Fn(([position] : [any]) => {
            return sphere(position, node.radius);
        });
    }
    else if (node.type === 'block') {
        return Fn(([position] : [any]) => {
            return block(position, node.width, node.height, node.depth);
        });
    }
    else if (node.type === 'cylinder') {
        return Fn(([position] : [any]) => {
            return cylinder(position, node.radius, node.depth);
        });
    }
    else if (node.type === 'circle'){
        return Fn(([position] : [any]) => {
            return circle(position, node.radius);
        });
    }
    else if (node.type === 'rectangle'){
        return Fn(([position] : [any]) => {
            return rectangle(position, node.width, node.height);
        });
    }   
    else if (node.type === 'roundRect'){
        return Fn(([position] : [any]) => {
            return roundRect(position, node.width, node.height, node.cornerRadius);
        });
    }
    else if (node.type === 'profile') {
        const paths = node.paths;

        let normals = undefined; // TODO: add optional (client-supplied) normals to the node. Only needed if profile will be actually used! (it's slow). Or, compute containment by another method..
        if (!normals) {
            normals = computeProfileNormals(paths);
        }

        console.log("Profile with " + paths.length + " paths:");       // --- DEBUG ---
        for (let p = 0; p < paths.length; p++) {
            console.log(`  Path ${p} with ${paths[p].length} points`);   // --- DEBUG ---
        }
        console.log("Profile normals:", normals);   // --- DEBUG ---

        // concatenate the paths into a single array, and keep track of lengths
        // (I couldn't make an array of arrays in TSL, but maybe that was due to my inexperience)
        const concatenatedPaths: Array<any> = [];
        const concatenatedNormals: Array<any> = []; 
        const pathLengths: Array<any> = [];
        for (let i = 0; i < paths.length; i++) {
            const pathPoints = paths[i];
            const vec2Points: Array<any> = pathPoints.map(pt => vec2(pt[0], pt[1]));
            concatenatedPaths.push(...vec2Points);
            pathLengths.push(int(pathPoints.length));
            if (normals) {
                const pathNormals = normals[i];
                const vec2Normals: Array<any> = pathNormals.map(nrm => vec2(nrm[0], nrm[1]));
                concatenatedNormals.push(...vec2Normals);
            }
        }

        console.log("Concatenated paths has total length:", concatenatedPaths.length);      // --- DEBUG ---  

        // convert concatenated arrays to TSL arrays
        const tslPaths = array(concatenatedPaths);
        const tslNormals = array(concatenatedNormals);
        const tslPathLengths = array(pathLengths);

        // return the profile SDF function
        // Implementation note: the generated functions is too slow for real-time use. Maybe try passing in consts for the lengths? Something about non-dynamic loops in GLSL?
        return Fn(([position] : [any]) => {
            return profile(position, tslPaths, tslNormals, tslPathLengths);
        });
    }
    else if (node.type === 'extrusion') {
        const childIndices = node.childIndices;

        // Implementation note; We do the error checking here, at generation time, rather than deferring to render time when the error would be impossible to recover from.
        if (!childIndices || childIndices.length == 0) {
            throw new Error("Extrusion node missing childIndices");
        }
        if (childIndices.length != 1) {
            throw new Error("Extrusion node must have exactly one child");
        }

        return Fn(([position] : [any]) => {
            const tslProfile = generateNode(childIndices[0], nodes)(position);
            return extrusion(position, tslProfile, node.depth);
        });
    }   
    else if (node.type === 'halfSpace') {
        return Fn(([position] : [any]) => {
            return halfSpace(position);
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
        // Implementation note: 2 different approaches seem to be possible here:
        // 1. Loop through children in TSL code
        // 2. Loop through children here at generation time
        // Approach 1 is taken here because Loop is slow, at least it was for Profile SDF. 
        // FWIW. approach 2 is taken for extrusion above - there is only one child there, so no loop needed.
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
        // Implementation note: follows the pattern of union, but could have been done like extrusion above.
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
        const position = node.position;
        const t = vec3(position[0], position[1], position[2]);
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
        const angles = node.angles;
        const r = vec3(angles[0], angles[1], angles[2]);
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
    else if (node.type === 'gyroid') {
        const cellLength = node.cellLength;
        const l_x = Math.PI * 2 / cellLength;
        const l_y = Math.PI * 2 / cellLength;
        const l_z = Math.PI * 2 / cellLength;
        const lambda = vec3(l_x, l_y, l_z);
        return Fn(([position] : [any]) => {
            return gyroid(position, lambda);
        });
    }
    else if (node.type === 'repetition') {
        const childIndices = node.childIndices;
        if (!childIndices || childIndices.length == 0) {
            throw new Error("Repetition node missing childIndices");
        }
        if (childIndices.length != 1) {
            throw new Error("Repetition node must have exactly one child");
        }

        const cellLength = node.cellLength;
        const cellSize = vec3(cellLength);
        return Fn(([position] : [any]) => {
            // This works just fine if the repeating primitive is symmetric about the x, y, and z planes and contained in its cell,
            // so that the closest point in the cell is always the closest point overall. Otherwse, see iniquez's article on repeating SDFs.

            const repeatingPos = mod(position.add(cellSize.mul(0.5)), cellSize).sub(cellSize.mul(0.5));
            return generateNode(childIndices[0], nodes)(repeatingPos);
        });
    }
    else {
        // @ts-ignore to suppress 'never' type error (want to keep the code in case new types are added later, that are erroneously not handled here)
        throw new Error(`Unsupported node type: ${node.type}`);
    }
}

export function generateScene(nodes: ISdfNode[]) {
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

