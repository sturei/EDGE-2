/** Primitives from which to construct an sdfScene 
 * See https://sbcode.net/tsl/raymarching/
 * and https://tympanus.net/codrops/2024/07/15/how-to-create-a-liquid-raymarching-scene-using-three-js-shading-language/
 * and https://iquilezles.org/articles/distfunctions/
*/


import {    
    float,
    Fn,
    uniform,
    Loop,
    If,
    Break,
    uv,
    length,
    vec3,
    vec2,
    min,
    dot,
    reflect,
    negate,
    mix,
    abs,
    max,
    time,
    sin,
    cos,
    atan,
    normalize,
    positionLocal,
    oscSine
} from 'three/tsl'

export const sphere = Fn(([position, radius]: [any, any]) => {
    return length(position).sub(radius);
})

export const ellipse = Fn(([position, scale]: [any, any]) => {
    const distance = position.div(scale);
    return length(distance)
        .sub(1)
        .mul(min(min(scale.x, scale.y), scale.z))
})

export const torus = Fn(([position, radius, thickness]: [any, any, any]) => {
    const distance = length(position.xz).sub(radius);
    return length(vec2(distance, position.y)).sub(thickness);
})

export const block = Fn(([position, dimensions] : [any, any]) => {
    const distance = abs(position).sub(dimensions);
    return length(max(distance, 0.0)).add(
        min(max(distance.x, max(distance.y, distance.z)), 0.0)
    )
})

export const cylinder = Fn(([position, radius, height]: [any, any, any]) => {
    const distance = vec2(
      length(position.xz).sub(radius),
      position.y.abs().sub(height)
    )
    return min(max(distance.x, distance.y), 0.0).add(
        length(max(distance, 0.0))
    )
})

export const corrugatedCylinder = Fn(([position, radius, height]: [any, any, any]) => {
    const angle = atan(position.x, position.z)
        const bumps = cos(angle.mul(sin(time).mul(6).add(6))).mul(float(0.15))
        const distance = vec2(
          length(position.xz).sub(radius).add(bumps),
          position.y.abs().sub(height)
        )
        return min(max(distance.x, distance.y), 0.0).add(
          length(max(distance, 0.0))
        )
})

export const intersectedSphereBox = Fn(([position, radius]: [any, any]) => {
    const circle = length(position).sub(radius.add(0.33))
    const box = block(position, vec3(radius))
    return max(circle, box)
})

export const subtractedSphereBox = Fn(([position, radius]: [any, any]) => {
    const circle = length(position).sub(radius.add(0.33))
    const box = block(position, vec3(radius))
    return max(negate(circle), box)
})

export const unionedSphereBox = Fn(([position, radius]: [any, any]) => {
    const circle = length(position).sub(radius.add(0.33))
    const box = block(position, vec3(radius))
    return min(circle, box)
})

export const union = Fn(([d1, d2]: [any, any]) => {
    return min(d1, d2);
})

export const multiUnion = Fn(([distances]: [any]) => {
    return distances.reduce((acc: any, d: any) => min(acc, d));
})

export const intersection = Fn(([d1, d2]: [any, any]) => {
    return max(d1, d2);
})

export const multiIntersection = Fn(([distances]: [any]) => {
    return distances.reduce((acc: any, d: any) => max(acc, d));    // will this work in TSL or only in JS? Use Loop, used fixed size?
})

export const complement = Fn(([d]: [any]) => {
    return negate(d);
})

export const multiComplement = Fn(([distances]: [any]) => {
    return multiUnion(distances).negate();
})

// regarding subtraction: symmetric difference using intersection and complement is preferred - no need to worry about the order. 
export const subtraction = Fn(([d1, d2]: [any, any]) => {
    return max(d1, negate(d2));
})

export const smoothUnion = Fn(([d1, d2, k]: [any, any, any]) => {
    const h = max(k.sub(abs(d1.sub(d2))), 0).div(k);
    return min(d1, d2).sub(h.mul(h).mul(k).mul(0.25));
})

export const oscillate = Fn(([position, amplitude, frequency]: [any, any, any]) => {
    const offset = oscSine(frequency).mul(amplitude);
    return position.y.add(offset);
})


