from pydantic import BaseModel


class BingoBoard(BaseModel):
    rows: list[list[int]] = []
    cols: list[list[int]] = []

    def all_vals(self):
        return set(sum(self.rows, []))

    def compute_cols(self):
        assert len(self.rows) == 5
        for row in self.rows:
            assert len(row) == 5
        self.cols = [[row[i] for row in self.rows] for i in range(5)]

    def win(self, callout):
        for vals in self.rows + self.cols:
            if len(set(callout).intersection(set(vals))) == len(vals):
                return callout[-1] * sum(self.all_vals().difference(set(callout)))
        return False


class BingoGame(BaseModel):
    boards: list[BingoBoard] = []
    callouts: list[int] = []

    def load(self, filename):
        with open(filename, "r") as fid:
            self.callouts = [int(val) for val in fid.readline().split(",")]
            fid.readline()
            board = BingoBoard()
            for line in fid.readlines():
                if line == "\n":
                    board.compute_cols()
                    self.boards.append(board)
                    board = BingoBoard()
                else:
                    board.rows.append([int(val) for val in line.split()])
            if board.rows:
                board.compute_cols()
                self.boards.append(board)

    def play(self):
        for i in range(1, len(self.callouts)):
            for board in self.boards:
                win = board.win(self.callouts[:i])
                if win:
                    return win

    def play_to_lose(self):
        for i in range(1, len(self.callouts)):
            remaining_boards = []
            for board in self.boards:
                win = board.win(self.callouts[:i])
                if not win:
                    remaining_boards.append(board)
                elif len(self.boards) == 1:
                    return win
            self.boards = remaining_boards


if __name__ == "__main__":
    test_game = BingoGame()
    test_game.load("./test_input.txt")
    assert test_game.play() == 4512
    game = BingoGame()
    game.load("./input.txt")
    print(game.play())
    test_game = BingoGame()
    test_game.load("./test_input.txt")
    assert test_game.play_to_lose() == 1924
    game = BingoGame()
    game.load("./input.txt")
    print(game.play_to_lose())
