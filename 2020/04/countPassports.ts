import * as fs from 'fs';

const requiredKeys: string[] = ['byr', 'iyr', 'eyr', 'hgt', 'hcl', 'ecl', 'pid'];

function countPasswords(filename: string): number {
  const input: string = fs.readFileSync(filename, 'utf8');
  const lines: string[] = input.split(/\r?\n\r?\n/);
  const entries: string[][][] = lines.map((line: string): string[][] => line.split(/\s/).map((entry: string): string[] => entry.split(':')));
  const passports: Object[] = entries.map((entry: string[][]): Object => Object.fromEntries(entry));
  let count: number = 0;
  passports.forEach((passport: Object): void => {
    if (requiredKeys.every((key: string): boolean => Object.keys(passport).includes(key))) {
      count += 1;
    }
  });
  return count;
}

console.log(countPasswords('input.txt'));
