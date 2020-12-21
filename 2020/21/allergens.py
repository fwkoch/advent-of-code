class Food:
    def __init__(self, ingredients, allergens):
        self.ingredients = ingredients
        self.allergens = allergens


def match_allergens(foods):
    matches = {}
    for food in foods:
        for allergen in food.allergens:
            if allergen not in matches:
                matches[allergen] = set(food.ingredients)
            else:
                matches[allergen] = matches[allergen].intersection(food.ingredients)
    while not all(len(val) == 1 for val in matches.values()):
        for key, val in matches.items():
            if len(val) != 1:
                continue
            for other_key in matches.keys():
                if other_key == key:
                    continue
                matches[other_key] = matches[other_key].difference(matches[key])
    return {key: value.pop() for key, value in matches.items()}


def count_nonallergens(foods, matches):
    count = 0
    matches = {v: k for k, v in matches.items()}
    for food in foods:
        for ingredient in food.ingredients:
            if ingredient not in matches:
                count += 1
    return count


if __name__ == "__main__":
    with open("input.txt", "r") as fid:
        foods = []
        for line in fid.readlines():
            foods.append(
                Food(
                    ingredients=set(line.split(" (contains ")[0].split()),
                    allergens=line.split(" (contains ")[1].split(")")[0].split(", "),
                )
            )
    matches = match_allergens(foods)
    print(count_nonallergens(foods, matches))
