OPPOSITE = {
    "n": "s",
    "s": "n",
    "e": "w",
    "w": "e",
}


class Tile:
    def __init__(self, tile: str, grid=None):
        self.number = int(tile.split("\n")[0][5:-1])
        self.grid = grid or tile.split("\n")[1:]

    @property
    def n(self):
        return self.grid[0]

    @property
    def s(self):
        return self.grid[-1]

    @property
    def w(self):
        return "".join(row[0] for row in self.grid)

    @property
    def e(self):
        return "".join(row[-1] for row in self.grid)

    @property
    def edges(self):
        return [self.n, self.e, self.s, self.w]

    def rotate(self):
        self.grid = [
            "".join(row[i] for row in self.grid[::-1]) for i in range(len(self.grid[0]))
        ]

    def flip(self):
        self.grid = self.grid[::-1]

    def is_corner(self, tiles):
        edges = 0
        for direction in "n", "e", "s", "w":
            if self.neighbor(direction, tiles):
                edges += 1
        return edges == 2

    def neighbor(self, direction, tiles):
        for tile in tiles:
            if tile is self:
                continue
            for _ in range(2):
                for _ in range(4):
                    if getattr(self, direction) == getattr(tile, OPPOSITE[direction]):
                        return tile
                    tile.rotate()
                tile.flip()
        return None

    def total_pounds(self):
        count = 0
        for row in self.grid:
            count += len([char for char in row if char == "#"])
        return count


def get_edges_count(tiles):
    edges_count = {}
    for tile in tiles:
        for edge in tile.edges:
            if edge in edges_count:
                edges_count[edge] += 1
            else:
                edges_count[edge] = 1
    return edges_count


def build_grid(tiles):
    copy = [tile for tile in tiles]
    for tile in copy:
        if tile.is_corner(copy):
            current_tile = tile
            copy.remove(tile)
            break
    while not (current_tile.neighbor("e", copy) and current_tile.neighbor("s", copy)):
        current_tile.rotate()
    grid = [[current_tile]]
    while copy:
        current_tile = current_tile.neighbor("e", copy)
        while current_tile:
            grid[-1].append(current_tile)
            copy.remove(current_tile)
            current_tile = current_tile.neighbor("e", copy)
        current_tile = grid[-1][0].neighbor("s", copy)
        if current_tile:
            grid.append([current_tile])
            copy.remove(current_tile)
    output = []
    for row in grid:
        output += [""] * 8
        for cell in row:
            for i in range(8):
                output[-(8 - i)] += cell.grid[1 + i][1:-1]
    return Tile("Tile 1:", grid=output)


def find_dragons(tile):
    dragon_indices = [
        [18],
        [0, 5, 6, 11, 12, 17, 18, 19],
        [1, 4, 7, 10, 13, 16],
    ]
    count = 0
    for row in range(len(tile.grid) - 2):
        for col in range(0, len(tile.grid[0]) - 20):
            if any([tile.grid[row][col + di] != "#" for di in dragon_indices[0]]):
                continue
            if any([tile.grid[row + 1][col + di] != "#" for di in dragon_indices[1]]):
                continue
            if any([tile.grid[row + 2][col + di] != "#" for di in dragon_indices[2]]):
                continue
            count += 1
    return count


def find_dragons_any_orientation(tile):
    for _ in range(2):
        for _ in range(4):
            dragons = find_dragons(tile)
            if dragons:
                return dragons
            tile.rotate()
        tile.flip()
    raise ValueError("dragon situation")


if __name__ == "__main__":
    with open("input.txt", "r") as fid:
        tiles = [Tile(grid) for grid in fid.read().split("\n\n")]

    # multiply corners
    product = 1
    sanity_count = 0
    for tile in tiles:
        if tile.is_corner(tiles):
            product *= tile.number
            sanity_count += 1
    if sanity_count != 4:
        raise ValueError("too many corners")
    print(product)

    # count rough water
    grid = build_grid(tiles)
    dragons = find_dragons_any_orientation(grid)
    dragon_tiles = dragons * 15
    print(grid.total_pounds() - dragon_tiles)
