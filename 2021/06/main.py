def read_input(filename):
    with open(filename, "r") as fid:
        return [int(val) for val in fid.readline().split(",")]


def lanternfish_counts(starts, time):
    ages = {age: starts.count(age) for age in range(9)}
    for i in range(time):
        new_ages = {age: 0 for age in range(9)}
        new_ages[8] += ages[0]
        new_ages[6] += ages[0]
        for i in range(8):
            new_ages[i] += ages[i + 1]
        ages = new_ages
    return sum(ages.values())


if __name__ == "__main__":
    test_fish = read_input("./test_input.txt")
    assert lanternfish_counts(test_fish, 18) == 26
    assert lanternfish_counts(test_fish, 80) == 5934
    assert lanternfish_counts(test_fish, 256) == 26984457539
    fish = read_input("./input.txt")
    print(lanternfish_counts(fish, 80))
    print(lanternfish_counts(fish, 256))
