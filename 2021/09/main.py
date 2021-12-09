def read_input(filename):
    heightmap = []
    with open(filename, 'r') as fid:
        for line in fid.readlines():
            heightmap.append([int(val) for val in line.strip()])
    return heightmap


def minima(heightmap):
    minima = []
    for row in range(len(heightmap)):
        for col in range(len(heightmap[row])):
            val = heightmap[row][col]
            print(val)
            if row - 1 >= 0 and heightmap[row - 1][col] <= val:
                continue
            if row + 1 < len(heightmap) and heightmap[row + 1][col] <= val:
                continue
            if col - 1 >= 0 and heightmap[row][col - 1] <= val:
                continue
            if col + 1 < len(heightmap[row]) and heightmap[row][col + 1] <= val:
                continue
            minima.append(val)
    print(minima)
    return sum(minima) + len(minima)

if __name__ == "__main__":
    test_heightmap = read_input('test_input.txt')
    assert minima(test_heightmap) == 15
    heightmap = read_input('input.txt')
    print(minima(heightmap))
