class Circle(list):
    def __getitem__(self, key):
        if isinstance(key, slice):
            return [self[i] for i in range(key.start, key.stop)]
        return super().__getitem__(key % len(self))

    def pop(self, index=None):
        if index:
            index = index % len(self)
        return super().pop(index)

class Game:
    def __init__(self, cups_int):
        self.cups = Circle([int(cup) for cup in str(cups_int)])
        self.current = self.cups[0]

    @property
    def index(self):
        return self.cups.index(self.current)

    def move(self):
        print(f'cups: {self.cups} (current: {self.current})')
        pickup = [
            self.cups.pop(self.index + 1),
            self.cups.pop(self.index + 1),
            self.cups.pop(self.index + 1),
        ]
        print(f'pickup: {pickup}')
        next_value = self.current - 1
        for i in range(7):
            try:
                destination = (next_value - i - 1) % max(self.cups) + 1
                destination_index = self.cups.index(destination)
                print(f'dest: {destination}')
            except ValueError:
                continue
            for j in range(3):
                self.cups.insert(destination_index + 1, pickup.pop())
            break
        self.current = self.cups[(self.index + 1)]

    def labels_after_one(self):
        print(self.cups)
        out = ''
        for i in range(1, 9):
            out += str(self.cups[self.cups.index(1) + i])
        return out


if __name__ == '__main__':
    input_int = 459672813
    g = Game(input_int)
    for _ in range(100):
        g.move()
    print(g.labels_after_one())
