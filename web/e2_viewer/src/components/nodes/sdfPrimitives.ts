/** Primitives from which to construct an sdfScene 
 * See https://sbcode.net/tsl/raymarching/
 * and https://tympanus.net/codrops/2024/07/15/how-to-create-a-liquid-raymarching-scene-using-three-js-shading-language/
 * and https://iquilezles.org/articles/distfunctions/
*/

import {    
    Fn,
    length,
    vec3,
    vec2,
    min,
    abs,
    max
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

export const block = Fn(([position, width, height, depth] : [any, any, any, any]) => {
    const dimensions = vec3(width.mul(0.5), height.mul(0.5), depth.mul(0.5));
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



