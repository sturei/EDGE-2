import { Document } from '../document/document';
import { Model } from '../document/model';
import { PRepModel } from '../prep/prepModel';
import { type IProductItem } from '../prep/item';

function addProductItem(doc: Document, payload: any): void {

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

export const addProductItemActionDef = { type: "Gfx::addProductItem", function: addProductItem };
