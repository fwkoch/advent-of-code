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


def dont_really_polymerize(rules, pair_counts):
    new_counts = {pair: 0 for pair in pair_counts}
    for pair, count in pair_counts.items():
        new_counts[pair[0] + rules[pair]] += count
        new_counts[rules[pair] + pair[1]] += count
    return new_counts


def grow_polymer(template, rules, steps):
    for _ in range(steps):
        template = polymerize(template, rules)
    element_counts = [template.count(elem) for elem in set(template)]
    return max(element_counts) - min(element_counts)


def dont_really_grow_polymer(template, rules, steps):
    pair_counts = {pair: 0 for pair in rules}
    for i in range(len(template) - 1):
        pair_counts[template[i] + template[i + 1]] += 1
    for _ in range(steps):
        pair_counts = dont_really_polymerize(rules, pair_counts)
    element_counts = {elem: 0 for elem in set("".join(rules))}
    for pair, count in pair_counts.items():
        element_counts[pair[0]] += count
    element_counts[template[-1]] += 1
    return max(element_counts.values()) - min(element_counts.values())


if __name__ == "__main__":
    test_template, test_rules = read_input("test_input.txt")
    assert grow_polymer(test_template, test_rules, 10) == 1588
    template, rules = read_input("input.txt")
    print(grow_polymer(template, rules, 10))
    assert dont_really_grow_polymer(test_template, test_rules, 10) == 1588
    assert grow_polymer(template, rules, 10) == dont_really_grow_polymer(
        template, rules, 10
    )
    assert dont_really_grow_polymer(test_template, test_rules, 40) == 2188189693529
    print(dont_really_grow_polymer(template, rules, 40))
