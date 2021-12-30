# coding=utf-8
# 使用说明：
# 按照csv要求的填充方式填入协议内容，运行该程序即可生成协议对应发送函数到proto.txt文件中，该操作会覆盖proto.txt中的原本内容
'''
csv的格式
'''

import csv
import platform
# note: CONFIG PATH
if (platform.machine() == "x86_64"):
    FOLDER = "/home/xiz/junkcar/serial_port/script/"
else:
    FOLDER = "/home/pi/junkcar/serial_port/script/"


def trans_type(typetot):
    dic_type = {}
    dic_type["float"] = 4
    dic_type["char"] = 1
    dic_type["unsigned char"] = 1
    dic_type["uchar_t"] = 1
    dic_type["int"] = 4
    dic_type["unsigned int"] = 4
    dic_type["short"] = 2
    dic_type["long"] = 4
    dic_type["bool"] = 1
    dic_type["double"] = 8
    dic_type["uint8_t"] = 1
    return dic_type[typetot]


if __name__ == "__main__":
    file_name = FOLDER + "proto.csv"
    with open(file_name) as csvf:
        rows = csv.reader(csvf)
        program = ""
        name = ""
        cmd = ""
        for row in rows:
            par_type_list = []
            par_name_list = []
            for i in range(len(row)):
                if (i == 0):
                    name = row[i]
                elif(i == 1):
                    cmd = row[i]
                elif(i % 2 == 0):
                    par_type_list.append(row[i])
                elif(i % 2 == 1):
                    par_name_list.append(row[i])

            program += "bool SerialInterface::"
            program += name + '('

            if(len(par_type_list) != len(par_name_list)):
                print("WRONG!")

            for i in range(len(par_type_list)):
                program += par_type_list[i]
                # print par_type_list[i]
                program += ' ' + par_name_list[i]
                if(i != (len(par_type_list) - 1)):
                    program += ','
            program += "){\n    FixedPacket packet;\n"
            program += "    " + "packet.loadData(" + cmd + ",1);\n"

            used_len = 2
            for i in range(len(par_name_list)):
                program += "    " + \
                    "packet.loadData(" + \
                    par_name_list[i] + "," + str(used_len) + ");\n"
                used_len += trans_type(par_type_list[i])
            program += "    " + "packet.pack();\n    " + "return sendPacket(packet) == 0;\n}\n\n"
        print(program)
        f = open(FOLDER + "proto.txt", 'w')  # 清空文件内容再写
        f.write(program)  # 只能写字符串
