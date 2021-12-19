from pydantic import BaseModel


class Scanner(BaseModel):

    beacons: list[tuple[int, int, int]]

    def distance_sets(self):
        output = {}
        for beacon in self.beacons:
            output[beacon] = [
                set(
                    [
                        abs(beacon[0] - b[0]),
                        abs(beacon[1] - b[1]),
                        abs(beacon[2] - b[2]),
                    ]
                )
                for b in self.beacons
            ]
        return output

    def compare(self, other):
        self_sets = self.distance_sets()
        other_sets = other.distance_sets()
        for beacon in self_sets:
            for other_beacon in other_sets:
                matches = list_of_sets(self_sets[beacon], other_sets[other_beacon])
                if len(matches) >= 12:
                    return [(self.beacons[i], other.beacons[j]) for i, j in matches]
        return None


def list_of_sets(a, b):
    matches = []
    for s in a:
        if s in b:
            matches.append((a.index(s), b.index(s)))
    return matches


def find_rotation(pairs):
    # This also includes mirrors... there could be a stinky input where
    # this is a problem. Hoping not though!
    orders = [[0, 1, 2], [1, 2, 0], [2, 0, 1], [0, 2, 1], [2, 1, 0], [1, 0, 2]]
    signs = [
        [1, 1, 1],
        [-1, 1, 1],
        [1, -1, 1],
        [1, 1, -1],
        [1, -1, -1],
        [-1, 1, -1],
        [-1, -1, 1],
        [-1, -1, -1],
    ]
    a, b = pairs[0]
    c, d = pairs[1]
    for order in orders:
        for sign in signs:
            diff0 = [a[i] - b[order[i]] * sign[i] for i in range(3)]
            diff1 = [c[i] - d[order[i]] * sign[i] for i in range(3)]
            if diff0 == diff1:
                return order, sign, diff0
    raise ValueError(":(")


def read_input(filename):
    scanners = []
    beacons = []
    with open(filename, "r") as fid:
        for line in fid.readlines():
            if line.startswith("---"):
                beacons = []
            elif not line.strip():
                scanners.append(Scanner(beacons=beacons))
            else:
                beacons.append(tuple(int(val) for val in line.strip().split(",")))
        scanners.append(Scanner(beacons=beacons))
    return scanners


def find_all_beacons(scanners):
    start = scanners[0]
    remaining = scanners[1:]
    scanner_locations = [[0, 0, 0]]
    while remaining:
        print(len(remaining))
        for scanner in remaining:
            pairs = start.compare(scanner)
            if not pairs:
                continue
            order, sign, diff = find_rotation(pairs)
            scanner_locations.append(diff)
            beacons = [
                tuple([diff[i] + beacon[order[i]] * sign[i] for i in range(3)])
                for beacon in scanner.beacons
            ]
            start.beacons = list(set(start.beacons + beacons))
            remaining.remove(scanner)
    return start.beacons, scanner_locations


def manhattan_distance(a, b):
    return sum([abs(a[i] - b[i]) for i in range(3)])


def greatest_distance(beacons):
    max_dist = 0
    for i in range(len(beacons)):
        for j in range(i + 1, len(beacons)):
            distance = manhattan_distance(beacons[i], beacons[j])
            if distance > max_dist:
                max_dist = distance
    return max_dist


if __name__ == "__main__":
    test_scanners = read_input("test_input.txt")
    test_all_beacons, test_all_scanners = find_all_beacons(test_scanners)
    assert len(test_all_beacons) == 79
    assert greatest_distance(test_all_scanners) == 3621
    scanners = read_input("input.txt")
    all_beacons, all_scanners = find_all_beacons(scanners)
    print(len(all_beacons))
    print(greatest_distance(all_scanners))
