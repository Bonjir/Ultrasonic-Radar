import re
import os
import math

os.chdir(r"D:\我的文档\学习\Atmel Studio\超声波雷达\GccApplication2\etc")

fout_path = r"QuickMath.out"

fout = open(fout_path, "w+")

prv_sin_val = -1
def sin_deg(deg):
    return math.sin(float(deg) * (math.pi) / 180)

digit_reserved = 2

for i in range(0, 91):
    sin_val = round(sin_deg(i), digit_reserved)
    fout.write("%.*f, " % (digit_reserved, sin_val))

fout.close()