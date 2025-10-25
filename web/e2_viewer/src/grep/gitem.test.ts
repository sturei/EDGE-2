import { expect, test } from 'vitest'
import { GPoint, GLine, GPlane, GSphere, GBlock } from './gitem.js'

test ('GPoint', () => {
    const point = new GPoint(5);
    expect(point.size()).toBe(5);
    expect(point.toString()).toBe('GPoint(5)');
});

test('GLine', () => {
    const line = new GLine(10);
    expect(line.length()).toBe(10);
    expect(line.toString()).toBe('GLine(10)');
});

test('GPlane', () => {
    const plane = new GPlane(10, 20);
    expect(plane.width()).toBe(10);
    expect(plane.height()).toBe(20);
    expect(plane.toString()).toBe('GPlane(10, 20)');
});

test('GSphere', () => {
    const sphere = new GSphere(15);
    expect(sphere.radius()).toBe(15);
    expect(sphere.toString()).toBe('GSphere(15)');
});

test('GBlock', () => {
    const block = new GBlock(5, 10, 15);
    expect(block.width()).toBe(5);
    expect(block.height()).toBe(10);
    expect(block.depth()).toBe(15);
    expect(block.toString()).toBe('GBlock(5, 10, 15)');
});

