from pydantic import BaseModel


class Probe(BaseModel):

    position: tuple[int, int] = (0, 0)
    velocity: tuple[int, int]

    def step(self):
        self.position = (
            self.position[0] + self.velocity[0],
            self.position[1] + self.velocity[1],
        )
        self.velocity = (
            self.velocity[0] - self.velocity[0] / abs(self.velocity[0])
            if self.velocity[0]
            else 0,
            self.velocity[1] - 1,
        )

    def height_on_hit(self, target):
        max_height = self.position[1]
        while self.position[0] <= target[1] and self.position[1] >= target[2]:
            if (
                target[0] <= self.position[0] <= target[1]
                and target[2] <= self.position[1] <= target[3]
            ):
                return max_height
            self.step()
            max_height = max(max_height, self.position[1])
        return None


def launch_a_ton_of_probes(target):
    x_vel_range = range(target[1] + 1)
    y_vel_range = range(target[2], target[3] + target[1] + 1)
    max_height = target[2]
    for x in x_vel_range:
        for y in y_vel_range:
            probe = Probe(velocity=(x, y))
            height = probe.height_on_hit(target)
            if height is not None and height > max_height:
                max_height = height
    return max_height


if __name__ == "__main__":
    test_area = (20, 30, -10, -5)
    assert Probe(velocity=(7, 2)).height_on_hit(test_area) == 3
    assert Probe(velocity=(6, 3)).height_on_hit(test_area) == 6
    assert Probe(velocity=(9, 0)).height_on_hit(test_area) == 0
    assert Probe(velocity=(17, -4)).height_on_hit(test_area) is None
    assert launch_a_ton_of_probes(test_area) == 45
    area = (195, 238, -93, -67)
    print(launch_a_ton_of_probes(area))
