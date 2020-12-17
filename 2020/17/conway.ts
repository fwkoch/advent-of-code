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
    dimProduct = dims.slice(i + 1).reduce((a: number, b: number): number => a * b, 1);
    linearIndex += index[i] * dimProduct;
  }
  return linearIndex;
}

function status(input: string[], index: number[], dims: number[]) {
  const neighbors: string[] = [];
  let current: string = '.';
  const ranges: [number, number][] = [];
  for (let i: number = 0; i < index.length; i += 1) {
    ranges.push([
      Math.max(0, index[i] - 1),
      Math.min(dims[i] - 1, index[i] + 1),
    ]);
  }
  for (let i: number = ranges[0][0]; i <= ranges[0][1]; i += 1) {
    for (let j: number = ranges[1][0]; j <= ranges[1][1]; j += 1) {
      for (let k: number = ranges[2][0]; k <= ranges[2][1]; k += 1) {
        if (i !== index[0] || j !== index[1] || k !== index[2]) {
          neighbors.push(input[getLinearIndex([i, j, k], dims)]);
        } else {
          current = input[getLinearIndex([i, j, k], dims)];
        }
      }
    }
  }
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
  for (let i: number = -1; i <= dims[0]; i += 1) {
    for (let j: number = -1; j <= dims[1]; j += 1) {
      for (let k: number = -1; k <= dims[2]; k += 1) {
        output[
          getLinearIndex([i + 1, j + 1, k + 1], [dims[0] + 2, dims[1] + 2, dims[2] + 2])
        ] = status(input, [i, j, k], dims);
      }
    }
  }
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
