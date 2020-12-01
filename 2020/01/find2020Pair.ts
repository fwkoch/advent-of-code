import * as fs from 'fs';

function readInput(filename: string): number[] {
    let input: string = fs.readFileSync(filename, 'utf8');
    let inputValues: string[] = input.split(/\r?\n/);
    return inputValues.map(x => +x);
}

function findPair(values: number[], target: number = 2020): number {
    let i: number;
    let j: number;
    for (i = 0; i < values.length; i++) {
        for (j = i + 1; j < values.length; j++) {
            if (values[i] + values[j] === target) {
                return values[i] * values[j];
            }
        }
    }
    throw 'target not found';
}

function findTrio(values: number[], target: number = 2020): number {
    let i: number;
    let j: number;
    let k: number;
    for (i = 0; i < values.length; i++) {
        for (j = i + 1; j < values.length; j++) {
            if (values[i] + values[j] >= target) {
                continue
            }
            for (k = j + 1; k < values.length; k++) {
                if (values[i] + values[j] + values[k] === target) {
                    return values[i] * values[j] * values[k];
                }
            }
        }
    }
    throw 'target not found';
}

// console.log(findPair(readInput('input.txt')));
console.log(findTrio(readInput('input.txt')));
