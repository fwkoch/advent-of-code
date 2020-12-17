import * as fs from 'fs';

function getStart(filename: string): string[][][] {
  const input: string[] = fs.readFileSync(filename, 'utf8').split('\n');
  const output: string[][][] = [[]];
  input.forEach((line: string): void => {
    output[0].push(line.split(''));
  });
  return output;
}

function status(input: string[][][], index: [number, number, number]) {
  const neighbors: string[] = [];
  let current: string = '.';
  const ranges: [number, number][] = [
    [Math.max(0, index[0] - 1), Math.min(input.length - 1, index[0] + 1)],
    [Math.max(0, index[1] - 1), Math.min(input[0].length - 1, index[1] + 1)],
    [Math.max(0, index[2] - 1), Math.min(input[0][0].length - 1, index[2] + 1)],
  ];
  for (let i: number = ranges[0][0]; i <= ranges[0][1]; i += 1) {
    for (let j: number = ranges[1][0]; j <= ranges[1][1]; j += 1) {
      for (let k: number = ranges[2][0]; k <= ranges[2][1]; k += 1) {
        if (i !== index[0] || j !== index[1] || k !== index[2]) {
          neighbors.push(input[i][j][k]);
        } else {
          current = input[i][j][k];
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

function cycle(input: string[][][]): string[][][] {
  const output: string[][][] = [];
  let layer: string[][];
  let row: string[];
  for (let i: number = -1; i <= input.length; i += 1) {
    layer = [];
    for (let j: number = -1; j <= input[0].length; j += 1) {
      row = [];
      for (let k: number = -1; k <= input[0][0].length; k += 1) {
        row.push(status(input, [i, j, k]));
      }
      layer.push(row);
    }
    output.push(layer);
  }
  return output;
}

function runCycles(numCycles: number, input: string[][][]): number {
  let output: string[][][] = input;
  for (let i = 0; i < numCycles; i += 1) {
    output = cycle(output);
  }
  let numActive: number = 0;
  output.forEach((layer: string[][]): void => {
    layer.forEach((row: string[]): void => {
      row.forEach((entry: string): void => {
        if (entry === '#') {
          numActive += 1;
        }
      });
    });
  });
  return numActive;
}

console.log(runCycles(6, getStart('input.txt')));
