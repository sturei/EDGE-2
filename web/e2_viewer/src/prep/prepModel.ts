/** a PRepModel describes the Product Structure
 */

import { Model } from '../document/model';
import {type IProductItem} from '../prep/item';


// TODO: perhaps initialise the root item to "/" at position 0, then no need to store root item imdex.
export class PRepModel extends Model {
    /** List of all product items. Each item lists its children. 
     * By convention, the root node is at index 0 and its pathname does not begin with a slash */
    private m_items: IProductItem[] = [];    

    constructor() {
        super();
    }

    numItems(): number {
        return this.m_items.length;
    }

    item(index: number): IProductItem {
        const item = this.m_items[index];
        if (!item) {
            throw new Error(`PRepModel.item: no item at index ${index}`);
        }
        return item;
    }

    items(): IProductItem[] {
        return this.m_items;
    }

    addItem(item: IProductItem): number {
        // Prevent adding duplicate items
        const existingItem = this.findItem(item.pathName);
        if (existingItem) {
            console.log(`PRepModel.addItem: item with pathName='${item.pathName}' already exists. Skipping add.`);
            return this.m_items.indexOf(existingItem);
        }

        // Check that the path start matches the root item if it exists
        if (this.m_items.length > 0) {
            const rootItem = this.m_items[0];
            if (!item.pathName.startsWith(rootItem.pathName)) {
                throw new Error(`PRepModel.addItem: item pathName='${item.pathName}' does not start with root item pathName='${rootItem.pathName}'`);
            }
        }

        // Ensure parent items exist.
        this.ensureParentItems(item);

        // Add the item
        const index = this.m_items.push(item) - 1;
        console.log(`PRepModel.addItem: added new item ${item.displayName} at ${item.pathName}, index=${index}`);

        // Add the item as a child of its parent
        const parentItem = this.findParentItem(item);
        if (parentItem) {
            if (!parentItem.childIndices) {
                parentItem.childIndices = [];
            }
            parentItem.childIndices.push(index);
            console.log(`  Added as child of parent item=${parentItem.pathName}`);
        } 
        return index;
    }

    findItem(pathName: string): IProductItem | null {
        for (const item of this.m_items) {
            if (item.pathName === pathName) {
                return item;
            }
        }
        return null;
    }

    clearItems(): void {
        this.m_items = [];
        console.log("PRepModel.clearItems: cleared all items.");
    }

    private findParentItem(item: IProductItem): IProductItem | null {
        const parentPathName = item.pathName.substring(0, item.pathName.lastIndexOf('/'));
        for (const potentialParent of this.m_items) {
            if (potentialParent.pathName === parentPathName) {
                return potentialParent;
            }
        }
        return null;
    }

    // Handy utility - auto-create the parents if they don't exist.
    private ensureParentItems(item: IProductItem): void {
        return this.ensureParentItemsRecursive(item.pathName);
    }

    private ensureParentItemsRecursive(pathName: string): void {
        const lastSlashIndex = pathName.lastIndexOf('/');

        if (lastSlashIndex <= 0) {
            return; // We're at the root. No parent to process
        }

        const parentPath = pathName.substring(0, lastSlashIndex);
    
        // Recursively ensure parent's parents exist first
        this.ensureParentItemsRecursive(parentPath);

        // Create parent if it doesn't exist
        if (!this.findItem(parentPath)) {
            // use the path segment as the display name for want of anything better. TODO: add a "displayPathname" concept
            const parentDisplayName = parentPath.substring(parentPath.lastIndexOf('/') + 1);

            const parentItem: IProductItem = {
                displayName: parentDisplayName,
                pathName: parentPath,
                active: true
            };
            this.addItem(parentItem);
            console.log(`PRepModel.ensureParentItems: added missing parent item='${parentItem.pathName}'`);
        }
    }

    toString(): string {
        return `PRepModel(numItems=${this.numItems()})`;
    }
}

