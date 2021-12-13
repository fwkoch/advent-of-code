def fold(dots, direction, axis):
    new_dots = set()
    for dot in dots:
        if direction == "y":
            if dot[1] <= axis:
                new_dots.add(dot)
            else:
                new_dots.add((dot[0], 2 * axis - dot[1]))
        elif direction == "x":
            if dot[0] <= axis:
                new_dots.add(dot)
            else:
                new_dots.add((2 * axis - dot[0], dot[1]))
    return new_dots


def read_input(filename):
    dots = set()
    instructions = []
    with open(filename, "r") as fid:
        for line in fid.readlines():
            if not line.strip():
                continue
            elif line.strip().startswith("fold along"):
                instructions.append(line.strip().split()[-1].split("="))
            else:
                dots.add(tuple([int(val) for val in line.strip().split(",")]))
    instructions = [(direction, int(axis)) for direction, axis in instructions]
    return dots, instructions

def print_dots(dots):
    xmax = max(dot[0] for dot in dots)
    ymax = max(dot[1] for dot in dots)
    grid = [['.' for _ in range(xmax + 1)] for _ in range(ymax + 1)]
    for dot in dots:
        grid[dot[1]][dot[0]] = '#'
    for row in grid:
        print(''.join(row))

def carry_out_instructions(dots, instructions):
    for instr in instructions:
        dots = fold(dots, instr[0], instr[1])
    print_dots(dots)


if __name__ == "__main__":
    test_dots, test_instr = read_input("test_input.txt")
    assert len(fold(test_dots, test_instr[0][0], test_instr[0][1])) == 17
    dots, instr = read_input("input.txt")
    print(len(fold(dots, instr[0][0], instr[0][1])))
    carry_out_instructions(test_dots, test_instr)
    carry_out_instructions(dots, instr)
