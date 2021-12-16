def hex_to_bin(message):
    packet = bin(int(message, 16))[2:]
    pad = len(message) * 4 - len(packet)
    return "0" * pad + packet


def get_version_sum(packet):
    version = int(packet[:3], 2)
    type_id = int(packet[3:6], 2)
    if type_id == 4:  # literal value
        ind = 6
        while packet[ind] == "1":
            ind += 5
        return version, ind + 5
    if packet[6] == "0":  # length type id
        total_length = int(packet[7 : 7 + 15], 2)
        start = 7 + 15
        while start - (7 + 15) < total_length:
            sub_version, end = get_version_sum(packet[start:])
            version += sub_version
            start = start + end
    else:
        num_sub_packets = int(packet[7 : 7 + 11], 2)
        start = 7 + 11
        for _ in range(num_sub_packets):
            sub_version, end = get_version_sum(packet[start:])
            version += sub_version
            start = start + end
    return version, start


def add_versions(message):
    return get_version_sum(hex_to_bin(message))[0]


def evaluate(packet):
    type_id = int(packet[3:6], 2)
    if type_id == 4:  # literal value
        ind = 6
        output_bin = packet[ind + 1 : ind + 5]
        while packet[ind] == "1":
            ind += 5
            output_bin += packet[ind + 1 : ind + 5]
        result = int(output_bin, 2)
        return result, ind + 5
    values = []
    if packet[6] == "0":  # length type id
        total_length = int(packet[7 : 7 + 15], 2)
        start = 7 + 15
        while start - (7 + 15) < total_length:
            value, end = evaluate(packet[start:])
            values.append(value)
            start = start + end
    else:
        num_sub_packets = int(packet[7 : 7 + 11], 2)
        start = 7 + 11
        for _ in range(num_sub_packets):
            value, end = evaluate(packet[start:])
            values.append(value)
            start = start + end
    if type_id == 0:
        result = sum(values)
    elif type_id == 1:
        result = 1
        for value in values:
            result *= value
    elif type_id == 2:
        result = min(values)
    elif type_id == 3:
        result = max(values)
    elif type_id == 5:
        assert len(values) == 2
        result = int(values[0] > values[1])
    elif type_id == 6:
        assert len(values) == 2
        result = int(values[0] < values[1])
    elif type_id == 7:
        assert len(values) == 2
        result = int(values[0] == values[1])

    return result, start


def read_input(filename):
    with open(filename, "r") as fid:
        return fid.readline().strip()


if __name__ == "__main__":
    message = "D2FE28"
    assert hex_to_bin(message) == "110100101111111000101000"
    assert add_versions(message) == 6
    message = "38006F45291200"
    assert (
        hex_to_bin(message)
        == "00111000000000000110111101000101001010010001001000000000"
    )
    assert add_versions("8A004A801A8002F478") == 16
    assert add_versions("620080001611562C8802118E34") == 12
    assert add_versions("C0015000016115A2E0802F182340") == 23
    assert add_versions("A0016C880162017C3686B18A3D4780") == 31
    print(add_versions(read_input("input.txt")))

    assert evaluate(hex_to_bin("C200B40A82"))[0] == 3
    assert evaluate(hex_to_bin("04005AC33890"))[0] == 54
    assert evaluate(hex_to_bin("880086C3E88112"))[0] == 7
    assert evaluate(hex_to_bin("CE00C43D881120"))[0] == 9
    assert evaluate(hex_to_bin("D8005AC2A8F0"))[0] == 1
    assert evaluate(hex_to_bin("F600BC2D8F"))[0] == 0
    assert evaluate(hex_to_bin("9C005AC2F8F0"))[0] == 0
    assert evaluate(hex_to_bin("9C0141080250320F1802104A08"))[0] == 1

    print(evaluate(hex_to_bin(read_input("input.txt")))[0])
