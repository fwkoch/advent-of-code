import * as fs from 'fs';

function getBuses(filename: string): [number, number[]] {
  const input: string = fs.readFileSync(filename, 'utf8');
  const timestamp: number = +input.split('\n')[0];
  const buses: number[] = input.split('\n')[1].split(',')
    .filter((bus: string): boolean => bus !== 'x')
    .map((bus: string): number => +bus);
  return [timestamp, buses];
}

function getRelativeDepartures(filename: string): number[] {
  const input: string = fs.readFileSync(filename, 'utf8');
  const departures: number[] = input.split('\n')[1].split(',')
    .map((bus: string, i: number): number => ((bus === 'x') ? -1 : i))
    .filter((bus: number): boolean => bus !== 1);
  return departures;
}

function departureTime(timestamp: number, buses: number[]) {
  let departure: number = timestamp;
  while (true) {
    for (let i: number = 0; i < buses.length; i += 1) {
      if (departure % buses[i] === 0) {
        return buses[i] * (departure - timestamp);
      }
    }
    departure += 1;
  }
}

function checkDeltas(timestamp: number, deltas: number[], starts: number[]) {
  for (let i = 0; i < deltas.length; i += 1) {
    if ((timestamp - starts[i]) % deltas[i] !== 0) {
      return false;
    }
  }
  return true;
}

function brutish2(ts: number, deltas: number[], starts: number[]) {
  let timestamp: number = ts;
  while (!checkDeltas(timestamp, deltas, starts)) {
    timestamp += deltas[0];
  }
  return timestamp;
}

function lessBrutish2(buses: number[], departures: number[]): number {
  const starts: number[] = [];
  const deltas: number[] = [];
  let first: number;
  let second: number;
  for (let i: number = 1; i < buses.length; i += 1) {
    first = brutish2(departures[0], [buses[0], buses[i]], [departures[0], departures[i]]);
    second = brutish2(first + buses[0], [buses[0], buses[i]], [departures[0], departures[i]]);
    starts.push(first);
    deltas.push(second - first);
  }
  if (starts.length === 1) {
    return starts[0];
  }
  return lessBrutish2(deltas, starts);
}

// The only difference between the following functions and the previous
// functions is the - on line 34 vs the + on line 70. I don't quite
// understand why I need that... it made sense while I was working
// my way through. I'm sure there's a sensible way to refactor this
// down to a single function (or group of functions or whatever).
function checkTimestamp(timestamp: number, buses: number[], departures: number[]): boolean {
  for (let i = 0; i < buses.length; i += 1) {
    if ((timestamp + departures[i]) % buses[i] !== 0) {
      return false;
    }
  }
  return true;
}

function brutish(ts: number, buses: number[], departures: number[]) {
  let timestamp: number = ts;
  while (!checkTimestamp(timestamp, buses, departures)) {
    timestamp += buses[0];
  }
  return timestamp;
}

function lessBrutish(buses: number[], departures: number[]): number {
  const starts: number[] = [];
  const deltas: number[] = [];
  let first: number;
  let second: number;
  for (let i: number = 1; i < buses.length; i += 1) {
    first = brutish(departures[0], [buses[0], buses[i]], [departures[0], departures[i]]);
    second = brutish(first + buses[0], [buses[0], buses[i]], [departures[0], departures[i]]);
    starts.push(first);
    deltas.push(second - first);
  }
  return lessBrutish2(deltas, starts);
}

const input: [number, number[]] = getBuses('input.txt');
console.log(departureTime(input[0], input[1]));
console.log(lessBrutish(input[1], getRelativeDepartures('input.txt')))
