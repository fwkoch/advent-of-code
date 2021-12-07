def read_input(filename):
    with open(filename, "r") as fid:
        return [int(val) for val in fid.readline().split(",")]


def fuel(positions, final):
    return sum(abs(position - final) for position in positions)


def least_fuel(positions):
    total = None
    for i in range(min(positions), max(positions)):
        current_total = fuel(positions, i)
        if total is None or current_total < total:
            total = current_total
    return total


if __name__ == "__main__":
    test_positions = read_input("test_input.txt")
    print(least_fuel(test_positions))
    positions = read_input("input.txt")
    print(least_fuel(positions))
