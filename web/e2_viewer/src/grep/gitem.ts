export abstract class GItem {
    constructor() {}
    }

export class GPoint extends GItem {
    private m_size: number;  // size (radius) of the point
    constructor(size: number) { 
        super();
        this.m_size = size;
    }
    size(): number { return this.m_size; }
    toString(): string {
        return `GPoint(${this.m_size})`;
    }
}

export class GLine extends GItem {
    private m_length: number;  // length of the line
    constructor(length: number) { 
        super();
        this.m_length = length;
    }
    length(): number { return this.m_length; }
    toString(): string {
        return `GLine(${this.m_length})`;
    }

}

export class GPlane extends GItem {
    private m_width: number;   // length in x direction
    private m_height: number;  // length in y direction
    constructor(width: number, height: number) { 
        super();
        this.m_width = width;
        this.m_height = height;
    }
    width(): number { return this.m_width; }
    height(): number { return this.m_height; }
    toString(): string {
        return `GPlane(${this.m_width}, ${this.m_height})`;
    }
}

export class GSphere extends GItem {
    private m_radius: number;
    constructor(radius: number) { 
        super();
        this.m_radius = radius;
    }
    radius(): number { return this.m_radius; }
    toString(): string {
        return `GSphere(${this.m_radius})`;
    }
}

export class GBlock extends GItem {
    private m_width: number;   // length in x direction
    private m_height: number;  // length in y direction
    private m_depth: number;   // length in z direction
    constructor(width: number, height: number, depth: number) { 
        super();
        this.m_width = width;
        this.m_height = height;
        this.m_depth = depth;
    }
    width(): number { return this.m_width; }
    height(): number { return this.m_height; }
    depth(): number { return this.m_depth; }
    toString(): string {
        return `GBlock(${this.m_width}, ${this.m_height}, ${this.m_depth})`;
    }
}
