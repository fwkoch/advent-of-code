import * as fs from 'fs';

function getSeats(filename: string): string[] {
  const input: string = fs.readFileSync(filename, 'utf8');
  return input.split(/\r?\n/);
}

function getValue(partition: string, upperChar: string): number {
  let count: number = 2**(partition.length-1);
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
  return getRow(partition.substring(0, 7))*8 + getCol(partition.substring(7, 10))
}

function getMaxSeatID(partitions: string[]): number {
  let seatID: number = 0;
  partitions.forEach((partition: string): void => {
    let id: number = getSeatID(partition);
    if (id > seatID) {
      seatID = id;
    }
  })
  return seatID;
}

console.log(getMaxSeatID(getSeats('input.txt')))
