def transform(subject_number, loop_size):
    value = 1
    for i in range(loop_size):
        value = (value * subject_number) % 20201227
    return value


def reverse_transform(subject_number, target):
    value = 1
    loop_count = 0
    while value != target:
        loop_count += 1
        value = (value * subject_number) % 20201227
    return loop_count


def get_encryption_key(card_public_key, door_public_key):
    card_loop_size = reverse_transform(7, card_public_key)
    door_loop_size = reverse_transform(7, door_public_key)

    card_encryption_key = transform(door_public_key, card_loop_size)
    door_encryption_key = transform(card_public_key, door_loop_size)
    assert card_encryption_key == door_encryption_key
    return card_encryption_key


if __name__ == "__main__":
    print(get_encryption_key(1614360, 7734663))
