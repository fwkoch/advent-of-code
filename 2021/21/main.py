from pydantic import BaseModel


class Pawn(BaseModel):

    score: int = 0
    position: int

    def move(self, spaces):
        self.position += spaces
        while self.position > 10:
            self.position = self.position - 10
        self.score += self.position
        return self.score


class DeterministicDie(BaseModel):

    current_value: int = 1
    roll_count: int = 0

    def roll(self):
        result = self.current_value
        self.current_value += 1
        if self.current_value == 101:
            self.current_value = 1
        self.roll_count += 1
        return result


def take_turns(p1, p2):
    die = DeterministicDie()
    while True:
        p1.move(die.roll() + die.roll() + die.roll())
        if p1.score >= 1000:
            return die.roll_count * p2.score
        p2.move(die.roll() + die.roll() + die.roll())
        if p2.score >= 1000:
            return die.roll_count * p1.score


if __name__ == "__main__":
    test_p1 = Pawn(position=4)
    test_p2 = Pawn(position=8)
    assert take_turns(test_p1, test_p2) == 739785
    p1 = Pawn(position=2)
    p2 = Pawn(position=8)
    print(take_turns(p1, p2))
