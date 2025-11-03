/** This component provides a form for dispatching actions to the document. It also registers all the available actions. */

import { DocumentContext } from "../Contexts"; 
import { useContext, useEffect } from "react";
import * as grepActions from '../grep/grep.actions.ts'  
import * as brepActions from '../brep/brep.actions.ts'  

/** these strings are displayed in the dropdown list of the actions input form */
const actionSuggestions = [
    '{"type": "ping", "payload": {} }',
    '{"type": "pingModeller", "payload": {} }',
    '{"type":"addGPoint", "payload":{"size":0.2}}',
    '{"type":"addGLine", "payload":{"length":4}}',
    '{"type":"addGPlane", "payload":{"width":3,"height":2}}',
    '{"type":"addGSphere", "payload":{"radius":0.75}}',
    '{"type":"addGBlock", "payload":{"width":1,"height":2,"depth":3}}',
    '{"type":"addGShape", "payload":{"points":[0,0,1,0,1,1,0,1,0,0]}}',
    '{"type":"addSheetRectangle", "payload":{"lowerLeft":{"x":0, "y":0, "z":0}, "upperRight":{"x":3, "y":2, "z":0}}}'
  ];
  
  export function Actions() {
    const document = useContext(DocumentContext);

    useEffect(() => {

        // Register actions with the document. These actions are the only valid way for the application to manipulate the drawlist.
        console.log("Registering actions");

        document.registerActionFunction(grepActions.pingActionDef);
        document.registerActionFunction(grepActions.addGPointActionDef);
        document.registerActionFunction(grepActions.addGLineActionDef);
        document.registerActionFunction(grepActions.addGPlaneActionDef);
        document.registerActionFunction(grepActions.addGSphereActionDef);
        document.registerActionFunction(grepActions.addGBlockActionDef);
        document.registerActionFunction(grepActions.addGShapeActionDef);

        document.registerActionFunction(brepActions.pingModellerActionDef);
        document.registerActionFunction(brepActions.addSheetRectangleActionDef);

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
          <input name="actionInput" type="text" className="flex w-4/5 input" placeholder="Next action?" list="suggestions" />
          <datalist id="suggestions">
            {actionSuggestions.map((suggestion, index) => (
              <option key={index} value={suggestion}></option>
            ))}
          </datalist>
        </form>   
      </>
    )
  }