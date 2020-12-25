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
    tiles = {}
    for line in lines:
        tile = traverse(line.strip())
        if tile in tiles:
            tiles[tile] = not tiles[tile]
        else:
            tiles[tile] = True
    return tiles

def total_black_aka_true(tiles):
    return sum(val for val in tiles.values())

if __name__ == "__main__":
    with open('input.txt', 'r') as file:
        tiles = flip(file.readlines())
    print(total_black_aka_true(tiles))
