import * as fs from 'fs';

function getInstructions(filename: string): [string, number][] {
  const input: string = fs.readFileSync(filename, 'utf8');
  return input.split('\n').map((instruction: string): [string, number] => [instruction.split(' ')[0], +instruction.split(' ')[1]]);
}

function runInstructions(instructions: [string, number][]): number {
  let index: number = 0;
  let accumulator: number = 0;
  const visited: Set<number> = new Set();
  while (true) {
    if (visited.has(index)) {
      return accumulator;
    }
    visited.add(index);
    if (instructions[index][0] === 'nop') {
      index += 1;
    } else if (instructions[index][0] === 'acc') {
      accumulator += instructions[index][1];
      index += 1;
    } else if (instructions[index][0] === 'jmp') {
      index += instructions[index][1];
    } else {
      throw new Error('unknown instruction');
    }
  }
}

console.log(runInstructions(getInstructions('input.txt')));
