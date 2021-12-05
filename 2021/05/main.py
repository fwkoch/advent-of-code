from pydantic import BaseModel


class VentLine(BaseModel):

    start: tuple[int, int]
    end: tuple[int, int]

    @classmethod
    def from_input(cls, line):
        return cls(
            start=tuple(int(val) for val in line.split()[0].split(",")),
            end=tuple(int(val) for val in line.split()[2].split(",")),
        )

    @property
    def horizontal(self):
        return self.start[1] == self.end[1]

    @property
    def vertical(self):
        return self.start[0] == self.end[0]

    def points(self, diagonals=False):
        if not (self.horizontal or self.vertical or diagonals):
            return []
        deltax = deltay = 0
        if not self.horizontal:
            deltay = (self.end[1] - self.start[1]) / abs(self.end[1] - self.start[1])
        if not self.vertical:
            deltax = (self.end[0] - self.start[0]) / abs(self.end[0] - self.start[0])
        steps = (
            abs(self.end[0] - self.start[0]) or abs(self.end[1] - self.start[1])
        ) + 1
        return [
            (self.start[0] + deltax * i, self.start[1] + deltay * i)
            for i in range(steps)
        ]

    def overlap(self, other, diagonals=False):
        return set(self.points(diagonals)).intersection(other.points(diagonals))


def load_vents(filename):
    lines = []
    with open(filename, "r") as fid:
        for line in fid.readlines():
            lines.append(VentLine.from_input(line))
    return lines


def count_overlaps(lines, diagonals=False):
    overlaps = set()
    for i in range(len(lines)):
        for other in lines[i + 1 :]:
            overlaps = overlaps.union(lines[i].overlap(other, diagonals))
    return len(overlaps)


if __name__ == "__main__":
    test_vents = load_vents("./test_input.txt")
    assert count_overlaps(test_vents) == 5
    vents = load_vents("./input.txt")
    print(count_overlaps(vents))
    assert count_overlaps(test_vents, diagonals=True) == 12
    print(count_overlaps(vents, diagonals=True))
