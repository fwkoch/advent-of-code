import * as fs from 'fs';

function readInput(filename: string): number[] {
  const input: string = fs.readFileSync(filename, 'utf8');
  const inputValues: string[] = input.split(/\r?\n/);
  return inputValues.map((x) => +x);
}

function findPair(values: number[], target: number = 2020): number {
  let i: number;
  let j: number;
  for (i = 0; i < values.length; i += 1) {
    for (j = i + 1; j < values.length; j += 1) {
      if (values[i] + values[j] === target) {
        return values[i] * values[j];
      }
    }
  }
  throw new Error('target not found');
}

function findTrio(values: number[], target: number = 2020): number {
  let i: number;
  let j: number;
  let k: number;
  for (i = 0; i < values.length; i += 1) {
    for (j = i + 1; j < values.length; j += 1) {
      if (values[i] + values[j] < target) {
        for (k = j + 1; k < values.length; k += 1) {
          if (values[i] + values[j] + values[k] === target) {
            return values[i] * values[j] * values[k];
          }
        }
      }
    }
  }
  throw new Error('target not found');
}

console.log(findPair(readInput('input.txt')));
console.log(findTrio(readInput('input.txt')));
