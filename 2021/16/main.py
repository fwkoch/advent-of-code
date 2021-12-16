def hex_to_bin(message):
    packet = bin(int(message, 16))[2:]
    if len(packet) % 4:
        pad = "0" * (4 - len(packet) % 4)
    else:
        pad = ""
    return pad + packet


def get_packets(packet):
    type_id = int(packet[3:6], 2)
    if type_id == 4:  # literal value
        ind = 6
        output_bin = packet[ind + 1 : ind + 5]
        while packet[ind] == "1":
            ind += 5
            output_bin += packet[ind + 1 : ind + 5]
        value = int(output_bin, 2)
        return [packet[: ind + 5]], ind + 5
    packets = []
    if packet[6] == "0":  # length type id
        total_length = int(packet[7 : 7 + 15], 2)
        start = 7 + 15
        while start - (7 + 15) < total_length:
            sub_packets, end = get_packets(packet[start:])
            packets += sub_packets
            start = start + end
    else:
        num_sub_packets = int(packet[7 : 7 + 11], 2)
        start = 7 + 11
        for _ in range(num_sub_packets):
            sub_packets, end = get_packets(packet[start:])
            packets += sub_packets
            start = start + end
    packets = [packet[:start]] + packets
    return packets, start


def get_version(packet):
    return int(packet[:3], 2)


def add_versions(message):
    packet = hex_to_bin(message)
    packets = get_packets(packet)[0]
    return sum([get_version(p) for p in packets])


def read_input(filename):
    with open(filename, "r") as fid:
        return fid.readline().strip()


if __name__ == "__main__":
    message = "D2FE28"
    assert hex_to_bin(message) == "110100101111111000101000"
    assert get_packets(hex_to_bin(message)) == (["110100101111111000101"], 21)
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
