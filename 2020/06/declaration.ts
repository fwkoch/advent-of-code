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

console.log(getAllYesses(getDeclarations('input.txt')));
