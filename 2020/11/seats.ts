import * as fs from 'fs';

function getSeats(filename: string): string[] {
  const input: string = fs.readFileSync(filename, 'utf8');
  return input.split('\n');
}

function nextSeatState(
  col: number,
  row: number,
  seats: string[],
  neighborCallback: (c: number, r: number, i: number, j: number, s: string[]) => string,
): string {
  const seat: string = seats[row][col];
  if (seat === '.') {
    return '.';
  }
  let occupied: number = 0;
  let neighbor: string;
  for (let i: number = -1; i < 2; i += 1) {
    for (let j: number = -1; j < 2; j += 1) {
      if (i !== 0 || j !== 0) {
        neighbor = neighborCallback(col, row, i, j, seats);
        if (neighbor === '#') {
          occupied += 1;
          if (seat === 'L') {
            return 'L';
          }
          if (occupied >= 4) {
            return 'L';
          }
        }
      }
    }
  }
  return '#';
}

function adjacent(col: number, row: number, i: number, j: number, seats: string[]): string {
  if (seats[row + i] === undefined) {
    return '.';
  }
  const neighbor: string = seats[row + i][col + j];
  if (neighbor === undefined) {
    return '.';
  }
  return neighbor;
}

function nextRound(
  seats: string[],
  neighborCallback: (c: number, r: number, i: number, j: number, s: string[]) => string,
): string[] {
  const nextSeats: string[] = [...seats];
  for (let row: number = 0; row < seats.length; row += 1) {
    nextSeats[row] = '';
    for (let col: number = 0; col < seats[0].length; col += 1) {
      nextSeats[row] += nextSeatState(col, row, seats, neighborCallback);
    }
  }
  return nextSeats;
}

function equal(a: string[], b: string[]): boolean {
  return a.every((value: string, i: number): boolean => value === b[i]);
}

function simulate(
  seats: string[],
  neighborCallback: (c: number, r: number, i: number, j: number, s: string[]) => string,
): string[] {
  let theseSeats: string[] = seats;
  let nextSeats: string[] = nextRound(theseSeats, neighborCallback);
  while (!equal(theseSeats, nextSeats)) {
    theseSeats = nextSeats;
    nextSeats = nextRound(theseSeats, neighborCallback);
  }
  return nextSeats;
}

function countOccupied(seats: string[]): number {
  let occupied: number = 0;
  for (let row: number = 0; row < seats.length; row += 1) {
    for (let col: number = 0; col < seats[0].length; col += 1) {
      if (seats[row][col] === '#') {
        occupied += 1;
      }
    }
  }
  return occupied;
}

console.log(countOccupied(simulate(getSeats('input.txt'), adjacent)));
