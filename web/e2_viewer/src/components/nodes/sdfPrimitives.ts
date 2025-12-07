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
    sign,
    float,
    sqrt,
    int,
    Loop,
    If
} from 'three/tsl'

// sphere centered on origin with given radius
export const sphere = Fn(([position, radius]: [any, any]) => {
    return length(position).sub(radius);
})

// block centered on origin with given dimensions in x, y, z
export const block = Fn(([position, width, height, depth] : [any, any, any, any]) => {
    const dimensions = vec3(width.mul(0.5), height.mul(0.5), depth.mul(0.5));
    const distance = abs(position).sub(dimensions);
    return length(max(distance, 0.0)).add(
        min(max(distance.x, max(distance.y, distance.z)), 0.0)
    )
})

// cylinder centered on origin with given radius and depth along Z
export const cylinder = Fn(([position, radius, depth]: [any, any, any]) => {
    const distance = float(0).toVar();
    const delta = vec2(
      length(position.xy).sub(radius),
      position.z.abs().sub(depth.mul(0.5))
    )
    distance.assign(min(max(delta.x, delta.z), 0.0).add(
        length(max(delta, 0.0))
    ));
    return distance;
})

// path (polyline) in XY plane
export const path = Fn(([position, pathPoints]: [any, any, any]) => {    
    const minDistance = float(99999.99).toVar(); // Large initial value
    const numPoints = int(pathPoints.count);

    // Loop over each segment (between successive points) in the path
    Loop( {start: 0, end: numPoints.add(-1), condition: '<' }, ( { i } ) => {
        const p1 = pathPoints.element(i);
        const p2 = pathPoints.element((i as any).add(1));

        const segmentDir = p2.sub(p1);
        const queryToP1 = position.xy.sub(p1);

        // Project position onto the infinite line (p1 + t * segmentDir)
        let t = queryToP1.dot(segmentDir).div(segmentDir.dot(segmentDir));

        // Clamp t to the segment (0 to 1)
        t = max(min(t, 1.0), 0.0);

        // Calculate the closest point on the segment
        const currentClosest = p1.add(segmentDir.mul(t));

        // Calculate distance to this segment's closest point
        const dist = length(position.xy.sub(currentClosest));

        minDistance.assign(min(minDistance, dist));
    } );

    return minDistance;
})

// profile (array of non-contiguous but oriented paths) in XY plane
export const profile = Fn(([position, profilePaths, profileNormals, profilePathLengths]: [any,any, any, any]) => {
    const minDistanceSq = float(99999.99).toVar(); // Large initial value
    const minSign = int(1).toVar(); // positive = outside, negative = inside 
    const numPaths = int(profilePathLengths.count);
    const index = int(0);

    // Loop over each path in the profile
    Loop( {start: 0, end: numPaths, condition: '<' }, ( { i } ) => {
        const numPoints = int(profilePathLengths.element(i));

        // Loop over each segment in the path
        Loop( {start: 0, end: numPoints.add(-1), condition: '<' }, () => {
            const p1 = profilePaths.element(index);
            const p2 = profilePaths.element(index.add(1));

            const segmentDir = p2.sub(p1);
            const delta = position.xy.sub(p1);

            // Project position onto the infinite line (p1 + t * segmentDir)
            const t = delta.dot(segmentDir).div(segmentDir.dot(segmentDir));

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

// half-space defined by plane position and normal
export const halfSpace = Fn(([position, planePosition, planeNormal]: [any, any, any]) => {
    return position.sub(planePosition).dot(planeNormal);
})

