children_cache = {}


def lanterfish_children(length):
    if length not in children_cache:
        children_cache[length] = [length - 1 - i for i in range(8, length, 7)]
    return children_cache[length]


def lanternfish_count(start, time):
    uncounted = [8 - start + time]
    counted = []
    while uncounted:
        uncounted += lanterfish_children(uncounted[0])
        counted.append(uncounted[0])
        uncounted = uncounted[1:]
    return len(counted)


def lanternfish_counts(starts, time):
    return sum(
        [starts.count(start) * lanternfish_count(start, time) for start in set(starts)]
    )


def read_input(filename):
    with open(filename, "r") as fid:
        return [int(val) for val in fid.readline().split(",")]


if __name__ == "__main__":
    test_fish = read_input("./test_input.txt")
    assert lanternfish_counts(test_fish, 18) == 26
    assert lanternfish_counts(test_fish, 80) == 5934
    fish = read_input("./input.txt")
    print(lanternfish_counts(fish, 80))
