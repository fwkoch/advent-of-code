import * as fs from 'fs';

function getStart(filename: string): [string[], number[]] {
  const input: string[] = fs.readFileSync(filename, 'utf8').split('\n');
  const output: string[] = [];
  const dims: number[] = [input.length, input[0].length, 1];
  input.forEach((line: string): void => {
    line.split('').forEach((state: string): void => {
      output.push(state);
    });
  });
  return [output, dims];
}

function getLinearIndex(index: number[], dims: number[]): number {
  let linearIndex: number = 0;
  let dimProduct: number;
  for (let i = 0; i < index.length; i += 1) {
    dimProduct = dims.slice(0, i).reduce((a: number, b: number): number => a * b, 1);
    linearIndex += index[i] * dimProduct;
  }
  return linearIndex;
}

function indexPermutations(shape: [number, number]): number[][] {
  const permutations: number[][] = [];
  if (shape[1] === 1) {
    for (let i: number = 0; i < shape[0]; i += 1) {
      permutations.push([i]);
    }
    return permutations;
  }
  indexPermutations([shape[0], shape[1] - 1]).forEach((perm: number[]): void => {
    for (let i: number = 0; i < shape[0]; i += 1) {
      permutations.push([i].concat(perm));
    }
  });
  return permutations;
}

function variablePermutations(shapes: number[]): number[][] {
  const allPerms: number[][] = indexPermutations([Math.max(...shapes), shapes.length]);
  const permutations: number[][] = [];
  let valid: boolean;
  allPerms.forEach((perm: number[]): void => {
    valid = true;
    for (let i: number = 0; i < shapes.length; i += 1) {
      if (perm[i] >= shapes[i]) {
        valid = false;
      }
    }
    if (valid) {
      permutations.push(perm);
    }
  });
  return permutations;
}

function getRange(start: number, stop: number): number[] {
  const output: number[] = [];
  for (let i: number = start; i <= stop; i += 1) {
    output.push(i);
  }
  return output;
}

function status(input: string[], index: number[], dims: number[]) {
  const neighbors: string[] = [];
  let current: string = '.';
  const ranges: number[][] = [];
  let thisIndex: number[];
  for (let i: number = 0; i < index.length; i += 1) {
    ranges.push(getRange(Math.max(0, index[i] - 1), Math.min(dims[i] - 1, index[i] + 1)));
  }
  const permutations: number[][] = variablePermutations(
    ranges.map((val: number[]): number => val.length),
  );
  permutations.forEach((perm: number[]): void => {
    thisIndex = perm.map((val: number, i: number): number => ranges[i][val]);
    if (thisIndex.filter((val: number, i: number): boolean => val !== index[i]).length !== 0) {
      neighbors.push(input[getLinearIndex(thisIndex, dims)]);
    } else {
      current = input[getLinearIndex(thisIndex, dims)];
    }
  });
  const activeNeighbors: number = neighbors.filter((val: string): boolean => val === '#').length;
  if (current === '#' && activeNeighbors !== 2 && activeNeighbors !== 3) {
    return '.';
  }
  if (current === '.' && activeNeighbors === 3) {
    return '#';
  }
  return current;
}

function cycle(input: string[], dims: number[]): string[] {
  const output: string[] = [];
  const ranges: number[][] = [];
  let linearIndex: number;
  let thisIndex: number[];
  for (let i: number = 0; i < dims.length; i += 1) {
    ranges.push(getRange(-1, dims[i]));
  }
  const permutations: number[][] = variablePermutations(
    ranges.map((val: number[]): number => val.length),
  );
  permutations.forEach((perm: number[]): void => {
    thisIndex = perm.map((val: number, i: number): number => ranges[i][val]);
    linearIndex = getLinearIndex(
      thisIndex.map((val: number): number => val + 1),
      dims.map((val: number): number => val + 2),
    );
    output[linearIndex] = status(input, thisIndex, dims);
  });
  return output;
}

function runCycles(numCycles: number, input: string[], dims: number[]): number {
  let output: string[] = input;
  let outputDims: number[] = dims;
  for (let i = 0; i < numCycles; i += 1) {
    output = cycle(output, outputDims);
    outputDims = outputDims.map((val: number): number => val + 2);
  }
  let numActive: number = 0;
  output.forEach((entry: string): void => {
    if (entry === '#') {
      numActive += 1;
    }
  });
  return numActive;
}

const [output, dims]: [string[], number[]] = getStart('input.txt');
console.log(runCycles(6, output, dims));
console.log(runCycles(6, output, dims.concat([1])));
