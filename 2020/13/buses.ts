import * as fs from 'fs';

function getBuses(filename: string): [number, number[]] {
  const input: string = fs.readFileSync(filename, 'utf8');
  const timestamp: number = +input.split('\n')[0];
  const buses: number[] = input.split('\n')[1].split(',')
    .filter((bus: string): boolean => bus !== 'x')
    .map((bus: string): number => +bus);
  return [timestamp, buses];
}

function departureTime(timestamp: number, buses: number[]) {
  let departure: number = timestamp;
  while (true) {
    for (let i: number = 0; i < buses.length; i += 1) {
      if (departure % buses[i] === 0) {
        return buses[i] * (departure - timestamp)
      }
    }
    departure += 1;
  }
}

const input: [number, number[]] = getBuses('input.txt');
console.log(departureTime(input[0], input[1]));
