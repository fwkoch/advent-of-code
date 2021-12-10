openers = ['(', '[', '{', '<']
closers = [')', ']', '}', '>']
scores = [3, 57, 1197, 25137]

def validate(line):
    line = line.strip()
    i = 0
    while len(line) > 0:
        print(line)
        if i >= len(line):
            return 0
        elif line[i] in openers:
            i += 1
        elif closers.index(line[i]) == openers.index(line[i-1]):
            line = line[:i-1] + line[i+1:]
            i -= 1
        else:
            return scores[closers.index(line[i])]
    return 0

def score_from_input(filename):
    with open(filename, 'r') as fid:
        return sum([validate(line) for line in fid.readlines()])



if __name__ == "__main__":
    assert score_from_input('test_input.txt') == 26397
    print(score_from_input('input.txt'))
