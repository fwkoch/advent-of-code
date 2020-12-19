import * as fs from 'fs';

function getInput(filename: string): [string[], string[]] {
  const input: string[] = fs.readFileSync(filename, 'utf8').split('\n\n');
  return [input[0].split('\n'), input[1].split('\n')];
}

class Rule {
  name: string

  value: string | string[][] | Rule[][]

  constructor(name: string, value: string | string[][] | Rule[][]) {
    this.name = name;
    this.value = value;
  }

  convert(lookup: {[ruleNumber: string]: Rule}): void {
    if (typeof this.value === 'string') {
      return;
    }
    let key: string;
    for (let i: number = 0; i < this.value.length; i += 1) {
      for (let j: number = 0; j < this.value[i].length; j += 1) {
        if (typeof this.value[i][j] === 'string') {
          key = this.value[i][j] as string;
          lookup[key].convert(lookup);
          this.value[i][j] = lookup[key];
        }
      }
    }
  }

  validate(input: string): boolean {
    if (typeof this.value === 'string') {
      return this.value === input;
    }
    let currentRules: Rule[];
    let valid: boolean = false;
    let tempRule: Rule;
    for (let i: number = 0; i < this.value.length; i += 1) {
      if (this.value[i].length > 0 && typeof this.value[i][0] === 'string') {
        throw new Error('convert before validating');
      }
      currentRules = this.value[i] as Rule[];
      for (let j: number = 1; j <= input.length - currentRules.length + 1; j += 1) {
        valid = currentRules[0].validate(input.slice(0, j));
        if (valid && currentRules.length > 1) {
          tempRule = new Rule('temp', [currentRules.slice(1)]);
          valid = valid && tempRule.validate(input.slice(j));
        } else if (input.slice(j).length > 0) {
          valid = false;
        }
        if (valid) {
          return true;
        }
      }
    }
    return valid;
  }
}

function getRules(input: string[]): {[ruleNumber: string]: Rule} {
  const rules: {[ruleNumber: string]: Rule} = {};
  let key: string;
  let remainder: string;
  input.forEach((rule: string): void => {
    [key, remainder] = rule.split(': ');
    if (remainder[0] === '"') {
      rules[key] = new Rule(key, remainder[1]);
    } else {
      rules[key] = new Rule(
        key,
        remainder.split(' | ').map((ruleSet: string): string[] => ruleSet.split(' ')),
      );
    }
  });
  return rules;
}

function countValidMessages(rule: Rule, messages: string[]): number {
  let count: number = 0;
  messages.forEach((message: string, i: number): void => {
    console.log(i);
    if (message.length === 24 && rule.validate(message)) {
      count += 1;
    }
  });
  return count;
}

const [rulesInput, messagesInput]: [string[], string[]] = getInput('input.txt');
const rules: {[ruleNumber: string]: Rule} = getRules(rulesInput);
rules[0].convert(rules);
console.log(countValidMessages(rules[0], messagesInput));
