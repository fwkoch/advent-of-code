import * as fs from 'fs';

function getJolts(filename: string): number[] {
  const input: string = fs.readFileSync(filename, 'utf8');
  return input.split('\n').map((entry: string): number => +entry).sort((a, b) => a - b);
}

function getDifferences(jolts: number[]): number[] {
  jolts.unshift(0);
  jolts.push(jolts[jolts.length - 1] + 3);
  return jolts.slice(1, jolts.length).map((jolt: number, i: number): number => jolt - jolts[i]);
}

function threeOneProduct(differences: number[]): number {
  const ones: number = differences.filter((x: number): boolean => x === 1).length;
  const threes: number = differences.filter((x: number): boolean => x === 3).length;
  return ones * threes;
}

console.log(threeOneProduct(getDifferences(getJolts('input.txt'))));
