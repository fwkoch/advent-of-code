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

function groupsOfOnes(differences: number[]): {[count: string]: number} {
  const counts: {[count: string]: number} = {};
  let currentCount: number = 0;
  for (let i = 0; i < differences.length; i += 1) {
    if (differences[i] === 1) {
      currentCount += 1;
    } else if (differences[i] === 3) {
      if (counts[currentCount] === undefined) {
        counts[currentCount] = 1;
      } else {
        counts[currentCount] += 1;
      }
      currentCount = 0;
    } else {
      throw new Error('unexpected difference');
    }
  }
  return counts;
}

function uniqueArrangements(n: number): [number, number, number][] {
  // Returns list of [n1, n2, n3] arrangements, where n1 is number of 1s, etc.
  if (n < 0) {
    return [];
  }
  if (n === 0) {
    return [[0, 0, 0]];
  }
  if (n === 1) {
    return [[0, 0, 1]];
  }
  const onePrefix: [number, number, number][] = uniqueArrangements(n - 1)
    .map((val: [number, number, number]): [number, number, number] => [val[0], val[1], val[2] + 1]);
  const twoPrefix: [number, number, number][] = uniqueArrangements(n - 2)
    .map((val: [number, number, number]): [number, number, number] => [val[0], val[1] + 1, val[2]]);
  const threePrefix: [number, number, number][] = uniqueArrangements(n - 3)
    .map((val: [number, number, number]): [number, number, number] => [val[0] + 1, val[1], val[2]]);
  return onePrefix.concat(twoPrefix).concat(threePrefix);
}

function factorial(n: number): number {
  if (n === 1 || n === 0) {
    return 1;
  }
  return n * factorial(n - 1);
}

function combinations(val: [number, number, number]): number {
  const numerator: number = factorial(val[0] + val[1] + val[2]);
  const denominator: number = factorial(val[0]) * factorial(val[1]) * factorial(val[2]);
  return numerator / denominator;
}

function setifyArrayOfArrays(array: [number, number, number][]): [number, number, number][] {
  const arraySet: Set<string> = new Set(
    array.map((val: [number, number, number]): string => `${val[0]}${val[1]}${val[2]}`),
  );
  return Array.from(arraySet)
    .map((val: string): [number, number, number] => [+val[0], +val[1], +val[2]]);
}

function totalCombinations(n: number): number {
  return setifyArrayOfArrays(uniqueArrangements(n))
    .map((val: [number, number, number]): number => combinations(val))
    .reduce((a, b) => a + b, 0);
}

// this makes the previous 46 lines of code obsolete. shucks
const totalCombinationsLookup: {[count: string]: number} = {
  0: 1,
  1: 1,
  2: 2, // 11, 2
  3: 4, // 111, 21, 12, 3
  4: 7, // 1111, 211, 121, 112, 22, 31, 13
};

function getBigProduct(groups: {[count: string]: number}): number {
  let product: number = 1;
  Object.keys(groups).forEach((key: string): void => {
    // product *= totalCombinations(+key) ** groups[key];
    product *= totalCombinationsLookup[key] ** groups[key];
  });
  return product;
}

console.log(threeOneProduct(getDifferences(getJolts('input.txt'))));
console.log(getBigProduct(groupsOfOnes(getDifferences(getJolts('input.txt')))));
