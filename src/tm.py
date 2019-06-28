import time
import os

res = 0
cnt = 0
for k in range(3):
    time_start = time.time()
    os.system("./interpreter ../javacode/loc1.class 1000000000")
    time_end = time.time()
    res += time_end - time_start
    cnt += 1

print(res / cnt)
