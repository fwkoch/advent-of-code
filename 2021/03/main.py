from pydantic import BaseModel

class DiagnosticReport(BaseModel):

    numbers: list[str] = []
    gamma_bin: str = ''

    def read_input(self, filename):
        self.gamma_bin = ''
        with open(filename) as fid:
            self.numbers = [line.strip() for line in fid.readlines()]
        return self.numbers

    def determine_column(self, index):
        zeros = 0
        for number in self.numbers:
            if number[index] == '0':
                zeros += 1
        if zeros == len(self.numbers) / 2:
            raise ValueError('uhh number of 0s and 1s equal')
        if zeros > len(self.numbers ) / 2:
            return '0'
        return '1'

    def gamma(self):
        if not self.gamma_bin:
            self.gamma_bin = ''
            for i in range(len(self.numbers[0])):
                self.gamma_bin += self.determine_column(i)
        return int(self.gamma_bin, 2)

    def epsilon(self):
        if not self.gamma_bin:
            self.gamma()
        epsilon_bin = ''.join('0' if val == '1' else '1' for val in self.gamma_bin)
        return int(epsilon_bin, 2)

if __name__ == "__main__":
    dr = DiagnosticReport()
    dr.read_input('./test_input.txt')
    assert dr.gamma() * dr.epsilon() == 198
    dr = DiagnosticReport()
    dr.read_input('./input.txt')
    print(dr.gamma() * dr.epsilon())
