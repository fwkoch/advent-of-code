import * as fs from 'fs';

function getEntries(filename: string): number[] {
  const input: string = fs.readFileSync(filename, 'utf8');
  return input.split('\n').map((entry: string): number => +entry);
}

function isValid(preamble: number[], value: number): boolean {
  const differences: number[] = preamble.map((entry: number): number => value - entry);
  for (let i: number = 0; i < differences.length; i += 1) {
    if (![i, -1].includes(preamble.indexOf(differences[i]))) {
      return true;
    }
  }
  return false;
}

function firstInvalid(entries: number[], preambleLength: number): number {
  for (let i = preambleLength; i < entries.length; i += 1) {
    if (!isValid(entries.slice(i - preambleLength, i), entries[i])) {
      return entries[i];
    }
  }
  throw new Error('all entries valid');
}

function encryptionWeakness(entries: number[], target: number): number {
  let sum: number;
  let j: number;
  for (let i: number = 0; i < entries.length - 1; i += 1) {
    j = i + 1;
    sum = entries[i];
    while (sum < target) {
      sum += entries[j];
      j += 1;
      if (sum === target) {
        const slice: number[] = entries.slice(i, j).sort((a, b) => a - b);
        return slice[0] + slice[slice.length - 1];
      }
    }
  }
  throw new Error('no contiguous sum found');
}

const entries: number[] = getEntries('input.txt');
console.log(firstInvalid(entries, 25));
console.log(encryptionWeakness(entries, firstInvalid(entries, 25)));
