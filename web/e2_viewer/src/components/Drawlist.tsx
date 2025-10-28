/** This component sets up the list of  */

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
