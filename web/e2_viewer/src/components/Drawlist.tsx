/** This component supplies the list of objects to be drawn */

import { Drawable } from "./Drawable";
import { type IDrawable } from "../grep/drawable";

export function Drawlist({drawlist}: {drawlist: IDrawable[]}) {
    return (
        <>
          {drawlist.map((drawable, index) => (
            <Drawable drawable={drawable} key={index}/>
          ))}
        </>
    )
}
