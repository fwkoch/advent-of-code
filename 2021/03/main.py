from pydantic import BaseModel


class DiagnosticReport(BaseModel):

    numbers: list[str] = []
    gamma_bin: str = ""

    def read_input(self, filename):
        self.gamma_bin = ""
        with open(filename) as fid:
            self.numbers = [line.strip() for line in fid.readlines()]
        return self.numbers

    @staticmethod
    def determine_column(numbers, index):
        zeros = 0
        for number in numbers:
            if number[index] == "0":
                zeros += 1
        if zeros > len(numbers) / 2:
            return "0"
        return "1"

    def gamma(self):
        if not self.gamma_bin:
            self.gamma_bin = ""
            for i in range(len(self.numbers[0])):
                self.gamma_bin += self.determine_column(self.numbers, i)
        return int(self.gamma_bin, 2)

    def epsilon(self):
        if not self.gamma_bin:
            self.gamma()
        epsilon_bin = "".join("0" if val == "1" else "1" for val in self.gamma_bin)
        return int(epsilon_bin, 2)

    def oxygen_filter(self, numbers, index):
        val = self.determine_column(numbers, index)
        return [n for n in numbers if n[index] == val]

    def co2_filter(self, numbers, index):
        val = self.determine_column(numbers, index)
        return [n for n in numbers if n[index] != val]

    def oxygen(self):
        numbers = self.numbers
        index = 0
        while len(numbers) > 1:
            numbers = self.oxygen_filter(numbers, index)
            index += 1
        return int(numbers[0], 2)

    def co2(self):
        numbers = self.numbers
        index = 0
        while len(numbers) > 1:
            numbers = self.co2_filter(numbers, index)
            index += 1
        return int(numbers[0], 2)


if __name__ == "__main__":
    dr_test = DiagnosticReport()
    dr_test.read_input("./test_input.txt")
    assert dr_test.gamma() * dr_test.epsilon() == 198
    dr = DiagnosticReport()
    dr.read_input("./input.txt")
    print(dr.gamma() * dr.epsilon())
    assert dr_test.oxygen() * dr_test.co2() == 230
    print(dr.oxygen() * dr.co2())
