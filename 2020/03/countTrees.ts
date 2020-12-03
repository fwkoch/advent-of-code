import * as fs from 'fs';

function readInput(filename: string): string[] {
  const input: string = fs.readFileSync(filename, 'utf8');
  return input.split(/\r?\n/);
}

function countTrees(input: string[], slope: [number, number], start: [number, number] = [0, 0], tree: string = '#'): number {
  let trees: number = 0;
  let location: [number, number] = start;
  while (location[0] < input.length) {
    if (input[location[0]][location[1] % input[0].length] === tree) {
      trees += 1;
    }
    location = [location[0] + slope[0], location[1] + slope[1]];
  }
  return trees;
}

console.log(countTrees(readInput('input.txt'), [1, 3]));
