import * as fs from 'fs';

function getExpressions(filename: string): string[] {
  return fs.readFileSync(filename, 'utf8').split('\n');
}

function evaluateNoParen(expression: string): number {
  const components: string[] = expression.split(' ');
  let operand: number = +components[0];
  let operation: string | undefined;
  let i: number = 1;
  while (i < components.length) {
    if (operation === undefined) {
      operation = components[i];
    } else {
      if (operation === '+') {
        operand += +components[i];
      } else {
        operand *= +components[i];
      }
      operation = undefined;
    }
    i += 1;
  }
  return operand;
}

function evaluatePlusesRule(expression: string): number {
  const components: string[] = expression.split(' ');
  let i: number = 0;
  if (components.indexOf('+') === -1 || components.indexOf('*') === -1) {
    return evaluateNoParen(expression);
  }
  while (components[i] !== '+' && i < components.length) {
    i += 1;
  }
  return evaluatePlusesRule(
    components.slice(0, i - 1).concat(
      [evaluateNoParen(components.slice(i - 1, i + 2).join(' ')).toString()],
    ).concat(
      components.slice(i + 2),
    ).join(' '),
  );
}

function evaluateParen(expression: string, plusesRuleDude: boolean): number {
  let parenStart: number = -1;
  let i: number = 0;
  while (expression[i] !== ')' && i < expression.length) {
    if (expression[i] === '(') {
      parenStart = i;
    }
    i += 1;
  }
  if (i === expression.length) {
    if (plusesRuleDude) {
      return evaluatePlusesRule(expression);
    }
    return evaluateNoParen(expression);
  }
  if (parenStart === -1) {
    throw new Error('bad parentheses');
  }
  return evaluateParen(
    expression.slice(0, parenStart)
    + evaluateParen(expression.slice(parenStart + 1, i), plusesRuleDude)
    + expression.slice(i + 1),
    plusesRuleDude,
  );
}

function sumOfResults(expressions: string[], plusesRuleDude: boolean): number {
  let sum: number = 0;
  expressions.forEach((expression: string): void => {
    sum += evaluateParen(expression, plusesRuleDude);
  });
  return sum;
}

console.log(sumOfResults(getExpressions('input.txt'), false));
console.log(sumOfResults(getExpressions('input.txt'), true));
