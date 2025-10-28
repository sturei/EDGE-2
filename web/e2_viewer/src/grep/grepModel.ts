/** a GRepModel consists of a list of GItems to be drawn. 
 * 
 * Implementation notes:
 * Perhaps add an index by name.
 */


import {Model} from '../document/model';
import {type IGItem} from './gitem';

export class GRepModel extends Model {
    private m_drawList: IGItem[] = [];

    constructor() {
        super();
    }

    numGItems(): number {
        return this.m_drawList.length;
    }

    gItem(index: number): IGItem | undefined {
        return this.m_drawList[index];
    }

    addGItem(item: IGItem): number {
        const index = this.m_drawList.push(item) - 1;
        console.log(`GRepModel.addGItem: item=${item}, index=${index}`);
        return index;
    }

    removeGItem(index: number): void {
        this.m_drawList.splice(index, 1);
    }

    toString(): string {
        return `GrepModel(numGItems=${this.numGItems()})`;
    }
}

