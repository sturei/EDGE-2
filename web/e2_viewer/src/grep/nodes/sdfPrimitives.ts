/** Primitives from which to construct an sdfScene 
 * See https://sbcode.net/tsl/raymarching/
 * and https://tympanus.net/codrops/2024/07/15/how-to-create-a-liquid-raymarching-scene-using-three-js-shading-language/
 * and https://iquilezles.org/articles/distfunctions/
*/

import {    
    Fn,
    length,
    lengthSq,
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

//import * as THREE from 'three/webgpu';
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
    return extrusion(position, rectangle(position, width, height), depth);
})

// block centered on origin with given dimensions in x, y, z and top/bottom edges rounded with given corner radius
export const roundedBlock = Fn(([position, width, height, depth, cornerRadius] : [any, any, any, any, any]) => {
    return roundedExtrusion(position, rectangle(position, width, height), depth, cornerRadius);
})

// block centered on origin with given dimensions in x, y, z and all edges rounded with given corner radius
export const roundBlock = Fn(([position, width, height, depth, cornerRadius] : [any, any, any, any, any]) => {
    return roundedExtrusion(position, roundRect(position, width, height, cornerRadius), depth, cornerRadius);
})

// cylinder centered on origin with given radius and depth along Z
export const cylinder = Fn(([position, radius, depth]: [any, any, any]) => {
    return extrusion(position, circle(position, radius), depth);
})

// cylinder centered on origin with given radius, depth along Z, and corner radius for top and bottom edges
export const roundedCylinder = Fn(([position, radius, depth, cornerRadius]: [any, any, any, any]) => {
    return roundedExtrusion(position, circle(position, radius), depth, cornerRadius);
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

export const extrusion = Fn(([position, tslProfile, depth]: [any, any, any]) => {
    const d = tslProfile;
    const w = vec2( d, abs(position.z).sub(mul(depth, 0.5)) );
    return min( max(w.x, w.y), 0.0 ).add( length( max(w, 0.0) ) );
})

// extrusion with top and bottom edges rounded
export const roundedExtrusion = Fn(([position, tslProfile, depth, cornerRadius]: [any, any, any, any]) => {
    const d = tslProfile.add(cornerRadius);
    const w = vec2( d, abs(position.z).sub(mul(depth, 0.5).sub(cornerRadius)) );
    return min( max(w.x, w.y), 0.0 ).add( length( max(w, 0.0) ) ).sub(cornerRadius);
})

// gyroid with given lambda factor (lamba = 2pi / cellSize);
export const gyroid = Fn(([position, lambda]: [any, any]) => {

    // See, for example, Ramirez et al., "Design parameter effects on relative density of triply periodic minimal surfaces for additive manufacturing", 2019
    const cosx = lambda.mul(position.x).cos();
    const cosy = lambda.mul(position.y).cos();
    const cosz = lambda.mul(position.z).cos();
    const sinx = lambda.mul(position.x).sin();
    const siny = lambda.mul(position.y).sin();
    const sinz = lambda.mul(position.z).sin();

    const xyTerm = cosx.mul(siny);
    const yzTerm = cosy.mul(sinz);
    const zxTerm = cosz.mul(sinx);
    const f = xyTerm.add( yzTerm ).add( zxTerm );

    // divide by the maximum possible value of the gradient magnitude (sqrt(3)) to normalize the function. It's not distance, but at least it's a bound.
    return f.div(1.7320508075688772).div(lambda);

})

// half-space (+z)
export const halfSpace = Fn(([position]: [any]) => {
    return position.z;
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


