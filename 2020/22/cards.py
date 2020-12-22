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

    def win_cards(self, cards):
        self.hand += sorted(cards)[::-1]

    def play_round(self, opponent):
        mine = self.play_card()
        theirs = opponent.play_card()
        if mine > theirs:
            self.win_cards([mine, theirs])
        else:
            opponent.win_cards([mine, theirs])

    def has_lost(self):
        return len(self.hand) == 0

    def score(self):
        score = 0
        for i, card in enumerate(self.hand[::-1]):
            score += card * (i + 1)
        return score

    def play_game(self, opponent):
        while not self.has_lost() and not opponent.has_lost():
            self.play_round(opponent)
        if self.has_lost():
            return opponent.score()
        return self.score()


if __name__ == "__main__":
    with open("input.txt", "r") as fid:
        hands = fid.read().split("\n\n")
    a = Hand.from_input(hands[0])
    b = Hand.from_input(hands[1])
    print(a.play_game(b))
