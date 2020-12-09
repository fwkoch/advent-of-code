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

console.log(firstInvalid(getEntries('input.txt'), 25));
