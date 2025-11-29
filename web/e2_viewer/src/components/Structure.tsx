/** This component sets up the Product Structure. 
 * It also creates the Structure store and Model within the Document, and uses that to update structure dynamically in response to 
 * state changes in the Structure store.
 */
  
import { useContext, useEffect, useState } from "react";
import { Store } from '../document/store.ts' 
import { DocumentContext } from '../Contexts.ts';
import { PRepModel } from "../prep/prepModel.ts";
import { type IProductItem } from "../prep/item.ts";

interface IStructureReactState {
    // just a flat list of all items. Hierarchy is represented by childIndices within each item. Root is at index 0.
    items: IProductItem[];
}

function TreeNode({itemIndex, items} : {itemIndex: number, items: IProductItem[]}) {
    const item = items[itemIndex];
    return (
        <li className="my-0.5">
            <div className="flex items-center py-1 px-2 text-gray-700 hover:bg-gray-50 rounded-sm transition-colors duration-150">
                <span className="text-xs font-medium">{item.displayName}</span>
            </div>
            {item.childIndices && item.childIndices.length > 0 && (
                <ul className="ml-4 border-l border-gray-200 pl-2">
                    {item.childIndices.map((childIndex) => (
                        <TreeNode key={childIndex} itemIndex={childIndex} items={items} />
                    ))}
                </ul>
            )}
        </li>
    );
}

const emptyStructure:IStructureReactState = {items: []}

export function Structure(){
    const document = useContext(DocumentContext);
    const [reactState, setReactState] = useState(emptyStructure);

    /** Set up the Structure Model and Store */
    useEffect(() => {
        console.log(" Setting up the Structure's Model and Store");
        const structureModel = new PRepModel();
        const structureStore = new Store(structureModel, postStateChangeCallback);
        document.addStore('structure', structureStore);
    }, []);

    /** Update the React state whenever the Structure model changes */
    function postStateChangeCallback() {
        console.log("Structure model state changed. Updating react state.");
        const structureStore = document.getStore('structure');
        const structureModel = structureStore.getModel() as PRepModel;
        const newState:IStructureReactState = { ...reactState };
        newState.items = structureModel.items().slice(); // Copy to force React to see it as changed in case elements have been added/removed/changed. Alternatives exist e.g. make it immutable etc.
        console.log(" New state:", newState);
        setReactState(newState);
    }

    // Render the root item and its children recursively.
    return (
        <>
            {reactState.items.length > 0 &&
                <ul><TreeNode itemIndex={0} items={reactState.items} /></ul>
            }  
        </>
    )
}
