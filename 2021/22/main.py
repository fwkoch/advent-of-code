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


def volume(xyz):
    return (
        (xyz[0][1] - xyz[0][0] + 1)
        * (xyz[1][1] - xyz[1][0] + 1)
        * (xyz[2][1] - xyz[2][0] + 1)
    )


def overlap(xyza, xyzb):
    xyzc = []
    for dim in range(3):
        if xyza[dim][0] <= xyzb[dim][0] <= xyza[dim][1]:
            xyzc.append([xyzb[dim][0], min(xyza[dim][1], xyzb[dim][1])])
        elif xyza[dim][0] <= xyzb[dim][1] <= xyza[dim][1]:
            xyzc.append([xyza[dim][0], min(xyza[dim][1], xyzb[dim][1])])
        elif xyzb[dim][0] < xyza[dim][0] and xyzb[dim][1] > xyza[dim][1]:
            xyzc.append(xyza[dim])
        else:
            return []
    return xyzc


def split(xyz, overlap):
    new_xyzs = []
    new_xlims = [xyz[0][0], xyz[0][1]]
    if xyz[0][0] < overlap[0][0]:
        new_xyzs.append([[xyz[0][0], overlap[0][0] - 1], xyz[1], xyz[2]])
        new_xlims[0] = overlap[0][0]
    if overlap[0][1] < xyz[0][1]:
        new_xyzs.append([[overlap[0][1] + 1, xyz[0][1]], xyz[1], xyz[2]])
        new_xlims[1] = overlap[0][1]
    new_ylims = [xyz[1][0], xyz[1][1]]
    if xyz[1][0] < overlap[1][0]:
        new_xyzs.append([new_xlims, [xyz[1][0], overlap[1][0] - 1], xyz[2]])
        new_ylims[0] = overlap[1][0]
    if overlap[1][1] < xyz[1][1]:
        new_xyzs.append([new_xlims, [overlap[1][1] + 1, xyz[1][1]], xyz[2]])
        new_ylims[1] = overlap[1][1]
    if xyz[2][0] < overlap[2][0]:
        new_xyzs.append([new_xlims, new_ylims, [xyz[2][0], overlap[2][0] - 1]])
    if overlap[2][1] < xyz[2][1]:
        new_xyzs.append([new_xlims, new_ylims, [overlap[2][1] + 1, xyz[2][1]]])
    return new_xyzs


def maybe_better_on_cubes(ons, xyzs):
    on_cubes = []
    for i in range(len(ons)):
        on = ons[i]
        xyz = xyzs[i]
        if on:
            on_cubes.append(xyz)
        else:
            add = []
            remove = []
            for cube in on_cubes:
                cube_overlap = overlap(xyz, cube)
                if cube_overlap:
                    remove.append(cube)
                    add += split(cube, cube_overlap)
            for cube in remove:
                on_cubes.remove(cube)
            on_cubes += add
    final_cubes = []
    while on_cubes:
        xyz = on_cubes[0]
        on_cubes = on_cubes[1:]
        for fc in final_cubes:
            cube_overlap = overlap(xyz, fc)
            if cube_overlap:
                on_cubes += split(xyz, fc)
                break
        else:
            final_cubes.append(xyz)
    return sum([volume(cube) for cube in final_cubes])


if __name__ == "__main__":
    test_ons, test_xyzs = read_input("test_input.txt")
    assert on_cubes(test_ons, test_xyzs) == 590784
    ons, xyzs = read_input("input.txt")
    print(on_cubes(ons, xyzs))

    assert maybe_better_on_cubes(test_ons, test_xyzs) == 590784
    test_ons, test_xyzs = read_input("test_input_2.txt")
    assert maybe_better_on_cubes(test_ons, test_xyzs) == 2758514936282235
    print(maybe_better_on_cubes(ons, xyzs))
