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
    '{"type":"Gfx::addPolyline", "payload":{"positions":[[1.5,1,0],[-1.5,1,0],[-1.5,-1,0],[1.5,-1,0],[1.5,1,0]], "color":"blue"}}',
    '{"type":"Gfx::addPolyline", "payload":{"positions":[[1.5,1,1],[-1.5,1,1],[-1.5,-1,1],[1.5,-1,1],[1.5,1,1]], "color":"blue"}}',
    '{"type":"Gfx::addPolyline", "payload":{"positions":[[1.5,1,2],[-1.5,1,2],[-1.5,-1,2],[1.5,-1,2],[1.5,1,2]], "color":"blue"}}',
    '{"type":"Gfx::addPlane", "payload":{"width":3,"height":2, "color":"red"}}',
    '{"type":"Gfx::addPlane", "payload":{"width":10,"height":10, "texture":{"width":4,"height":4,"data":[0,255,0,255,15,240,63,255,31,224,127,255,47,208,191,255,63,192,0,255,79,176,63,255,95,160,127,255,111,144,191,255,127,128,0,255,143,112,63,255,159,96,127,255,175,80,191,255,191,64,0,255,207,48,63,255,223,32,127,255,239,16,191,255]}}}',
    '{"type":"Gfx::addSphere", "payload":{"radius":1.5, "color":"green"}}',
    '{"type":"Gfx::addBlock", "payload":{"width":2,"height":3,"depth":5, "color":"red"}}',
    '{"type":"Gfx::addCylinder", "payload":{"radius":1.0,"depth":6.0, "color":"yellow"}}',
    '{"type":"Gfx::addProfile", "payload":{"paths":[[[0,0],[1,0]],[[1,0],[1,1]],[[1,1],[0,1]],[[0,1],[0,0]]], "color":"red"}}',
    '{"type":"Gfx::addContour", "payload":{"paths":[[[0,0],[1,0]],[[1,0],[1,1]],[[1,1],[0,1]],[[0,1],[0,0]]], "color":"red"}}',
    '{"type":"Gfx::addSdfNode", "payload":{"type":"intersection", "pathName":"objects"}}',
    '{"type":"Gfx::addSdfNode", "payload":{"type":"union", "pathName":"objects/blanks"}}',
    '{"type":"Gfx::addSdfNode", "payload":{"type":"complement", "pathName":"objects/tools"}}',
    '{"type":"Gfx::addSdfNode", "payload":{"type":"union", "pathName":"objects/tools/tools"}}',
    '{"type":"Gfx::addSdfNode", "payload":{"type":"block", "width":3.0, "height":2.0, "depth":5.0, "pathName":"objects/blanks/block1"}}',
    '{"type":"Gfx::addSdfNode", "payload":{"type":"cylinder", "radius":1.0, "depth":6.0, "pathName":"objects/blanks/cylinder1"}}',
    '{"type":"Gfx::addSdfNode", "payload":{"type":"translation", "translation":[0.5,0,0], "pathName":"objects/blanks/translation"}}',
    '{"type":"Gfx::addSdfNode", "payload":{"type":"cylinder", "radius":1.0, "depth":6.0, "pathName":"objects/blanks/translation/cylinder2"}}',
    '{"type":"Gfx::addSdfNode", "payload":{"type":"rotation", "rotation":[1.57079632679,0,0], "pathName":"objects/blanks/rotation"}}',
    '{"type":"Gfx::addSdfNode", "payload":{"type":"cylinder", "radius":1.0, "depth":6.0, "pathName":"objects/blanks/rotation/cylinder3"}}',
    '{"type":"Gfx::addSdfNode", "payload":{"type":"extrusion", "depth":6.0, "pathName":"objects/blanks/extrusion1"}}',
    '{"type":"Gfx::addSdfNode", "payload":{"type":"circle", "radius":1.0, "pathName":"objects/blanks/extrusion1/circle1"}}',
    '{"type":"Gfx::addSdfNode", "payload":{"type":"rectangle", "width":3.0, "height":2.0, "pathName":"objects/blanks/extrusion1/rectangle1"}}',
    '{"type":"Gfx::addSdfNode", "payload":{"type":"roundRect", "width":3.0, "height":2.0, "cornerRadius":0.2, "pathName":"objects/blanks/extrusion1/roundRect1"}}',
    '{"type":"Gfx::addSdfNode", "payload":{"type":"profile", "paths":[[[0,0],[1,0]],[[1,0],[1,1]],[[1,1],[0,1]],[[0,1],[0,0]]], "pathName":"objects/blanks/extrusion1/profile1"}}',
    '{"type":"Gfx::addSdfNode", "payload":{"type":"sphere", "radius":1.5, "pathName":"objects/tools/tools/sphere1"}}',
    '{"type":"Gfx::addSdfNode", "payload":{"type":"halfSpace", "planePosition":[0,0,0], "planeNormal":[0,1,0], "pathName":"objects/tools/tools/halfSpace1"}}',
    '{"type":"Gfx::updateSdfScene", "payload":{}}',
    '{"type":"Gfx::addProductItem", "payload":{"displayName":"cell[0] (plane)", "pathName":"Unnamed (shape)/workplanes/body[0] (workplane)/cell[0]"}}',
    '{"type":"Modeller::ping", "payload": {} }',
    '{"type":"Sketches::addRectangle", "payload":{"lowerLeft":{"x":-3, "y":-2, "z":0}, "upperRight":{"x":3, "y":2, "z":0}}}',
    '{"type":"Sketches::addRoundRect", "payload":{"lowerLeft":{"x":-3, "y":-2, "z":0}, "upperRight":{"x":3, "y":2, "z":0}, "cornerRadius":0.2}}',
    '{"type":"Profiles::addRectangle", "payload":{"lowerLeft":{"x":-3, "y":-2, "z":0}, "upperRight":{"x":3, "y":2, "z":0}}}',
    '{"type":"Profiles::addRoundRect", "payload":{"lowerLeft":{"x":-3, "y":-2, "z":0}, "upperRight":{"x":3, "y":2, "z":0}, "cornerRadius":0.2}}',
    '{"type":"Objects::addEmptyObject", "payload":{}}',
    '{"type":"Objects::addSphereObject", "payload":{"radius":1.5}}',
    '{"type":"Objects::addBlockObject", "payload":{"width":3, "height":2, "depth":5}}',
    '{"type":"Objects::addCylinderObject", "payload":{"radius":1.0, "depth":6.0}}',
    '{"type":"Objects::addInfiniteRectangle", "payload":{"lowerLeft":{"x":-3, "y":-2, "z":0}, "upperRight":{"x":3, "y":2, "z":0}}}',
    '{"type":"Objects::addCappedRectangle", "payload":{"lowerLeft":{"x":-3, "y":-2, "z":0}, "upperRight":{"x":3, "y":2, "z":0}, "depth":5}}',
    '{"type":"Objects::addExtrudedRectangle", "payload":{"lowerLeft":{"x":-3, "y":-2, "z":0}, "upperRight":{"x":3, "y":2, "z":0}, "depth":5}}',
    '{"type":"Features::addPrimitive", "payload":{"primitiveType":"block", "width":3, "height":2, "depth":1, "pathName":"shape/features/block1"}}',
    '{"type":"Features::addPrimitive", "payload":{"primitiveType":"block", "featureEffect":"subtract", "width":1, "height":1, "depth":2, "pathName":"shape/features/cut1"}}',
    '{"type":"Features::addPrimitive", "payload":{"primitiveType" : "sphere", "radius":1.5, "pathName":"shape/features/sphere1"}}',
    '{"type":"Features::addPrimitive", "payload":{"primitiveType" : "cylinder", "radius":1, "depth":3, "pathName":"shape/features/cylinder1"}}',
    '{"type":"Features::addPrimitive2D", "payload":{"primitiveType":"rectangle", "width":3, "height":2, "pathName":"shape/profiles/rectangle1"}}',
    '{"type":"Features::addPrimitive2D", "payload":{"primitiveType":"circle", "radius":1.0, "pathName":"shape/profiles/circle1"}}',
    '{"type":"Features::addPrimitive2D", "payload":{"primitiveType":"roundRect", "width":3, "height":2, "cornerRadius":0.2, "pathName":"shape/profiles/roundRect1"}}',
    '{"type":"Features::addExtrusion", "payload":{"depth":3, "profilePathName":"shape/profiles/roundRect1", "pathName":"shape/features/extrusion1"}}'
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