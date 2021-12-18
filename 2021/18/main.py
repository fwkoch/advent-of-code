from typing import Union

from pydantic import BaseModel


class Number(BaseModel):

    value: int

    def to_string(self):
        return str(self.value)

    def to_list(self):
        return [self]

    def magnitude(self):
        return self.value


class SnailfishNumber(BaseModel):

    left: BaseModel
    right: BaseModel

    @classmethod
    def from_input(cls, number):
        if number[0] == "[":
            left = cls.from_input(number[1:])
            right = cls.from_input(number[2 + len(left.to_string()) :])
        else:
            return Number(value=int(number.split(",", 1)[0].split("]", 1)[0]))
        return SnailfishNumber(left=left, right=right)

    def to_list(self):
        return self.left.to_list() + self.right.to_list()

    def to_string(self):
        return f"[{self.left.to_string()},{self.right.to_string()}]"

    def add(self, other):
        new = SnailfishNumber(left=self, right=other)
        return new.reduce()

    def find_nested(self, nesting=4):
        if isinstance(self.left, SnailfishNumber):
            if nesting == 1:
                return self.left, self
            else:
                out = self.left.find_nested(nesting - 1)
                if out[0]:
                    return out
        if isinstance(self.right, SnailfishNumber):
            if nesting == 1:
                return self.right, self
            else:
                return self.right.find_nested(nesting - 1)
        return None, None

    def explode(self, nesting=0):
        target, parent = self.find_nested()
        if not target:
            return self
        numbers = self.to_list()
        for ind in range(len(numbers)):
            if numbers[ind] is target.left and ind > 0:
                numbers[ind - 1].value += target.left.value
            if numbers[ind] is target.right and ind < len(numbers) - 1:
                numbers[ind + 1].value += target.right.value
        if parent.left is target:
            parent.left = Number(value=0)
        else:
            parent.right = Number(value=0)
        return self

    def split(self):
        before = self.to_string()
        if isinstance(self.left, Number) and self.left.value > 9:
            value = self.left.value
            self.left = SnailfishNumber(
                left=Number(value=value // 2),
                right=Number(value=value // 2 + value % 2),
            )
        elif isinstance(self.left, SnailfishNumber):
            self.left.split()
        if self.to_string() != before:
            return self
        if isinstance(self.right, Number) and self.right.value > 9:
            value = self.right.value
            self.right = SnailfishNumber(
                left=Number(value=value // 2),
                right=Number(value=value // 2 + value % 2),
            )
        elif isinstance(self.right, SnailfishNumber):
            self.right.split()
        return self

    def reduce(self):
        before = self.to_string()
        self.explode()
        if self.to_string() == before:
            self.split()
        if self.to_string() == before:
            return self
        return self.reduce()

    def magnitude(self):
        return 3 * self.left.magnitude() + 2 * self.right.magnitude()


def compute_sum(filename):
    with open(filename, "r") as fid:
        state = SnailfishNumber.from_input(fid.readline().strip())
        for line in fid.readlines():
            state = state.add(SnailfishNumber.from_input(line.strip()))
    return state.magnitude()


def compute_greatest_sum(filename):
    with open(filename, "r") as fid:
        inputs = [line.strip() for line in fid.readlines()]
    largest = 0
    for i in range(len(inputs)):
        for j in range(len(inputs)):
            if i == j:
                continue
            a = SnailfishNumber.from_input(inputs[i])
            b = SnailfishNumber.from_input(inputs[j])
            mag = a.add(b).magnitude()
            if mag > largest:
                largest = mag
    return largest


if __name__ == "__main__":
    a = SnailfishNumber.from_input("[[[[[9,8],1],2],3],4]")
    b = SnailfishNumber.from_input("[7,[6,[5,[4,[3,2]]]]]")
    c = SnailfishNumber.from_input("[[6,[5,[4,[3,2]]]],1]")
    d = SnailfishNumber.from_input("[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]")
    e = SnailfishNumber.from_input("[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]")
    assert a.find_nested()[0].to_string() == "[9,8]"
    assert b.find_nested()[0].to_string() == "[3,2]"
    assert c.find_nested()[0].to_string() == "[3,2]"
    assert d.find_nested()[0].to_string() == "[7,3]"
    assert e.find_nested()[0].to_string() == "[3,2]"
    assert a.explode().to_string() == "[[[[0,9],2],3],4]"
    assert b.explode().to_string() == "[7,[6,[5,[7,0]]]]"
    assert c.explode().to_string() == "[[6,[5,[7,0]]],3]"
    assert d.explode().to_string() == "[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]"
    assert e.explode().to_string() == "[[3,[2,[8,0]]],[9,[5,[7,0]]]]"
    f = SnailfishNumber.from_input("[[[[0,7],4],[15,[0,13]]],[1,1]]")
    assert f.split().to_string() == "[[[[0,7],4],[[7,8],[0,13]]],[1,1]]"
    assert (
        SnailfishNumber.from_input("[[[[4,3],4],4],[7,[[8,4],9]]]")
        .add(SnailfishNumber.from_input("[1, 1"))
        .to_string()
        == "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]"
    )
    assert SnailfishNumber.from_input("[9,1").magnitude() == 29
    assert SnailfishNumber.from_input("[[1,2],[[3,4],5]]").magnitude() == 143
    assert (
        SnailfishNumber.from_input("[[[[0,7],4],[[7,8],[6,0]]],[8,1]]").magnitude()
        == 1384
    )
    assert (
        SnailfishNumber.from_input("[[[[1,1],[2,2]],[3,3]],[4,4]]").magnitude() == 445
    )
    assert (
        SnailfishNumber.from_input("[[[[3,0],[5,3]],[4,4]],[5,5]]").magnitude() == 791
    )
    assert (
        SnailfishNumber.from_input("[[[[5,0],[7,4]],[5,5]],[6,6]]").magnitude() == 1137
    )
    assert (
        SnailfishNumber.from_input(
            "[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]"
        ).magnitude()
        == 3488
    )
    assert compute_sum("test_input.txt") == 4140
    print(compute_sum("input.txt"))

    assert compute_greatest_sum("test_input.txt") == 3993
    print(compute_greatest_sum("input.txt"))
