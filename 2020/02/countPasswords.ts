import * as fs from 'fs';

function readInput(filename: string): string[] {
  const input: string = fs.readFileSync(filename, 'utf8');
  return input.split(/\r?\n/);
}

function passwordIsValid(password: string, char: string, min: number, max: number): boolean {
  const count: number = password.split(char).length - 1;
  return (count >= min && count <= max);
}

function validPasswordCount(inputs: string[]): number {
  let validCount: number = 0;
  inputs.forEach((input: string) => {
    console.log(input);
    const pieces: string[] = input.split(/[-: ]/);
    const min: number = +pieces[0];
    const max: number = +pieces[1];
    const char: string = pieces[2];
    const password: string = pieces[4];
    if (passwordIsValid(password, char, min, max)) {
      validCount += 1;
    }
  });
  return validCount;
}

console.log(validPasswordCount(readInput('input.txt')));
