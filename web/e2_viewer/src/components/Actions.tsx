/** This component provides a form for dispatching actions to the document. It also registers all the available actions. */

import { DocumentContext } from "../Contexts"; 
import { useContext, useEffect } from "react";
import * as grepActions from '../grep/grep.actions.ts'  
import * as prepActions from '../prep/prep.actions.ts'
import * as shapeActions from '../shape/shape.actions.ts'  

/** these strings are displayed in the dropdown list of the actions input form */
const actionSuggestions = [
    '{"type":"Gfx::ping", "payload": {} }',
    '{"type":"Gfx::addPoint", "payload":{"size":5, "position":[1,1,1], "color":"red"}}',
    '{"type":"Gfx::addLine", "payload":{"start":[-2,0,0], "end":[2,0,0], "color":"blue"}}',
    '{"type":"Gfx::addPolyline", "payload":{"positions":[[0.5,1,0],[-0.5,1,0],[-0.5,-1,0],[0.5,-1,0],[0.5,1,0]], "color":"blue"}}',
    '{"type":"Gfx::addPolyline", "payload":{"positions":[[0.5,1,1],[-0.5,1,1],[-0.5,-1,1],[0.5,-1,1],[0.5,1,1]], "color":"blue"}}',
    '{"type":"Gfx::addPolyline", "payload":{"positions":[[0.5,1,2],[-0.5,1,2],[-0.5,-1,2],[0.5,-1,2],[0.5,1,2]], "color":"blue"}}',
    '{"type":"Gfx::addPlane", "payload":{"width":3,"height":2, "color":"red"}}',
    '{"type":"Gfx::addPlane", "payload":{"width":10,"height":10, "texture":{"width":4,"height":4,"data":[0,255,0,255,15,240,63,255,31,224,127,255,47,208,191,255,63,192,0,255,79,176,63,255,95,160,127,255,111,144,191,255,127,128,0,255,143,112,63,255,159,96,127,255,175,80,191,255,191,64,0,255,207,48,63,255,223,32,127,255,239,16,191,255]}}}',
    '{"type":"Gfx::addSphere", "payload":{"radius":0.75, "color":"green"}}',
    '{"type":"Gfx::addBlock", "payload":{"width":1,"height":2,"depth":3, "color":"red"}}',
    '{"type":"Gfx::addProfile", "payload":{"paths":[[[0,0],[1,0]],[[1,0],[1,1]],[[1,1],[0,1]],[[0,1],[0,0]]], "color":"red"}}',
    '{"type":"Gfx::addProductItem", "payload":{"displayName":"cell[0] (plane)", "pathName":"Unnamed (shape)/workplanes/body[0] (workplane)/cell[0]"}}',
    '{"type":"Modeller::ping", "payload": {} }',
    '{"type":"Sketches::addRectangle", "payload":{"lowerLeft":{"x":-3, "y":-2, "z":0}, "upperRight":{"x":3, "y":2, "z":0}}}',
    '{"type":"Sketches::addRoundRect", "payload":{"lowerLeft":{"x":-3, "y":-2, "z":0}, "upperRight":{"x":3, "y":2, "z":0}, "cornerRadius":0.2}}',
    '{"type":"Profiles::addRectangle", "payload":{"lowerLeft":{"x":-3, "y":-2, "z":0}, "upperRight":{"x":3, "y":2, "z":0}}}',
    '{"type":"Profiles::addRoundRect", "payload":{"lowerLeft":{"x":-3, "y":-2, "z":0}, "upperRight":{"x":3, "y":2, "z":0}, "cornerRadius":0.2}}',
    '{"type":"Objects::addEmptyObject", "payload":{}}',
    '{"type":"Objects::addSphereObject", "payload":{"radius":1.5}}',
    '{"type":"Objects::addBlockObject", "payload":{"width":3, "height":2, "depth":5}}',
    '{"type":"Objects::addInfiniteRectangle", "payload":{"lowerLeft":{"x":-3, "y":-2, "z":0}, "upperRight":{"x":3, "y":2, "z":0}}}',
    '{"type":"Objects::addCappedRectangle", "payload":{"lowerLeft":{"x":-3, "y":-2, "z":0}, "upperRight":{"x":3, "y":2, "z":0}, "depth":5}}',
    '{"type":"Objects::addExtrudedRectangle", "payload":{"lowerLeft":{"x":-3, "y":-2, "z":0}, "upperRight":{"x":3, "y":2, "z":0}, "depth":5}}'
  ];
  
  export function Actions() {
    const document = useContext(DocumentContext);

    useEffect(() => {

        // Register actions with the document. These actions are the only valid way for the application to manipulate the drawlist.
        console.log("Registering actions");

        document.registerActionFunction(grepActions.pingActionDef);   // register ping action separately so that the test mock works!

        for (const actionDef of Object.values(grepActions)) {
            if (typeof actionDef === "object" && "type" in actionDef && "function" in actionDef) {
                document.registerActionFunction(actionDef);
            }
        }

        for (const actionDef of Object.values(prepActions)) {
            if (typeof actionDef === "object" && "type" in actionDef && "function" in actionDef) {
                document.registerActionFunction(actionDef);
            }
        }
          
        for (const actionDef of Object.values(shapeActions)) {
            if (typeof actionDef === "object" && "type" in actionDef && "function" in actionDef) {
                document.registerActionFunction(actionDef);
            }
        }
        
    }, []);

    /** This method takes a JSON string from an input form, converts it to an object representing an action, and dispatches the action to 
     * the global document (which is passed in as context) 
     * The format for an action is {"type": <string>, "payload": <any valid json>}. 
     * Any errors are caught here and logged, and then execution continues normally.
     */
    function dispatchAction(formData: FormData) {
        const actionText = formData.get("actionInput") as string;
        if (actionText.trim() === "") {
            console.log("Blank line - no action dispatched.");
            return;
        }
        try {
            console.log(`Dispatching action: ${actionText}`);
            const action = JSON.parse(actionText);
            document.dispatchAction(action);
        } catch (e: unknown) {
            let errorMessage = "";
            if (typeof e === "string") {
                errorMessage = e;
            }
            else if (e instanceof Error) {
                errorMessage = e.message;
            }
            else {
                errorMessage = "Unknown error.";
            }
            console.error(`Error dispatching action: ${actionText}\n${errorMessage}`);
        }
    }

    function handleSubmit(event: React.FormEvent<HTMLFormElement>) {
        event.preventDefault();
        const formData = new FormData(event.currentTarget);
        dispatchAction(formData);
        event.currentTarget.reset();
    }

    return (
      <>
        <form onSubmit={handleSubmit}>
          <input name="actionInput" type="text" className="flex w-full input" placeholder="Next action?" list="suggestions" />
          <datalist id="suggestions">
            {actionSuggestions.map((suggestion, index) => (
              <option key={index} value={suggestion}></option>
            ))}
          </datalist>
        </form>   
      </>
    )
  }