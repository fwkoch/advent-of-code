import * as fs from 'fs';

function getInstructions(filename: string): [string, number][] {
  const input: string = fs.readFileSync(filename, 'utf8');
  return input.split('\n').map((instruction: string): [string, number] => [instruction.split(' ')[0], +instruction.split(' ')[1]]);
}

function runInstructions(instructions: [string, number][], modIndex?: number): number {
  let index: number = 0;
  let accumulator: number = 0;
  const visited: Set<number> = new Set();
  let instruction: string;
  let step: number;
  while (index < instructions.length) {
    if (visited.has(index)) {
      throw new Error(`Loop found with accumulator ${accumulator}`);
    }
    visited.add(index);
    [instruction, step] = instructions[index];
    if (index === modIndex && instruction === 'jmp') {
      instruction = 'nop';
    } else if (index === modIndex && instruction === 'nop') {
      instruction = 'jmp';
    }
    if (instruction === 'nop') {
      index += 1;
    } else if (instruction === 'acc') {
      accumulator += step;
      index += 1;
    } else if (instruction === 'jmp') {
      index += step;
    } else {
      throw new Error('unknown instruction');
    }
  }
  return accumulator;
}

function findModifiedOutput(instructions: [string, number][]): number {
  for (let i = 0; i < instructions.length; i += 1) {
    if (instructions[i][0] !== 'acc') {
      try {
        return runInstructions(instructions, i);
      // eslint-disable-next-line no-empty
      } catch {}
    }
  }
  throw new Error('no successful modification found');
}

try {
  runInstructions(getInstructions('input.txt'));
} catch (err) {
  console.log(err);
}
console.log(findModifiedOutput(getInstructions('input.txt')));
