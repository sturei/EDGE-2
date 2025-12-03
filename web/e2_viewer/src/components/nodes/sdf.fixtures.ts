import { Fn, vec3, length, abs, max, min, add, mul, time, oscSine } from 'three/tsl'

const Sphere = Fn(([position, radius]: [any, any]) => {
  return length(position).sub(radius)
})

const Box = Fn(([position, dimensions] : [any, any]) => {
        const distance = abs(position).sub(dimensions)
        //return length(max(distance, 0.0))  // better if you don't need internal calculations. E.g, intersect or subtraction
        return length(max(distance, 0.0)).add(
          min(max(distance.x, max(distance.y, distance.z)), 0.0)
        )
})

const SmoothMinimum = Fn(([a, b, k]: [any, any, any]) => {
  const h = max(k.sub(abs(a.sub(b))), 0).div(k)
  return min(a, b).sub(h.mul(h).mul(k).mul(0.25))
})

export const BoxUnionedWithMetaballs = Fn(([position]: [any]) => {
    const t = time.mul(0.2)

    const sphere = Sphere(position.sub(vec3(0, 0, 0)), 1)
    const translatedPos = position.add(vec3(oscSine(t).mul(4).sub(2), 0, 0)); // oscSine returns [0,1]
    const movingSphere = Sphere(translatedPos.sub(vec3(0, 1, 0)), 0.5)
    const box = Box(position.sub(vec3(0, 0, 0)), vec3(1,2,3).mul(0.5))
    const distance = SmoothMinimum(sphere, movingSphere, 0.5);
    distance.assign(min(distance, box));

    return distance 
})
