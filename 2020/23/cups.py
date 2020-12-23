class Link:

    registry = {}

    def __init__(self, value, prev):
        self.value = value
        self.prev = prev
        self._next = None
        self.registry.update({value: self})

    @property
    def prev(self):
        return self._prev

    @prev.setter
    def prev(self, value):
        if value:
            value._next = self
        self._prev = value

    @property
    def next(self):
        return self._next

    @next.setter
    def next(self, value):
        if value:
            value._prev = self
        self._next = value

    def find(self, value):
        return self.registry[value]

    def to_list(self):
        out = [self.value]
        this = self.next
        while this is not self and this is not None:
            out.append(this.value)
            this = this.next
        return out


class Game:
    @staticmethod
    def from_input(cups_int):
        return Game([int(cup) for cup in str(cups_int)])

    def __init__(self, input_list):
        self.current = prev = Link(input_list[0], None)
        for val in input_list[1:]:
            prev = Link(val, prev)
        prev.next = self.current
        self.max = max(input_list)

    def move(self):
        pickup = self.current.next
        self.current.next = self.current.next.next.next.next
        pickup.prev = None
        pickup.next.next.next = None
        pickup_list = pickup.to_list()
        next_value = self.current.value - 1
        for i in range(4):
            destination = (next_value - i - 1) % self.max + 1
            if destination not in pickup_list:
                destination_cup = self.current.find(destination)
                pickup.next.next.next = destination_cup.next
                destination_cup.next = pickup
                break
        self.current = self.current.next

    def labels_after_one(self):
        cups = self.current.find(1).to_list()
        out = ""
        for i in range(1, 9):
            out += str(cups[i])
        return out

    def product_after_one(self):
        cup_one = self.current.find(1)
        return cup_one.next.value * cup_one.next.next.value


class BigGame(Game):
    @staticmethod
    def from_input(cups_int):
        return Game([int(cup) for cup in str(cups_int)] + list(range(10, 1000001)))


if __name__ == "__main__":
    input_int = 459672813
    g = Game.from_input(input_int)
    for _ in range(100):
        g.move()
    print(g.labels_after_one())
    bg = BigGame.from_input(input_int)
    for _ in range(10000000):
        bg.move()
    print(bg.product_after_one())
