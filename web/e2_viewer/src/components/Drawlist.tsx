/** This component supplies the list of objects to be drawn */

import { Drawable, type IDrawable } from "./Drawable";

export function Drawlist({drawlist}: {drawlist: IDrawable[]}) {
    return (
        <>
          {drawlist.map((drawable, index) => (
            <Drawable drawable={drawable} key={index}/>
          ))}
        </>
    )
}
