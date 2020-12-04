import * as fs from 'fs';

const requiredKeys: string[] = ['byr', 'iyr', 'eyr', 'hgt', 'hcl', 'ecl', 'pid'];

interface Passport {
  byr: string;
  iyr: string;
  eyr: string;
  hgt: string;
  hcl: string;
  ecl: string;
  pid: string;
}

function validateByr(byr: string): boolean {
  return +byr >= 1920 && +byr <= 2002;
}

function validateIyr(iyr: string): boolean {
  return +iyr >= 2010 && +iyr <= 2020;
}

function validateEyr(eyr: string): boolean {
  return +eyr >= 2020 && +eyr <= 2030;
}

function validateHgt(hgt: string): boolean {
  let height: number;
  if (hgt.endsWith('cm')) {
    height = +hgt.replace('cm', '');
    return height >= 150 && height <= 193;
  }
  if (hgt.endsWith('in')) {
    height = +hgt.replace('in', '');
    return height >= 59 && height <= 76;
  }
  return false;
}

function validateHcl(hcl: string): boolean {
  return hcl.match(/^#[0-9a-f]{6}$/) !== null;
}

function validateEcl(ecl: string): boolean {
  const colors: string[] = ['amb', 'blu', 'brn', 'gry', 'grn', 'hzl', 'oth'];
  return colors.includes(ecl);
}

function validatePid(pid: string): boolean {
  return pid.length === 9 && +pid >= 0;
}

function validatePassportBasic(passport: Object): boolean {
  return requiredKeys.every((key: string): boolean => Object.keys(passport).includes(key));
}

function validatePassport(passport: Passport): boolean {
  if (!validatePassportBasic(passport)) {
    return false;
  }
  const validators: boolean[] = [
    validateByr(passport.byr),
    validateIyr(passport.iyr),
    validateEyr(passport.eyr),
    validateHgt(passport.hgt),
    validateHcl(passport.hcl),
    validateEcl(passport.ecl),
    validatePid(passport.pid),
  ];
  return validators.every(Boolean);
}

function countPassports(filename: string, validator: (passport: Passport) => boolean): number {
  const input: string = fs.readFileSync(filename, 'utf8');
  const lines: string[] = input.split(/\r?\n\r?\n/);
  const entries: string[][][] = lines.map((line: string): string[][] => line.split(/\s/).map((entry: string): string[] => entry.split(':')));
  const passports: Passport[] = entries.map((ent: string[][]): Passport => Object.fromEntries(ent));
  let count: number = 0;
  passports.forEach((passport: Passport): void => {
    if (validator(passport)) {
      count += 1;
    }
  });
  return count;
}

console.log(countPassports('input.txt', validatePassportBasic));
console.log(countPassports('input.txt', validatePassport));
