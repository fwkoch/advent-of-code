import * as fs from 'fs';

function getGameStart(filename: string): [{[num: string]: number[]}, string, number] {
  const input: string = fs.readFileSync(filename, 'utf8');
  const spoken: {[num: string]: number[]} = {};
  let previous: string = '';
  let turn: number = 0;
  input.split(',').forEach((value: string, i: number): void => {
    spoken[value] = [i];
    previous = value;
    turn = i;
  });
  return [spoken, previous, turn + 1];
}

function playGame(history: {[num: string]: number[]}, previous: string, start: number) {
  const game: {[num: string]: number[]} = history;
  let prev: string = previous;
  let turn: number = start;
  let current: number = -1;
  let gamePrev: number[];
  while (turn < 30000000) {
    if (turn % 10000 === 0) {
      console.log(turn);
    }
    gamePrev = game[prev];
    if (gamePrev.length === 1) {
      current = 0;
    } else {
      current = gamePrev[1] - gamePrev[0];
    }
    prev = current.toString();
    if (history[prev] === undefined) {
      game[prev] = [turn];
    } else {
      game[prev] = [game[prev][game[prev].length - 1], turn];
    }
    turn += 1;
  }
  return current;
}

const [history, previous, start]: [{[num: string]: number[]}, string, number] = getGameStart('input.txt');
console.log(playGame(history, previous, start));
