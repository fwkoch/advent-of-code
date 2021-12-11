def read_input(filename):
    with open(filename, "r") as fid:
        return [[int(val) for val in line.strip()] for line in fid.readlines()]


def step(octopi):
    flashes = set()
    for row in range(len(octopi)):
        for col in range(len(octopi[row])):
            if octopi[row][col] == 9:
                flashes.add((row, col))
            else:
                octopi[row][col] += 1
    while len(flashes):
        row, col = flashes.pop()
        octopi[row][col] = 0
        for i in -1, 0, 1:
            for j in -1, 0, 1:
                if not (0 <= row + i < len(octopi) and 0 <= col + j < len(octopi[row])):
                    continue
                val = octopi[row + i][col + j]
                if val == 0:
                    continue
                if val == 9:
                    flashes.add((row + i, col + j))
                else:
                    octopi[row + i][col + j] += 1
    count = sum([sum([val == 0 for val in row]) for row in octopi])
    return count, octopi


def count_flashes(octopi, steps):
    total = 0
    for i in range(steps):
        count, octopi = step(octopi)
        total += count
    return total


def find_simultaneous_flash(octopi):
    expected = len(octopi) * len(octopi[0])
    steps = 0
    while True:
        steps += 1
        count, octopi = step(octopi)
        if count == expected:
            break
    return steps


if __name__ == "__main__":
    test_oct = read_input("./test_input.txt")
    assert count_flashes(test_oct, 100) == 1656
    octopi = read_input("./input.txt")
    print(count_flashes(octopi, 100))
    test_oct = read_input("./test_input.txt")
    assert find_simultaneous_flash(test_oct) == 195
    octopi = read_input("./input.txt")
    print(find_simultaneous_flash(octopi))
