openers = ["(", "[", "{", "<"]
closers = [")", "]", "}", ">"]
scores = [3, 57, 1197, 25137]


def validate(line):
    line = line.strip()
    i = 0
    while len(line) > 0:
        if i >= len(line):
            return 0
        elif line[i] in openers:
            i += 1
        elif closers.index(line[i]) == openers.index(line[i - 1]):
            line = line[: i - 1] + line[i + 1 :]
            i -= 1
        else:
            return scores[closers.index(line[i])]
    return 0


def score_from_input(filename):
    with open(filename, "r") as fid:
        return sum([validate(line) for line in fid.readlines()])


def complete(line):
    line = line.strip()
    i = 0
    score = 0
    while len(line) > 0:
        if i >= len(line):
            break
        elif line[i] in openers:
            i += 1
        elif closers.index(line[i]) == openers.index(line[i - 1]):
            line = line[: i - 1] + line[i + 1 :]
            i -= 1
        else:
            return 0
    for char in line[::-1]:
        score = 5 * score + 1 + openers.index(char)
    return score


def score_from_completions(filename):
    with open(filename, "r") as fid:
        completions = [complete(line) for line in fid.readlines()]
        completions = [value for value in sorted(completions) if value != 0]
        return completions[(len(completions) - 1) // 2]


if __name__ == "__main__":
    assert score_from_input("test_input.txt") == 26397
    print(score_from_input("input.txt"))
    assert score_from_completions("test_input.txt") == 288957
    print(score_from_completions("input.txt"))
