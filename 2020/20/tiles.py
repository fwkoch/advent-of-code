class Tile:
    def __init__(self, tile: str):
        self.number = int(tile.split("\n")[0][5:-1])
        self.grid = tile.split("\n")[1:]
        self.n = self.grid[0]
        self.s = self.grid[-1]
        self.w = "".join(row[0] for row in self.grid)
        self.e = "".join(row[-1] for row in self.grid)

    @property
    def edges(self):
        return [self.n, self.e, self.s, self.w]

    def rotate(self):
        e = self.n
        self.n = self.w[::-1]
        self.w = self.s
        self.s = self.e[::-1]
        self.e = e

    def is_corner(self, tiles):
        edges = 0
        for direction in "n", "e", "s", "w":
            if len(self.neighbors(direction, tiles)) == 0:
                edges += 1
        return edges > 1

    def is_edge(self, tiles):
        for direction in "n", "e", "s", "w":
            if len(self.neighbors(direction, tiles)) == 0:
                return True
        return False

    def neighbors(self, direction, tiles):
        neighbors = []
        for tile in tiles:
            if tile is self:
                continue
            if getattr(self, direction) in tile.edges:
                neighbors.append(tile)
                continue
            tile.rotate()
            tile.rotate()
            if getattr(self, direction) in tile.edges:
                neighbors.append(tile)
        return neighbors


def get_edges_count(tiles):
    edges_count = {}
    for tile in tiles:
        for edge in tile.edges:
            if edge in edges_count:
                edges_count[edge] += 1
            else:
                edges_count[edge] = 1
    return edges_count


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

    print(sanity_count)
    if sanity_count != 4:
        raise ValueError("too many corners")
    print(product)
