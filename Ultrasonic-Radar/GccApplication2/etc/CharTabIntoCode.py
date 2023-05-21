import re 
import os

os.chdir(r"D:\我的文档\学习\Atmel Studio\OLED\GccApplication2\tools and old versions")
fin_path = r"CharTabIntoCode.in"
fout_path = r"CharTabIntoCode.out"

with open(fin_path, "r+") as fin:
    fin.seek(0, 2)
    fin_eof = fin.tell() # 获取eof
    fin.seek(0, 0)

    fout = open(fout_path, "w+")
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


        fout.write("case \'%s\':" % (char_name))
        if len(char_name) == 1:
            fout.write(" { ")
        if len(char_name) != 1:
            fout.write("{ ")
        #fout.write("unsigned char chs[] = { %s }; return chs[col];" % (line))
        # # 5934 bytes   72.4 % Full  #  520 bytes   50.8 % Full
        # for ichdat in range(0, len(chars)):
        #     chdat = chars[ichdat]
        #     fout.write("if (col == %d) " % (ichdat))
        #     fout.write("return %s; " % (chdat))
        #fout.write(" }\n")

        # 
        fout.write("switch (col) { ")
        for ichdat in range(0, len(chars)):
            chdat = chars[ichdat]
            fout.write("case %d: " % (ichdat))
            fout.write("return %s; " % (chdat))
        fout.write(" } }\n")
            
    fout.close()
