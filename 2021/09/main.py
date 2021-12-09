def read_input(filename):
    heightmap = []
    with open(filename, "r") as fid:
        for line in fid.readlines():
            heightmap.append([int(val) for val in line.strip()])
    return heightmap


def minima(heightmap):
    minima = []
    minima_inds = []
    for row in range(len(heightmap)):
        for col in range(len(heightmap[row])):
            val = heightmap[row][col]
            if row - 1 >= 0 and heightmap[row - 1][col] <= val:
                continue
            if row + 1 < len(heightmap) and heightmap[row + 1][col] <= val:
                continue
            if col - 1 >= 0 and heightmap[row][col - 1] <= val:
                continue
            if col + 1 < len(heightmap[row]) and heightmap[row][col + 1] <= val:
                continue
            minima.append(val)
            minima_inds.append((row, col))
    return sum(minima) + len(minima), minima_inds


def basin(heightmap, minima_ind, basin_inds=None):
    if basin_inds is None:
        basin_inds = set()
    basin_inds.add(minima_ind)
    rows = [minima_ind[0] - 1, minima_ind[0] + 1, minima_ind[0], minima_ind[0]]
    cols = [minima_ind[1], minima_ind[1], minima_ind[1] - 1, minima_ind[1] + 1]
    for i in range(4):
        if rows[i] < 0 or cols[i] < 0:
            continue
        if rows[i] >= len(heightmap) or cols[i] >= len(heightmap[rows[i]]):
            continue
        if heightmap[rows[i]][cols[i]] == 9:
            continue
        inds = (rows[i], cols[i])
        if inds not in basin_inds:
            basin_inds.add(inds)
            basin(heightmap, inds, basin_inds)
    return basin_inds


def basin_product(heightmap):
    minima_inds = minima(heightmap)[1]
    areas = []
    for ind in minima_inds:
        areas.append(len(basin(heightmap, ind)))
    areas = sorted(areas)
    return areas[-1] * areas[-2] * areas[-3]


if __name__ == "__main__":
    test_heightmap = read_input("test_input.txt")
    assert minima(test_heightmap)[0] == 15
    heightmap = read_input("input.txt")
    print(minima(heightmap)[0])
    assert basin_product(test_heightmap) == 1134
    print(basin_product(heightmap))
