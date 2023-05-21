import re 
import os

os.chdir(r"D:\我的文档\学习\Atmel Studio\超声波雷达\GccApplication2\etc")
fin_path = r"EepProducer.in"
fout_path = r"EepProducer.eep"

# fout = open(fout_path, "wb+")
# Bytes = [0x13]
# fout.write(bytes(Bytes))
# fout.close()


with open(fin_path, "r+") as fin:
    fin.seek(0, 2)
    fin_eof = fin.tell() # get input file length
    fin.seek(0, 0)

    fout = open(fout_path, "wb+")
    Bytes = [0xff]
    for i in range(0, 400):
        fout.write(bytes(Bytes))  # clear eep file
    fout.seek(0, 0)  # move fp to the beginning

    while True:
        if fin.tell() >= fin_eof:
            break
        line = fin.readline()
        char_name = re.findall(r"// (.*)", line)
        if len(char_name):
            char_name = char_name[0]
        else :
            char_name = re.findall(r"//(.*)", line) [0]
        line = re.findall(r"(.*),//", line) [0]
        chars = line.split(", ")
        print(char_name, ": ", chars)

        Bytes = []
        for char in chars:
            Bytes.append(int(char, 16))
        fout.write(bytes(Bytes))

    fout.close()
