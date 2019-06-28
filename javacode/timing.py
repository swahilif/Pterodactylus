import time
import os
res = 0
for i in range(5):
    time_start = time.time()
    os.system("java loc1")
    time_end = time.time()
    res += time_end - time_start
print(res / 5)
