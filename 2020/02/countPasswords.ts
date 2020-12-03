import * as fs from 'fs';

function readInput(filename: string): string[] {
  const input: string = fs.readFileSync(filename, 'utf8');
  return input.split(/\r?\n/);
}

function sledPasswordIsValid(password: string, char: string, min: number, max: number): boolean {
  const count: number = password.split(char).length - 1;
  return (count >= min && count <= max);
}

function passwordIsValid(password: string, char: string, indexA: number, indexB: number): boolean {
  const a: boolean = password.charAt(indexA - 1) === char;
  const b: boolean = password.charAt(indexB - 1) === char;
  return a !== b;
}

function validPasswordCount(
  inputs: string[],
  validator: (pw: string, char: string, a: number, b: number) => boolean,
): number {
  let validCount: number = 0;
  inputs.forEach((input: string): void => {
    const pieces: string[] = input.split(/[-: ]/);
    const min: number = +pieces[0];
    const max: number = +pieces[1];
    const char: string = pieces[2];
    const password: string = pieces[4];
    if (validator(password, char, min, max)) {
      validCount += 1;
    }
  });
  return validCount;
}

console.log(validPasswordCount(readInput('input.txt'), sledPasswordIsValid));
console.log(validPasswordCount(readInput('input.txt'), passwordIsValid));
