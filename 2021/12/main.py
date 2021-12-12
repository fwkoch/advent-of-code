from pydantic import BaseModel


class Cave(BaseModel):
    name: str
    connections: set[str] = set()

    @property
    def small(self):
        return self.name.lower() == self.name

    def routes(self, dest, path, caves):
        path = path + [self.name]
        if self.name == dest:
            return [path]
        paths = []
        for conn in self.connections:
            cave = caves[conn]
            if cave.small and cave.name in path:
                continue
            paths += cave.routes(dest, path, caves)
        return paths


def load_caves(filename):
    caves = {}
    with open(filename, "r") as fid:
        for line in fid:
            a, b = line.strip().split("-")
            if a not in caves:
                caves[a] = Cave(name=a)
            if b not in caves:
                caves[b] = Cave(name=b)
            caves[a].connections.add(b)
            caves[b].connections.add(a)
    return caves


if __name__ == "__main__":
    test_caves = load_caves("test_input_0.txt")
    assert len(test_caves["start"].routes("end", [], test_caves)) == 10
    test_caves = load_caves("test_input_1.txt")
    assert len(test_caves["start"].routes("end", [], test_caves)) == 19
    test_caves = load_caves("test_input_2.txt")
    assert len(test_caves["start"].routes("end", [], test_caves)) == 226
    caves = load_caves("input.txt")
    print(len(caves["start"].routes("end", [], caves)))
