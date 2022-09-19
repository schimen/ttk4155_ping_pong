from sram_result import data

import numpy as np
import matplotlib.pyplot as plt

extram_start = 0x1800

result = np.array(data)
n_tests = 2048
fail_percentage = (len(np.unique(result))/n_tests)*100
print(f'{len(result)} fails, percentage of fails: {fail_percentage}')

bit_highs = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
bit_lows  = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

for fail in result:
    for i, bit in enumerate(bin(fail+extram_start)[3:]):
        if bit == '1':
            bit_highs[i] += 1
        else:
            bit_lows[i] += 1

bit_highs.reverse()
bit_lows.reverse()

print(f'N high bits {bit_highs}')
print(f'N low bits {bit_lows}')

plt.bar(list(range(12)), bit_highs)
plt.show()