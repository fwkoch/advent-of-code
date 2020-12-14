import * as fs from 'fs';

function getInput(filename: string): string[] {
  const input: string = fs.readFileSync(filename, 'utf8');
  return input.split('\n');
}

function readInput(input: string[]): {[address: string]: number} {
  const memory: {[address: string]: number} = {};
  let andMask: string = '';
  let orMask: string = '';
  let andMaskFront: string = '';
  let orMaskFront: string = '';
  let addr: string;
  let value: string;
  let newValue: string;
  for (let i: number = 0; i < input.length; i += 1) {
    if (input[i].startsWith('mask = ')) {
      andMaskFront = input[i].slice(7, 12).replace(/X/g, '1');
      orMaskFront = input[i].slice(7, 12).replace(/X/g, '0');
      andMask = input[i].slice(12).replace(/X/g, '1');
      orMask = input[i].slice(12).replace(/X/g, '0');
    } else {
      if (andMask === '' || orMask === '') {
        throw new Error('masks not defined yet.');
      }
      [, addr, , , value] = input[i].split(/[ [\]]/);
      if (+value > 2147483647) {
        throw new Error('this implementation breaks with value > 2147483647');
      }
      newValue = (
        // eslint-disable-next-line no-bitwise
        ((0 & parseInt(andMaskFront, 2)) | parseInt(orMaskFront, 2)).toString(2)
        // eslint-disable-next-line no-bitwise
        + ((+value & parseInt(andMask, 2)) | parseInt(orMask, 2)).toString(2).padStart(31, '0')
      );
      memory[addr] = parseInt(newValue, 2);
    }
  }
  return memory;
}

function valueSum(memory: {[address: string]: number}): number {
  let sum: number = 0;
  Object.keys(memory).forEach((addr: string): void => {
    sum += memory[addr];
  });
  return sum;
}

function resolveFloaters(input: string): number[] {
  let addrs: number[] = [];
  if (input.includes('X')) {
    addrs = addrs.concat(resolveFloaters(input.replace('X', '0')));
    addrs = addrs.concat(resolveFloaters(input.replace('X', '1')));
  } else {
    addrs.push(parseInt(input, 2));
  }
  return addrs;
}

function applyMask(address: number, mask: string): string {
  const output: string[] = [];
  const addrString: string = address.toString(2).padStart(36, '0');
  for (let i: number = 0; i < 36; i += 1) {
    if (mask[i] === '0') {
      output.push(addrString[i]);
    } else {
      output.push(mask[i]);
    }
  }
  return output.join('');
}

function readInputRule2(input: string[]): {[address: string]: number} {
  const memory: {[address: string]: number} = {};
  let mask: string = '';
  let address: string;
  let value: string;
  for (let i: number = 0; i < input.length; i += 1) {
    if (input[i].startsWith('mask = ')) {
      mask = input[i].slice(7);
    } else {
      if (mask === '') {
        throw new Error('mask not defined yet.');
      }
      [, address, , , value] = input[i].split(/[ [\]]/);
      const val: number = +value;
      resolveFloaters(applyMask(+address, mask)).forEach((addr: number): void => {
        memory[addr] = val;
      });
    }
  }
  return memory;
}

console.log(valueSum(readInput(getInput('input.txt'))));
console.log(valueSum(readInputRule2(getInput('input.txt'))));
