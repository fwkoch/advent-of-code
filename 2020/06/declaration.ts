import * as fs from 'fs';

function getDeclarations(filename: string): string[] {
  const input: string = fs.readFileSync(filename, 'utf8');
  return input.replace(/\n\n/g, ':').replace(/\n/g, '').split(':');
}

function getAllYesses(declarations: string[]): number {
  let yesses: number = 0;
  declarations.forEach((declaration: string): void => {
    const newYesses: Set<string> = new Set(declaration);
    yesses += newYesses.size;
  });
  return yesses;
}

function getAllDeclarations(filename: string): number {
  const input: string = fs.readFileSync(filename, 'utf8');
  const declarations: string[] = input.split('\n');
  let count: number = 0;
  let i: number;
  let declarationSet: Set<string> = new Set(declarations[0]);
  for (i = 1; i < declarations.length; i += 1) {
    if (declarations[i] === '') {
      count += declarationSet.size;
      i += 1;
      declarationSet = new Set(declarations[i]);
    } else {
      const comparisonSet: Set<string> = new Set(declarations[i]);
      declarationSet = new Set([...declarationSet].filter((x: string) => comparisonSet.has(x)));
    }
  }
  count += declarationSet.size;
  return count;
}

console.log(getAllYesses(getDeclarations('input.txt')));
console.log(getAllDeclarations('input.txt'));
