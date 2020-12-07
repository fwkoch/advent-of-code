import * as fs from 'fs';

class Bag {
  parents: Bag[];

  children: [Bag, number][];

  name: string;

  constructor(name: string) {
    this.name = name;
    this.parents = [];
    this.children = [];
  }

  getAllParentNames(): Set<string> {
    let parentNames: Set<string> = new Set();
    this.parents.forEach((parent: Bag): void => {
      if (!parentNames.has(parent.name)) {
        parentNames.add(parent.name);
        parentNames = new Set([...parentNames, ...parent.getAllParentNames()]);
      }
    });
    return parentNames;
  }

  countAllChildren(): number {
    let count: number = 0;
    this.children.forEach((child: [Bag, number]): void => {
      count += child[1] * (1 + child[0].countAllChildren());
    });
    return count;
  }
}

function childNamesAndCount(rule: string): [string, number][] {
  const ruleHalf: string = rule.split(' bags contain ')[1];
  if (ruleHalf.startsWith('no ')) {
    return [];
  }
  return ruleHalf.split(', ').map((child: string): [string, number] => [
    child.substring(child.indexOf(' ') + 1, child.lastIndexOf(' ')),
    +child.substring(0, child.indexOf(' ')),
  ]);
}

function getBags(filename: string): { [name: string]: Bag } {
  const input: string = fs.readFileSync(filename, 'utf8');
  const bags: { [name: string]: Bag } = {};
  input.split(/\n/).forEach((rule: string): void => {
    const parent: string = rule.split(' bags contain ')[0];
    if (bags[parent] === undefined) {
      bags[parent] = new Bag(parent);
    }
    childNamesAndCount(rule).forEach((child: [string, number]): void => {
      if (bags[child[0]] === undefined) {
        bags[child[0]] = new Bag(child[0]);
      }
      bags[child[0]].parents.push(bags[parent]);
      bags[parent].children.push([bags[child[0]], child[1]]);
    });
  });
  return bags;
}

const shinyGold = getBags('input.txt')['shiny gold'];

console.log(shinyGold.getAllParentNames().size);
console.log(shinyGold.countAllChildren());
