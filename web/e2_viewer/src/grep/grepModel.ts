/** a GRepModel consists of a list of GItems to be drawn. 
 * 
 * Implementation notes:
 * Perhaps add an index by name.
 */


import { Model } from '../document/model';
import {type IDrawable} from './drawables/drawable';
import { type IShaderNode } from './nodes/shaderNode';

export class GRepModel extends Model {
    private m_drawList: IDrawable[] = [];
    private m_sdfScene: IShaderNode[] = []; // SDF scene as a list of shader nodes, root node at index 0.
    private m_sdfGuid: string = "Initial State";  // GUID gets updated to indicate that the SDF scene should be updated;

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

    clearShaderNodes(): void {
        this.m_sdfScene = [];
        console.log("GRepModel.clearShaderNodes: cleared all shader nodes.");
    }

    addShaderNode(node: IShaderNode): number {
        // Prevent adding duplicate nodes
        const existingNode = this.findNode(node.pathName);
        if (existingNode) {
            console.log(`PRepModel.addNode: node with pathName='${node.pathName}' already exists. Skipping add.`);
            if (existingNode.type !== node.type) {
                // update the existing node type, to support making tools visible by toggling between 'complement' and 'union' type
                console.log(`  updating existing node type='${existingNode.type}' to match new node type='${node.type}'`);
                existingNode.type = node.type;
            }
            return this.m_sdfScene.indexOf(existingNode);
        }

        // Check that the path start matches the root item if it exists
        if (this.m_sdfScene.length > 0) {
            const rootNode = this.m_sdfScene[0];
            if (!node.pathName.startsWith(rootNode.pathName)) {
                throw new Error(`GRepModel.addShaderNode: node pathName='${node.pathName}' does not start with root node pathName='${rootNode.pathName}'`);
            }
        }

        // Check that parent node exists.
        const parentNode = this.findParentNode(node);
        if (this.m_sdfScene.length > 0 && !parentNode) {
            throw new Error(`GRepModel.addShaderNode: parent node for pathName='${node.pathName}' does not exist`);
        }

        // Add the node
        const index = this.m_sdfScene.push(node) - 1;

        // Add the node as a child of its parent
        if (parentNode) {
            if (!parentNode.childIndices) {
                parentNode.childIndices = [];
            }
            parentNode.childIndices.push(index);
            console.log(`  Added as child of parent node=${parentNode.pathName}`);
        }

        console.log(`GRepModel.addShaderNode: node=${node.pathName}, type=${node.type}, index=${index}`);
        return index;
    }

    sdfGuid(): string {
        return this.m_sdfGuid;
    }

    setSdfGuid(guid: string): void {
        this.m_sdfGuid = guid;
    }

    private findNode(pathName: string): IShaderNode | null {
        for (const node of this.m_sdfScene) {
            if (node.pathName === pathName) {
                return node;
            }
        }
        return null;
    }

    private findParentNode(node: IShaderNode): IShaderNode | null {
        const parentPathName = node.pathName.substring(0, node.pathName.lastIndexOf('/'));
        for (const potentialParent of this.m_sdfScene) {
            if (potentialParent.pathName === parentPathName) {
                return potentialParent;
            }
        }
        return null;
    }

    toString(): string {
        return `GrepModel(numDrawables=${this.numDrawables()}, numShaderNodes=${this.numShaderNodes()})`;
    }
}

