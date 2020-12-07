import * as fs from 'fs';

class Bag {
  parents: Bag[];

  children: Bag[];

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
}

function childNames(rule: string): string[] {
  const ruleHalf: string = rule.split(' bags contain ')[1];
  if (ruleHalf.startsWith('no ')) {
    return [];
  }
  return ruleHalf.split(', ').map((child: string): string => child.substring(child.indexOf(' ') + 1, child.lastIndexOf(' ')));
}

function getBags(filename: string): { [name: string]: Bag } {
  const input: string = fs.readFileSync(filename, 'utf8');
  const bags: { [name: string]: Bag } = {};
  input.split(/\n/).forEach((rule: string): void => {
    const parent: string = rule.split(' bags contain ')[0];
    if (bags[parent] === undefined) {
      bags[parent] = new Bag(parent);
    }
    childNames(rule).forEach((child: string): void => {
      if (bags[child] === undefined) {
        bags[child] = new Bag(child);
      }
      bags[child].parents.push(bags[parent]);
      bags[parent].children.push(bags[child]);
    });
  });
  return bags;
}

console.log(getBags('input.txt')['shiny gold'].getAllParentNames().size);
