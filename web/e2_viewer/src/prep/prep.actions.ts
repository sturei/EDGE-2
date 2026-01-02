import { Document } from '../document/document';
import { Model } from '../document/model';
import { PRepModel } from '../prep/prepModel';
import { type IProductItem } from '../prep/item';

// Adds a product item to the Product Structure tree.
// It is recommended to build the tree top-down.
// Changes are reflected in the UI immediately.
async function addProductItem(doc: Document, payload: any): Promise<void> {

    const displayName = payload.displayName ?? "Unnamed Item";
    const pathName = payload.pathName ?? "Error: no pathName provided";
    
    const store = doc.getStore("structure");
    store.changeState((model: Model) => {
        let structureModel = model as PRepModel; 
        const item: IProductItem = {
            displayName: displayName,
            pathName: pathName,
            active: true           
        };
        structureModel.addItem(item);
    });
    console.log(`Added Product Item: ${displayName}`);          //--- DEBUG ---
}

// Clears all product items from the Product Structure tree.
async function clearProductItems(doc: Document, _payload: any): Promise<void> {
    const store = doc.getStore("structure");
    store.changeState((model: Model) => {
        let structureModel = model as PRepModel; 
        structureModel.clearItems();
    });
    console.log("Cleared all Product Items");          //--- DEBUG ---
}

export const addProductItemActionDef = { type: "Structure::addProductItem", function: addProductItem };
export const clearProductItemsActionDef = { type: "Structure::clearProductItems", function: clearProductItems };  
