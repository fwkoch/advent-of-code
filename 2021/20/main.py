def read_input(filename):
    with open(filename, "r") as fid:
        algorithm = fid.readline().strip()
        fid.readline()
        image = []
        for line in fid.readlines():
            image.append(line.strip())

    return algorithm, image


def enhance(algorithm, image, n):
    for iteration in range(n):
        outside_fill = "." if algorithm[0] == "." or iteration % 2 == 0 else "#"
        new_image = []
        for y in range(-1, len(image) + 1):
            row = ""
            for x in range(-1, len(image[0]) + 1):
                index = ""
                for j in [y - 1, y, y + 1]:
                    for i in [x - 1, x, x + 1]:
                        if i < 0 or i >= len(image[0]) or j < 0 or j >= len(image):
                            index += outside_fill
                        else:
                            index += image[j][i]
                index = index.replace(".", "0").replace("#", "1")
                row += algorithm[int(index, 2)]
            new_image.append(row)
        image = new_image
    return image


def count_lit(image):
    return "".join(image).count("#")


if __name__ == "__main__":
    test_alg, test_img = read_input("test_input.txt")
    assert count_lit(enhance(test_alg, test_img, 2)) == 35
    alg, img = read_input("input.txt")
    print(count_lit(enhance(alg, img, 2)))
