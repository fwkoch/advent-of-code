from typing import Optional

from pydantic import BaseModel


class Position(BaseModel):

    value: int
    location: tuple[int, int]
    minimum_risk: Optional[int] = None

    def get_neighbors(self, positions):
        neighbors = [
            positions.get((self.location[0] - 1, self.location[1]), None),
            positions.get((self.location[0] + 1, self.location[1]), None),
            positions.get((self.location[0], self.location[1] - 1), None),
            positions.get((self.location[0], self.location[1] + 1), None),
        ]
        return [n for n in neighbors if n is not None]

    def compute_minimum_risk(self, positions):
        neighbor_risks = [
            n.minimum_risk
            for n in self.get_neighbors(positions)
            if n.minimum_risk is not None
        ]
        if len(neighbor_risks) == 0:
            return self.minimum_risk
        new_minimum_risk = min(neighbor_risks) + self.value
        if self.minimum_risk is None or self.minimum_risk > new_minimum_risk:
            self.minimum_risk = new_minimum_risk
        return self.minimum_risk


def read_input(filename):
    with open(filename, "r") as fid:
        rows = [line.strip() for line in fid.readlines()]
    positions = {}
    for row in range(len(rows)):
        for col in range(len(rows[row])):
            location = (row, col)
            positions[location] = Position(value=int(rows[row][col]), location=location)
    positions[(0, 0)].minimum_risk = 0
    return positions


def grow_cave(positions):
    original_dims = (
        max([p[0] for p in positions]) + 1,
        max([p[1] for p in positions]) + 1,
    )
    new_positions = {}
    for i in range(5):
        for j in range(5):
            for position in positions:
                new_position = (
                    i * original_dims[0] + position[0],
                    j * original_dims[1] + position[1],
                )
                new_value = (positions[position].value + i + j - 1) % 9 + 1
                new_positions[new_position] = Position(
                    value=new_value, location=new_position
                )
    new_positions[(0, 0)].minimum_risk = 0
    return new_positions


def iterate_for_minimum_risks(positions):
    something_changed = True
    while something_changed:
        something_changed = False
        for position in positions.values():
            if position.minimum_risk != position.compute_minimum_risk(positions):
                something_changed = True
    bottom_right = (max([p[0] for p in positions]), max([p[1] for p in positions]))
    return positions[bottom_right].minimum_risk


if __name__ == "__main__":
    test_positions = read_input("test_input.txt")
    assert iterate_for_minimum_risks(test_positions) == 40
    positions = read_input("input.txt")
    print(iterate_for_minimum_risks(positions))
    test_positions = grow_cave(test_positions)
    assert iterate_for_minimum_risks(test_positions) == 315
    positions = grow_cave(positions)
    print(iterate_for_minimum_risks(positions))
