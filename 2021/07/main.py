def read_input(filename):
    with open(filename, "r") as fid:
        return [int(val) for val in fid.readline().split(",")]


def fuel(positions, final, linear=True):
    if linear:
        return sum(abs(position - final) for position in positions)
    return sum(sum(range(abs(position - final) + 1)) for position in positions)


def least_fuel(positions, linear=True):
    total = None
    for i in range(min(positions), max(positions)):
        current_total = fuel(positions, i, linear)
        if total is None or current_total < total:
            total = current_total
    return total


if __name__ == "__main__":
    test_positions = read_input("test_input.txt")
    print(least_fuel(test_positions))
    positions = read_input("input.txt")
    print(least_fuel(positions))
    print(least_fuel(test_positions, linear=False))
    print(least_fuel(positions, linear=False))
