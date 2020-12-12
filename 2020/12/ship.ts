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

  waypoint: [number, number];

  facing: number;

  constructor() {
    this.location = [0, 0];
    this.waypoint = [10, 1];
    this.facing = 90;
  }

  moveShip(action: string, value: number): void {
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

  moveWaypoint(action: string, value: number): void {
    if (action === 'F') {
      this.location = [
        this.location[0] + value * this.waypoint[0],
        this.location[1] + value * this.waypoint[1],
      ];
    } else if (action === 'R') {
      for (let i = 0; i < value / 90; i += 1) {
        this.waypoint = [this.waypoint[1], -this.waypoint[0]];
      }
    } else if (action === 'L') {
      for (let i = 0; i < value / 90; i += 1) {
        this.waypoint = [-this.waypoint[1], this.waypoint[0]];
      }
    } else if (action === 'N') {
      this.waypoint[1] += value;
    } else if (action === 'S') {
      this.waypoint[1] -= value;
    } else if (action === 'E') {
      this.waypoint[0] += value;
    } else if (action === 'W') {
      this.waypoint[0] -= value;
    }
  }

  consumeShipInstructions(instructions: string[]): void {
    instructions.forEach((instruction: string): void => {
      this.moveShip(instruction[0], +instruction.slice(1, instruction.length));
    });
  }

  consumeWaypointInstructions(instructions: string[]): void {
    instructions.forEach((instruction: string): void => {
      this.moveWaypoint(instruction[0], +instruction.slice(1, instruction.length));
    });
  }
}

function manhattanDistanceShip(filename: string): number {
  const ship: Ship = new Ship();
  ship.consumeShipInstructions(getInstructions(filename));
  return Math.abs(ship.location[0]) + Math.abs(ship.location[1]);
}

function manhattanDistanceWaypoint(filename: string): number {
  const ship: Ship = new Ship();
  ship.consumeWaypointInstructions(getInstructions(filename));
  return Math.abs(ship.location[0]) + Math.abs(ship.location[1]);
}

console.log(manhattanDistanceShip('input.txt'));
console.log(manhattanDistanceWaypoint('input.txt'));
