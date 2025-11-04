/** a GRepModel consists of a list of GItems to be drawn. 
 * 
 * Implementation notes:
 * Perhaps add an index by name.
 */


import { Model } from '../document/model';
import {type IDrawable} from '../grep/drawable';

export class GRepModel extends Model {
    private m_drawList: IDrawable[] = [];

    constructor() {
        super();
    }

    numDrawables(): number {
        return this.m_drawList.length;
    }

    drawable(index: number): IDrawable {
        const item = this.m_drawList[index];
        if (!item) {
            throw new Error(`GRepModel.drawable: no drawable at index ${index}`);
        }
        return item;
    }

    drawlist(): IDrawable[] {
        return this.m_drawList;
    }

    addDrawable(item: IDrawable): number {
        const index = this.m_drawList.push(item) - 1;

        console.log(`GRepModel.addDrawable: item=${item.geometry?.type??"unknown"}, index=${index}`);
        return index;
    }

    removeDrawable(index: number): void {
        this.m_drawList.splice(index, 1);
    }

    toString(): string {
        return `GrepModel(numDrawables=${this.numDrawables()})`;
    }
}

