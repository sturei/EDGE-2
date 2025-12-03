/** a GRepModel consists of a list of GItems to be drawn. 
 * 
 * Implementation notes:
 * Perhaps add an index by name.
 */


import { Model } from '../document/model';
import {type IDrawable} from '../grep/drawable';
import { type IShaderNode } from '../grep/shaderNode';

export class GRepModel extends Model {
    private m_drawList: IDrawable[] = [];
    private m_sdfScene: IShaderNode[] = []; // SDF scene as a list of shader nodes, root node at index 0.

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

    numShaderNodes(): number {
        return this.m_sdfScene.length;
    }

    shaderNode(index: number): IShaderNode {
        const node = this.m_sdfScene[index];
        if (!node) {
            throw new Error(`GRepModel.shaderNode: no shader node at index ${index}`);
        }
        return node;
    }

    shaderNodes(): IShaderNode[] {
        return this.m_sdfScene;
    }

    addShaderNode(node: IShaderNode): number {
        const index = this.m_sdfScene.push(node) - 1;

        console.log(`GRepModel.addShaderNode: node=${node.pathName}, type=${node.type}, index=${index}`);
        return index;
    }  

    toString(): string {
        return `GrepModel(numDrawables=${this.numDrawables()}, numShaderNodes=${this.numShaderNodes()})`;
    }
}

