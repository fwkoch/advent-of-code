from pydantic import BaseModel

class Amphipod(BaseModel):

    energy: int
    home: int
    position: tuple[int, int]
    movement: int = 0

    def move(self, new_position):
        self.movement += abs(new_position[0] - self.position[0])
        self.movement += abs(new_position[1] - self.position[1])
        self.position = new_position

class Amber(Amphipod):

    energy: int = 1
    home: int = 2

class Bronze(Amphipod):

    energy: int = 10
    home: int = 4

class Copper(Amphipod):

    energy: int = 100
    home: int = 6

class Desert(Amphipod):

    energy: int = 1000
    home: int = 8



class Hallway(BaseModel):
    """Hallway Layout:

          012345678910
         #############
    0    #...........#
    1    ###.#.#.#.###
    2      #.#.#.#.#
    3      #.#.#.#.#
    4      #.#.#.#.#
           #########
    """

    amphipods: list[Amphipod]
    room_size: int = 4
    hall_positions: list[int] = [0, 1, 3, 5, 7, 9, 10]

    def amphipod_at_position(self, position):
        for amphipod in self.amphipods:
            if amphipod.position == position:
                return amphipod
        return None

    def room_free(self, room):
        # print(room)
        if room not in range(2,10,2):
            raise ValueError(f'bad room {room}')
        amphipods_in_room = [
            amph for amph in self.amphipods if amph.position[0] == room
        ]
        # print(amphipods_in_room)
        if any([amph.home != room for amph in amphipods_in_room]):
            return False
        return (room, self.room_size - len(amphipods_in_room))

    def organized(self):
        rooms = [self.room_free(room) for room in range(2, 10, 2)]
        # print(rooms)
        return all(rooms) and all([avail == 0 for _, avail in rooms])

    def bad(self):
        hallway_amphipods = [amph for amph in self.amphipods if amph.position[1] == 0]
        for i in range(len(hallway_amphipods)):
            for j in range(len(hallway_amphipods)):
                if i == j:
                    continue
                ai = hallway_amphipods[i]
                aj = hallway_amphipods[j]
                # print(ai)
                # print(aj)
                if ai.home < aj.position[0] < ai.position[0] < aj.home:
                    return True
                if aj.home < ai.position[0] < aj.position[0] < ai.home:
                    return True
        return False

    def energy(self):
        return sum([amph.energy * amph.movement for amph in self.amphipods])


    def valid_moves(self, amphipod):
        # Amphipod settled in it's room
        if amphipod.position[0] == amphipod.home and self.room_free(amphipod.home):
            return []
        # Amphipod in a room
        if amphipod.position[1] > 0:
            # Stuck under another amphipod
            if self.amphipod_at_position((amphipod.position[0], amphipod.position[1] - 1)):
                return []
            # Move to the hallway
            positions = []
            for i in range(amphipod.position[0] + 1, 11):
                if i not in self.hall_positions:
                    continue
                if self.amphipod_at_position((i, 0)):
                    break
                positions.append((i, 0))
            for i in range(amphipod.position[0] - 1, -1, -1):
                if i not in self.hall_positions:
                    continue
                if self.amphipod_at_position((i, 0)):
                    break
                positions.append((i, 0))
            return positions
        # Amphipod in hallway
        if not self.room_free(amphipod.home):
            return []
        # self.printout()
        # print(amphipod.position)
        if amphipod.position[0] > amphipod.home:
            blockers = range(amphipod.home, amphipod.position[0])
        else:
            blockers = range(amphipod.position[0]+1, amphipod.home)
        # print(blockers)
        # input()
        if any([self.amphipod_at_position((x, 0)) for x in blockers]):
            # print('this one')
            return []
        room_free = self.room_free(amphipod.home)
        if not room_free:
            return []
        return [room_free]

    def all_moves(self):
        moves = []
        for amphipod in self.amphipods:
            moves += [(amphipod, position) for position in self.valid_moves(amphipod)]
        return moves

    def try_to_organize(self, start=False, stay_below=None):
        lowest_score = None
        # self.printout()
        # input()
        for i in range(len(self.all_moves())):
            hallway_copy = self.copy(deep=True)
            # hallway_copy.printout()
            # input()
            amph, pos = hallway_copy.all_moves()[i]
            amph.move(pos)
            if start:
                hallway_copy.printout()
                print(hallway_copy.energy())
            if hallway_copy.organized():
                # print('organized!')
                # hallway_copy.printout()
                # print(hallway_copy.energy())
                # input()
                if not lowest_score or lowest_score > hallway_copy.energy():
                    lowest_score = hallway_copy.energy()
            elif self.bad():
                continue
            elif not stay_below or hallway_copy.energy() < stay_below:
                score = hallway_copy.try_to_organize(stay_below=lowest_score or stay_below)
                if not score:
                    continue
                if not lowest_score or lowest_score > score:
                    lowest_score = score
        # if lowest_score:
        #     print(lowest_score)
        return lowest_score

    def printout(self):
        amph_lookup = {amph.position: amph for amph in self.amphipods}
        print('#############')
        for y in range(self.room_size + 1):
            line = '#'
            for x in range(11):
                if (x, y) in amph_lookup:
                    line += amph_lookup[(x, y)].__class__.__name__[0]
                else:
                    line += '.' if y == 0  or x in [2, 4, 6, 8] else '#'
            line += '#'
            print(line)
        print('#############')

    @classmethod
    def from_input(cls, filename):
        with open(filename, 'r') as fid:
            fid.readline()  # -> ############
            amphipods = []
            y = 0
            line = fid.readline()
            while set(line.strip()) != {'#'}:
                if y == 0:
                    indices = [0, 1, 3, 5, 7, 9, 10]
                else:
                    indices = [2, 4, 6, 8]
                for index in indices:
                    if line[index + 1] not in ['A', 'B', 'C', 'D']:
                        continue
                    if line[index + 1] == 'A':
                        species = Amber
                    elif line[index + 1] == 'B':
                        species = Bronze
                    elif line[index + 1] == 'C':
                        species = Copper
                    elif line[index + 1] == 'D':
                        species = Desert
                    amphipods.append(species(position=(index, y)))
                y += 1
                line = fid.readline()
            return cls(amphipods=amphipods, room_size=y-1)


if __name__ == "__main__":
    test_hallway = Hallway.from_input('test_input_0.txt')
    # print(test_hallway.try_to_organize(start=True))
    hallway = Hallway.from_input('input_0.txt')
    print(hallway.try_to_organize(start=True))



