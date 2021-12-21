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


class Universe(BaseModel):

    p1: Pawn
    p2: Pawn
    next_player: int = 1

    def move(self, distance):
        if self.next_player == 1:
            self.p1.move(distance)
            self.next_player = 2
        else:
            self.p2.move(distance)
            self.next_player = 1

    def winner(self):
        if self.p1.score >= 21:
            return 1
        if self.p2.score >= 21:
            return 2
        return None

    def serialize(self):
        return (
            self.p1.score,
            self.p1.position,
            self.p2.score,
            self.p2.position,
            self.next_player,
        )

    @classmethod
    def deserialize(cls, value):
        return Universe(
            p1=Pawn(score=value[0], position=value[1]),
            p2=Pawn(score=value[2], position=value[3]),
            next_player=value[4],
        )

    def player_turn(self, universe_lookup, wins):
        current_state = self.serialize()
        current_count = universe_lookup[current_state]
        universes = [Universe.deserialize(current_state) for _ in range(3 * 3 * 3)]
        universes[0].move(3)
        [u.move(4) for u in universes[1:4]]
        [u.move(5) for u in universes[4:10]]
        [u.move(6) for u in universes[10:17]]
        [u.move(7) for u in universes[17:23]]
        [u.move(8) for u in universes[23:26]]
        universes[26].move(9)
        for universe in universes:
            state = universe.serialize()
            if universe.winner():
                wins[universe.winner()] += current_count
            elif state in universe_lookup:
                universe_lookup[state] += current_count
            else:
                universe_lookup[state] = current_count
        universe_lookup.pop(current_state)


def take_turns_dirac(pos1, pos2):
    starting_universe = Universe(p1=Pawn(position=pos1), p2=Pawn(position=pos2))
    universe_lookup = {
        starting_universe.serialize(): 1,
    }
    wins = {1: 0, 2: 0}
    while universe_lookup:
        Universe.deserialize(list(universe_lookup)[0]).player_turn(
            universe_lookup, wins
        )
    return wins


if __name__ == "__main__":
    test_p1 = Pawn(position=4)
    test_p2 = Pawn(position=8)
    assert take_turns(test_p1, test_p2) == 739785
    p1 = Pawn(position=2)
    p2 = Pawn(position=8)
    print(take_turns(p1, p2))

    print(take_turns_dirac(4, 8))
    print(take_turns_dirac(2, 8))
