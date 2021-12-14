def read_input(filename):
    rules = {}
    with open(filename, "r") as fid:
        template = [char for char in fid.readline().strip()]
        fid.readline()
        for line in fid.readlines():
            pair, insert = line.strip().split(" -> ")
            rules[pair] = insert
    return template, rules


def polymerize(template, rules):
    polymer = []
    for i in range(len(template) - 1):
        polymer += [template[i], rules[template[i] + template[i + 1]]]
    polymer += [template[-1]]
    return polymer


def grow_polymer(template, rules, steps):
    for i in range(steps):
        template = polymerize(template, rules)
    element_counts = [template.count(elem) for elem in set(template)]
    return max(element_counts) - min(element_counts)


if __name__ == "__main__":
    test_template, test_rules = read_input("test_input.txt")
    assert grow_polymer(test_template, test_rules, 10) == 1588
    template, rules = read_input("input.txt")
    print(grow_polymer(template, rules, 10))
