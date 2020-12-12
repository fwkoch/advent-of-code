import * as fs from 'fs';

function getInstructions(filename: string): string[] {
  const input: string = fs.readFileSync(filename, 'utf8');
  return input.split('\n');
}

const directions: {[dir: number]: string} = {
  0: 'N',
  90: 'E',
  180: 'S',
  270: 'W',
};

class Ship {
  location: [number, number];

  facing: number;

  constructor() {
    this.location = [0, 0];
    this.facing = 90;
  }

  move(action: string, value: number): void {
    if (action === 'R') {
      this.facing = (this.facing + value) % 360;
    } else if (action === 'L') {
      this.facing = (this.facing + 360 - value) % 360;
    } else {
      let direction: string = action;
      if (action === 'F') {
        direction = directions[this.facing];
      }
      if (direction === 'N') {
        this.location[1] += value;
      } else if (direction === 'S') {
        this.location[1] -= value;
      } else if (direction === 'E') {
        this.location[0] += value;
      } else if (direction === 'W') {
        this.location[0] -= value;
      }
    }
  }

  consumeInstructions(instructions: string[]): void {
    instructions.forEach((instruction: string): void => {
      this.move(instruction[0], +instruction.slice(1, instruction.length));
    });
  }
}

function manhattanDistance(filename: string): number {
  const ship: Ship = new Ship();
  ship.consumeInstructions(getInstructions(filename));
  return Math.abs(ship.location[0]) + Math.abs(ship.location[1]);
}

console.log(manhattanDistance('input.txt'));
