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

function errorRate(rules: {[num: string]: number[]}, tickets: number[][]): number {
  let invalidValues: number[] = [];
  for (let i: number = 0; i < tickets.length; i += 1) {
    invalidValues = invalidValues.concat(getInvalidValues(rules, tickets[i]));
  }
  return invalidValues.reduce((a: number, b: number): number => a + b, 0);
}

function getValidTickets(rules: {[num: string]: number[]}, tickets: number[][]): number[][] {
  const validTickets: number[][] = [];
  for (let i: number = 0; i < tickets.length; i += 1) {
    if (getInvalidValues(rules, tickets[i]).length === 0) {
      validTickets.push(tickets[i]);
    }
  }
  return validTickets;
}

function getAllIndicesForRule(
  rules: {[num: string]: number[]},
  tickets: number[][],
): {[num: string]: number[]} {
  const validIndicesForRule: {[rule: string]: number[]} = {};
  const ruleKeys: string[] = Object.keys(rules);
  let j: number;
  let valid: boolean;
  for (let i: number = 0; i < ruleKeys.length; i += 1) {
    validIndicesForRule[ruleKeys[i]] = [];
    for (let field: number = 0; field < tickets[0].length; field += 1) {
      j = 0;
      valid = true;
      while (j < tickets.length && valid) {
        valid = isValueValid(tickets[j][field], rules[ruleKeys[i]]);
        j += 1;
      }
      if (valid) {
        validIndicesForRule[ruleKeys[i]].push(field);
      }
    }
  }
  return validIndicesForRule;
}

function reduceRuleIndices(ruleIndices: {[num: string]: number[]}): {[num: string]: number[]} {
  const ruleIndicesCopy: {[num: string]: number[]} = { ...ruleIndices };
  const ruleKeys: string[] = Object.keys(ruleIndices);
  let allIndicesUnique: boolean = false;
  while (!allIndicesUnique) {
    allIndicesUnique = true;
    for (let i: number = 0; i < ruleKeys.length; i += 1) {
      if (ruleIndicesCopy[ruleKeys[i]].length === 1) {
        for (let j: number = 0; j < ruleKeys.length; j += 1) {
          if (j !== i) {
            ruleIndicesCopy[ruleKeys[j]] = ruleIndicesCopy[ruleKeys[j]].filter(
              (index: number): boolean => index !== ruleIndicesCopy[ruleKeys[i]][0],
            );
          }
        }
      } else {
        allIndicesUnique = false;
      }
    }
  }
  return ruleIndicesCopy;
}

function departureProduct(ruleIndices: {[num: string]: number[]}, ticket: number[]): number {
  const ruleKeys: string[] = Object.keys(ruleIndices);
  let product: number = 1;
  for (let i: number = 0; i < ruleKeys.length; i += 1) {
    if (ruleKeys[i].startsWith('departure')) {
      product *= ticket[ruleIndices[ruleKeys[i]][0]];
    }
  }
  return product;
}

const [rules, yourTicket, nearbyTickets]: [
  {[num: string]: number[]}, number[], number[][]
] = getInputs('input.txt');
console.log(errorRate(rules, nearbyTickets));
console.log(
  departureProduct(
    reduceRuleIndices(
      getAllIndicesForRule(
        rules,
        getValidTickets(
          rules,
          nearbyTickets,
        ),
      ),
    ),
    yourTicket,
  ),
);
