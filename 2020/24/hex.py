# locations are ~(e/w, n/s)

def traverse(line):
    loc = [0, 0]
    i = 0
    while i < len(line):
        char = line[i]
        i += 1
        if char == 'e':
            loc[0] += 2
            continue
        if char == 'w':
            loc[0] -= 2
            continue
        char += line[i]
        i += 1
        if char == 'se':
            loc[0] += 1
            loc[1] -= 1
        elif char == 'nw':
            loc[0] -= 1
            loc[1] += 1
        elif char == 'ne':
            loc[0] += 1
            loc[1] += 1
        elif char == 'sw':
            loc[0] -= 1
            loc[1] -= 1
        else:
            raise ValueError(char)
    return tuple(loc)

def flip(lines):
    black_tiles = set()
    for line in lines:
        black_tiles.symmetric_difference_update({traverse(line.strip())})
    return black_tiles

def adjacent(tile):
    deltas = [[2, 0], [1, 1], [-1, 1], [-2, 0], [-1, -1], [1, -1]]
    return {(tile[0] + d[0], tile[1] + d[1]) for d in deltas}

def daily_flip(tiles):
    new_tiles = set()
    for tile in tiles:
        neighbors = adjacent(tile)
        if 0 < len(neighbors.intersection(tiles)) < 3:
            new_tiles.add(tile)
        for neighbor in neighbors.difference(tiles):
            if len(adjacent(neighbor).intersection(tiles)) == 2:
                new_tiles.add(neighbor)
    return new_tiles

if __name__ == "__main__":
    with open('input.txt', 'r') as file:
        tiles = flip(file.readlines())
    print(len(tiles))
    for _ in range(100):
        tiles = daily_flip(tiles)
    print(len(tiles))
