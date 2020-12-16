import * as fs from 'fs';

const numRules: number = 20;

function getInputs(filename: string): [{[num: string]: number[]}, number[], number[][]] {
  const input: string[] = fs.readFileSync(filename, 'utf8').split('\n');
  const rules: {[num: string]: number[]} = {};
  let yourTicket: number[] = [];
  const nearbyTickets: number[][] = [];
  let regexOutput: string[] | null;
  for (let i = 0; i < input.length; i += 1) {
    if (i < numRules) {
      regexOutput = input[i].match(/([a-z ]+): (\d+)-(\d+) or (\d+)-(\d+)/)
      if (regexOutput === null) {
        throw new Error('bad regex');
      } else {
        regexOutput = regexOutput.slice(1, 6);
      }
      rules[regexOutput[0]] = regexOutput.slice(1).map((value: string): number => +value);
    } else if (i === numRules + 2) {
      yourTicket = input[i].split(',').map((value: string): number => +value);
    } else if (i > numRules + 4) {
      nearbyTickets.push(input[i].split(',').map((value: string): number => +value));
    }
  }
  return [rules, yourTicket, nearbyTickets];
}

function isValueValid(value: number, ruleValues: number[]) {
  return (
    (value >= ruleValues[0] && value <= ruleValues[1])
    || (value >= ruleValues[2] && value <= ruleValues[3])
  );
}

function getInvalidValues(rules: {[num: string]: number[]}, ticket: number[]): number[] {
  const ruleKeys: string[] = Object.keys(rules);
  let valid: boolean;
  const invalidValues: number[] = [];
  for (let i: number = 0; i < ticket.length; i += 1) {
    valid = false;
    let j: number = 0;
    while (j < ruleKeys.length && !valid) {
      valid = isValueValid(ticket[i], rules[ruleKeys[j]]);
      j += 1;
    }
    if (!valid) {
      invalidValues.push(ticket[i]);
    }
  }
  return invalidValues;
}

function errorRate(filename: string): number {
  const [rules, , nearbyTickets]: [
    {[num: string]: number[]}, number[], number[][]
  ] = getInputs(filename);
  let invalidValues: number[] = [];
  for (let i: number = 0; i < nearbyTickets.length; i += 1) {
    invalidValues = invalidValues.concat(getInvalidValues(rules, nearbyTickets[i]));
  }
  return invalidValues.reduce((a: number, b: number): number => a + b, 0);
}

console.log(errorRate('input.txt'));
