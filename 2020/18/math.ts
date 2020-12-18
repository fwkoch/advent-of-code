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

function evaluateParen(expression: string): number {
  let parenStart: number = -1;
  let i: number = 0;
  while (expression[i] !== ')' && i < expression.length) {
    if (expression[i] === '(') {
      parenStart = i;
    }
    i += 1;
  }
  if (i === expression.length) {
    return evaluateNoParen(expression);
  }
  if (parenStart === -1) {
    throw new Error('bad parentheses');
  }
  return evaluateParen(
    expression.slice(0, parenStart)
    + evaluateParen(expression.slice(parenStart + 1, i))
    + expression.slice(i + 1),
  );
}

function sumOfResults(expressions: string[]): number {
  let sum: number = 0;
  expressions.forEach((expression: string): void => {
    sum += evaluateParen(expression);
  });
  return sum;
}

console.log(sumOfResults(getExpressions('input.txt')));
