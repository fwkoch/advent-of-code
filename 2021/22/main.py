def read_line(line):
    on = line.split()[0] == "on"  # True if on, False if off
    xyz = []
    for val in line.split()[1].split(","):
        xyz.append([int(i) for i in val.split("=")[1].split("..")])
    return on, xyz


def read_input(filename):
    ons = []
    xyzs = []
    with open(filename, "r") as fid:
        for line in fid.readlines():
            on, xyz = read_line(line.strip())
            ons.append(on)
            xyzs.append(xyz)
    return ons, xyzs


def on_cubes(ons, xyzs):
    output = set()
    for i in range(len(ons)):
        on = ons[i]
        xyz = xyzs[i]
        for x in range(max(xyz[0][0], -50), min(xyz[0][1], 50) + 1):
            for y in range(max(xyz[1][0], -50), min(xyz[1][1], 50) + 1):
                for z in range(max(xyz[2][0], -50), min(xyz[2][1], 50) + 1):
                    if on:
                        output.add((x, y, z))
                    else:
                        output.discard((x, y, z))
    return len(output)


if __name__ == "__main__":
    test_ons, test_xyzs = read_input("test_input.txt")
    assert on_cubes(test_ons, test_xyzs) == 590784
    ons, xyzs = read_input("input.txt")
    print(on_cubes(ons, xyzs))
