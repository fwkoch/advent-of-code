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

function multiplySlopes(input: string[], slopes: [number, number][]) {
  let product: number = 1;
  slopes.forEach((slope: [number, number]): void => {
    product *= countTrees(input, slope);
  });
  return product;
}

console.log(countTrees(readInput('input.txt'), [1, 3]));
console.log(multiplySlopes(readInput('input.txt'), [[1, 1], [1, 3], [1, 5], [1, 7], [2, 1]]));
