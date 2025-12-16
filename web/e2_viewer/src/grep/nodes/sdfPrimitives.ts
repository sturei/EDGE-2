/** Primitives from which to construct an sdfScene 
 * See https://sbcode.net/tsl/raymarching/
 * and https://tympanus.net/codrops/2024/07/15/how-to-create-a-liquid-raymarching-scene-using-three-js-shading-language/
 * and https://iquilezles.org/articles/distfunctions/
*/

import {    
    Fn,
    length,
    lengthSq,
    vec3,
    vec2,
    min,
    abs,
    max,
    mul,
    sign,
    sqrt,
    float,
    int,
    Loop,
    If
} from 'three/tsl'

import * as THREE from 'three/webgpu';
// Implementation note: the above line brings in the types, including the types for THREE.Node etc. 
// Unfortunately, at time of writing, the types are out-of-date and cannot be used, hence the widespread use of 'any' in this file.
// The arguments and return types are THREE.Node, in general, or more specific types such as THREE.ArrayNode.
// As a rule-of-thumb, arguments should be THREE.Node or value types that convert to THREE.Node according to
// https://github.com/mrdoob/three.js/wiki/Three.js-Shading-Language#constants-and-explicit-conversions

// sphere centered on origin with given radius
export const sphere = Fn(([position, radius]: [any, any]) => {
    return length(position).sub(radius);
})

// block centered on origin with given dimensions in x, y, z
export const block = Fn(([position, width, height, depth] : [any, any, any, any]) => {
    const dimensions = vec3(mul(width, 0.5), mul(height, 0.5), mul(depth, 0.5))    ;
    const distance = abs(position).sub(dimensions);
    return length(max(distance, 0.0)).add(
        min(max(distance.x, max(distance.y, distance.z)), 0.0)
    )
})

// cylinder centered on origin with given radius and depth along Z
export const cylinder = Fn(([position, radius, depth]: [any, THREE.Node | number, THREE.Node | number]) => {
    const distance = float(0).toVar();
    const delta = vec2(
      length(position.xy).sub(radius),
      position.z.abs().sub(mul(depth, 0.5))
    )
    distance.assign(min(max(delta.x, delta.z), 0.0).add(
        length(max(delta, 0.0))
    ));
    return distance;
})

// circle centered on origin with given radius. Returns distance in XY plane.
export const circle = Fn(([position, radius]: [any, any]) => {
    return length(position.xy).sub(radius);
})

// rectangle centered on origin with given width and height. Returns distance in XY plane.
export const rectangle = Fn(([position, width, height]: [any, any, any]) => {
    const dimensions = vec2(mul(width, 0.5), mul(height, 0.5));
    const distance = abs(position.xy).sub(dimensions);
    return length(max(distance, 0.0)).add(
        min(max(distance.x, distance.y), 0.0)
    )
})

// rounded rectangle centered on origin with given width, height and corner radius. Returns distance in XY plane.
export const roundRect = Fn(([position, width, height, cornerRadius]: [any, any, any, any]) => {
    const dimensions = vec2(mul(width, 0.5), mul(height, 0.5));
    const distance = abs(position.xy).sub(dimensions.sub(vec2(cornerRadius)));
    return length(max(distance, 0.0)).add(
        min(max(distance.x, distance.y), 0.0)
    ).sub(cornerRadius);
})

// profile (array of non-contiguous but oriented paths) in XY plane. The arguments are ArrayNodes.
export const profile = Fn(([position, profilePaths, profileNormals, profilePathLengths]: [any, any, any, any]) => {

    // Implementation note:
    // This could be made more efficient, e.g. see Iniquez's article on polygonal SDFs
    // However, even with the more efficient approach, this is way too slow to be a real-time SDF primitive.
    // I guess it's to do with the Loop and If constructs.
    // With current technology, it's probably necessary to bake the profile SDF into a 2D texture and sample it.

    const minDistanceSq = float(99999.99).toVar(); // Large initial value
    const minSign = int(1).toVar(); // positive = outside, negative = inside 
    const numPaths = int(profilePathLengths.count);
    const index = int(0).toVar();  // index into concatenated paths

    // Loop over each path in the profile
    Loop( {start: 0, end: numPaths, condition: '<' }, ( { i } ) => {
        const numPoints = int(profilePathLengths.element(i)).toVar();

        // Loop over each segment in the path
        Loop( {start: 0, end: numPoints.add(-1), condition: '<' }, () => {
            const p1 = profilePaths.element(index);
            const p2 = profilePaths.element(index.add(1));

            const segmentDir = p2.sub(p1);
            const delta = position.xy.sub(p1);

            // Project position onto the infinite line (p1 + t * segmentDir)
            const t = delta.dot(segmentDir).div(segmentDir.dot(segmentDir)).toVar();

            // Clamp t to the segment (0 to 1)
            t.assign(max(min(t, 1.0), 0.0));

            // Calculate the closest point on the segment
            const currentClosest = p1.add(segmentDir.mul(t));

            // Calculate distance to this segment's closest point
            const distSq = lengthSq(position.xy.sub(currentClosest));

            If( distSq.lessThan(minDistanceSq), () => {
                // Determine which side of the segment the position is on
                const normal = profileNormals.element(index);
                const toPoint = position.xy.sub(currentClosest);
                const dotProduct = normal.dot(toPoint);
                minSign.assign(sign(dotProduct));
                minDistanceSq.assign(distSq);
            });

            index.addAssign(1);   // move to next segment
        });
        index.addAssign(1);   // move to next path
    } );

    return minDistanceSq.sqrt().mul(minSign);
})

export const extrusion = Fn(([position, tslProfile, depth]: [any, any, any]) => {
    const w_x = tslProfile;
    const w_y = position.z.abs().sub(mul(depth, 0.5));
    const w_max = max(w_x, w_y);
    const w_x_pos = max(w_x, 0.0);
    const w_y_pos = max(w_y, 0.0);
    const w_length = sqrt( w_x_pos.mul(w_x_pos).add( w_y_pos.mul(w_y_pos) ) );
    return min(w_max, 0.0).add( w_length );
})

// half-space defined by plane position and normal
export const halfSpace = Fn(([position, planePosition, planeNormal]: [any, any, any]) => {
    return position.sub(planePosition).dot(planeNormal);
})

