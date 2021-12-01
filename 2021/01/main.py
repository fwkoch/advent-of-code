def read_input(filename):
    with open(filename) as fid:
        depths = [int(line) for line in fid.readlines()]
    return depths


def count_increases(depths):
    count = 0
    for i in range(len(depths) - 1):
        if depths[i + 1] > depths[i]:
            count += 1
    return count


if __name__ == "__main__":
    assert count_increases(read_input("./test_input.txt")) == 7
    print(count_increases(read_input("./input.txt")))
