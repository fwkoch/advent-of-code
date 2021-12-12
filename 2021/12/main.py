from pydantic import BaseModel


def has_no_duplicate_smalls(path):
    smalls = [name for name in path if name.lower() == name]
    return len(smalls) == len(set(smalls))


class Cave(BaseModel):
    name: str
    connections: set[str] = set()

    @property
    def small(self):
        return self.name.lower() == self.name

    def routes(self, dest, path, caves, allow_double_visit=False):
        path = path + [self.name]
        if self.name == dest:
            return [path]
        paths = []
        for conn in self.connections:
            if conn == "start":
                continue
            cave = caves[conn]
            if allow_double_visit and has_no_duplicate_smalls(path):
                pass
            elif cave.small and cave.name in path:
                continue
            paths += cave.routes(dest, path, caves, allow_double_visit)
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

    test_caves = load_caves("test_input_0.txt")
    assert len(test_caves["start"].routes("end", [], test_caves, True)) == 36
    test_caves = load_caves("test_input_1.txt")
    assert len(test_caves["start"].routes("end", [], test_caves, True)) == 103
    test_caves = load_caves("test_input_2.txt")
    assert len(test_caves["start"].routes("end", [], test_caves, True)) == 3509
    caves = load_caves("input.txt")
    print(len(caves["start"].routes("end", [], caves, True)))
