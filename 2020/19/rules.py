import itertools


class Rule:
    def __init__(self, rule_string=None, value=None):
        if value:
            self.value = value
        elif rule_string[0] == '"':
            self.value = rule_string[1]
        else:
            self.value = [val.split() for val in rule_string.split(" | ")]

    def resolve(self, rules):
        if isinstance(self.value, str):
            return
        self.value = [[rules[val] for val in entry] for entry in self.value]

    def possible_lengths(self):
        if not hasattr(self, "_possible_lengths"):
            if isinstance(self.value, str):
                return {1}
            output = set()
            for rules in self.value:
                output = output.union(
                    set(
                        sum(prod)
                        for prod in itertools.product(
                            *[rule.possible_lengths() for rule in rules]
                        )
                    )
                )
            self._possible_lengths = output
        return self._possible_lengths

    def validate(self, message):
        if len(message) not in self.possible_lengths():
            return False
        if isinstance(self.value, str):
            return self.value == message
        valid = False
        for rules in self.value:
            for length in rules[0].possible_lengths():
                valid = rules[0].validate(message[:length])
                if valid and len(rules) > 1:
                    valid = valid and Rule(value=[rules[1:]]).validate(message[length:])
                elif len(message[length:]) > 0:
                    valid = False
                if valid:
                    return True
        return valid


with open("input.txt", "r") as fid:
    rules, messages = fid.read().split("\n\n")
    rules = {val.split(": ")[0]: Rule(val.split(": ")[1]) for val in rules.split("\n")}
    for rule in rules.values():
        rule.resolve(rules)
    ct = 0
    for message in messages.split("\n"):
        if rules["0"].validate(message):
            ct += 1
    print(ct)
