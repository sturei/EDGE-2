/** a GRepModel consists of a list of GItems to be drawn. 
 * 
 * Implementation notes:
 * Perhaps add an index by name.
 */


import {Model} from '../document/model';
import {GItem} from './gitem';

export class GRepModel extends Model {
    private m_drawList: GItem[] = [];

    constructor() {
        super();
    }

    numGItems(): number {
        return this.m_drawList.length;
    }

    gItem(index: number): GItem | undefined {
        return this.m_drawList[index];
    }

    addGItem(item: GItem): number {
        return this.m_drawList.push(item) - 1;
    }

    removeGItem(index: number): void {
        this.m_drawList.splice(index, 1);
    }

    toString(): string {
        return `GrepModel(numGItems=${this.numGItems()})`;
    }
}

