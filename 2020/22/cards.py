class Hand:
    def __init__(self, player, hand):
        self.player = player
        self.hand = hand

    @staticmethod
    def from_input(inp):
        return Hand(
            player=inp.split("\n")[0].split(":")[0],
            hand=[int(card) for card in inp.split("\n")[1:]],
        )

    def play_card(self):
        card = self.hand[0]
        self.hand = self.hand[1:]
        return card

    def win_cards(self, winning, losing):
        self.hand += [winning, losing]

    def play_round(self, opponent, recursive=False):
        mine = self.play_card()
        theirs = opponent.play_card()
        if recursive and len(self.hand) >= mine and len(opponent.hand) >= theirs:
            winner = self.play_game(opponent, True, [mine, theirs])
            if winner.player == self.player:
                self.win_cards(mine, theirs)
            else:
                opponent.win_cards(theirs, mine)
        else:
            if mine > theirs:
                self.win_cards(mine, theirs)
            else:
                opponent.win_cards(theirs, mine)

    def has_lost(self):
        return len(self.hand) == 0

    def score(self):
        score = 0
        for i, card in enumerate(self.hand[::-1]):
            score += card * (i + 1)
        return score

    def play_game(self, opponent, recursive=False, lengths=None):
        if recursive and lengths:
            player_0 = Hand(self.player, self.hand[: lengths[0]])
            player_1 = Hand(opponent.player, opponent.hand[: lengths[1]])
        else:
            player_0 = self
            player_1 = opponent
        hands = set()
        while not player_0.has_lost() and not player_1.has_lost():
            if recursive:
                hands_state = (tuple(player_0.hand), tuple(player_1.hand))
                if hands_state in hands:
                    return player_0
                hands.add(hands_state)
            player_0.play_round(player_1, recursive)
        if player_0.has_lost():
            return player_1
        return player_0


if __name__ == "__main__":
    with open("input.txt", "r") as fid:
        hands = fid.read().split("\n\n")
    a = Hand.from_input(hands[0])
    b = Hand.from_input(hands[1])
    print(a.play_game(b).score())
    a = Hand.from_input(hands[0])
    b = Hand.from_input(hands[1])
    print(a.play_game(b, recursive=True).score())
