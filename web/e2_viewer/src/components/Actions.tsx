/** This component provides a form for dispatching actions to the document. It also registers all the available actions. */

import { DocumentContext } from "../Contexts"; 
import { useContext, useEffect } from "react";
import * as grepActions from '../grep/grep.actions.ts'  

/** these strings are displayed in the dropdown list of the actions input form */
const actionSuggestions = [
    '{"type": "ping", "payload": {} }',
    '{"type":"addGPoint", "payload":{"size":0.2}}',
    '{"type":"addGLine", "payload":{"length":3}}',
    '{"type":"addGPlane", "payload":{"width":3,"height":2}}',
    '{"type":"addGSphere", "payload":{"radius":3}}',
    '{"type":"addGBlock", "payload":{"width":1,"height":2,"depth":3}}'
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

    }, []);

    /** when the user enters text into the actions input form, this function dispatches it to the document */
    function dispatchAction(formData: FormData) {
        const actionText = formData.get("actionInput") as string;
        try {
            console.log(`Dispatching action: ${actionText}`);
            const action = JSON.parse(actionText);
            const dispatched = document.dispatchAction(action);
            if (!dispatched) {
                console.error(`Action type "${action.type}" not recognized. Was it registered correctly?`);
            }
        } catch (error) {
            console.error("Failed to parse action input as JSON:", error);
        }
    }

    return (
      <>
        <form action={dispatchAction}>
          <input name="actionInput" type="text" className="input" placeholder="Next action?" list="suggestions" />
          <datalist id="suggestions">
            {actionSuggestions.map((suggestion, index) => (
              <option key={index} value={suggestion}></option>
            ))}
          </datalist>
        </form>   
      </>
    )
  }