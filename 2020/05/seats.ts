import * as fs from 'fs';

function getSeats(filename: string): string[] {
  const input: string = fs.readFileSync(filename, 'utf8');
  return input.split(/\r?\n/);
}

function getValue(partition: string, upperChar: string): number {
  let count: number = 2 ** (partition.length - 1);
  let output: number = 0;
  let i: number;
  for (i = 0; i < partition.length; i += 1) {
    if (partition[i] === upperChar) {
      output += count;
    }
    count /= 2;
  }
  return output;
}

function getRow(partition: string): number {
  return getValue(partition, 'B');
}

function getCol(partition: string): number {
  return getValue(partition, 'R');
}

function getSeatID(partition: string): number {
  return getRow(partition.substring(0, 7)) * 8 + getCol(partition.substring(7, 10));
}

function getMaxSeatID(partitions: string[]): number {
  let seatID: number = 0;
  let id: number;
  partitions.forEach((partition: string): void => {
    id = getSeatID(partition);
    if (id > seatID) {
      seatID = id;
    }
  });
  return seatID;
}

function getSortedSeatIDs(partitions: string[]): number[] {
  const seatIDs: number[] = [];
  partitions.forEach((partition: string): void => {
    seatIDs.push(getSeatID(partition));
  });
  return seatIDs.sort((a: number, b: number): number => a - b);
}

function findGapOfOne(sortedSeats: number[]): number {
  let i: number;
  for (i = 0; i < sortedSeats.length - 1; i += 1) {
    if (sortedSeats[i + 1] - sortedSeats[i] === 2) {
      return sortedSeats[i] + 1;
    }
  }
  throw new Error('seat not found');
}

console.log(getMaxSeatID(getSeats('input.txt')));
console.log(findGapOfOne(getSortedSeatIDs(getSeats('input.txt'))));
