import { Drawable, type IDrawable } from "./Drawable";

export function Drawlist({drawlist}: {drawlist: IDrawable[]}) {
    return (
        <>
          {drawlist.map((drawable, index) => (
            <Drawable args={drawable} key={index}/>
          ))}
        </>
    )
}
