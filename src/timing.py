import time
import os

res = 0
cnt = 0
for k in range(1):
    time_start = time.time()
    os.system("./interpreter ../javacode/loc1.class 1000000000")
    time_end = time.time()
    t2 = time_end - time_start
    time_start = time.time()
    os.system("./interpreter ../javacode/locality.class 1000000000")
    time_end = time.time()
    t1 = time_end - time_start
    res += t1 - t2
    cnt += 1

print(res / cnt)
