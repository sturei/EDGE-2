/**
 * Definition for Product Item
 */

export interface IProductItem {
    displayName: string;          // name to display for the item
    pathName: string;             // path within the product structure. Must be unique within the structure.
    childIndices?: number[];        // indices of child items within the structure's item list
    active: boolean;              // whether the item is active or not. Inactive items are ignored in display and processing.
}
