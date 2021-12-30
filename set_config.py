#!/usr/bin/python
# encoding:utf-8
# >>>>>>
"""
####################################################################
Copyright (C) @innns 2021.
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
####################################################################
Descripttion: 设置相机、网络、判断物料等参数
version: 1.0
Author: innns
Email: ureinsecure@outlook.com
Date: 2021-05-08 19:49:54
LastEditors: innns
"""
import cv2
import numpy as np
import platform
# note: CONFIG PATH
if (platform.machine() == "x86_64"):
    CONFIG_PATH = "/home/xiz/junkcar/config/cam.yaml"
else:
    CONFIG_PATH = "/home/pi/junkcar/config/cam.yaml"

f = cv2.FileStorage(CONFIG_PATH, cv2.FileStorage_WRITE)

v = {}

# UDP
v["IP"] = "192.168.3.191"
v["PORT"] = 4210


v["res_w"] = 800
v["res_h"] = 600

v["white_balance"] = 5600  # 白平衡
v["exposure"] = 50

# Hue Saturation Value 色调 饱和度 明度
# 360°下，红色为0°，绿色为120°,蓝色为240°
# H 取值范围0-180
# S 取值范围0-255
# V 取值范围0-255
v["HSV_R"] = np.asarray([[156., 80., 20.],
                         [180., 255., 255.]])

v["HSV_R_"] = np.asarray([[0., 80., 20.],
                          [15., 255., 255.]])

v["HSV_G"] = np.asarray([[55., 120., 40.],
                         [90., 255., 255.]])

v["HSV_B"] = np.asarray([[100., 100., 20.],
                         [124., 255., 255.]])

v["ratio_x"] = 1.4
v["ratio_y"] = 1.2

# dp minDist param1 param2 minRadius maxRadius;      param2减小 检测的圆变多
v["HC"] = np.asarray([1., 1.5, 100., 160., 40, 250])

RESOLUTION = (800, 600)           # 相机的分辨率
RECT_WIDTH = int(RESOLUTION[1])          # 校正后图像边长（正方形）
HALF_RW = int(RECT_WIDTH // 2)           # 边长的一半
RADIUS = int(RECT_WIDTH // 3)            # 校正后圆形的半径
CENTER = [HALF_RW, HALF_RW]

v["SRC_PTS_L"] = np.asarray([[454, 220],
                             [206, 286],
                             [543, 462],
                             [262, 546]],
                            dtype=float)

v["SRC_PTS_R"] = np.asarray([[272, 223],
                             [196, 472],
                             [524, 272],
                             [481, 532]],
                            dtype=float)

v["SRC_PTS_C"] = np.asarray([[358, 199],
                             [168, 368],
                             [555, 360],
                             [363, 566]],
                            dtype=float)

v["DST_PTS"] = np.asarray([
    [HALF_RW, HALF_RW - RADIUS], [HALF_RW - RADIUS, HALF_RW],
    [HALF_RW + RADIUS, HALF_RW], [HALF_RW, HALF_RW + RADIUS]],
    dtype=float)


v["CEN_L"] = np.asarray([300, 300], dtype=float)

v["CEN_C"] = np.asarray([300, 300], dtype=float)

v["CEN_R"] = np.asarray([300, 300], dtype=float)


# TODO: TO TEST
v["STORE_L"] = np.asarray([[454, 220],
                           [206, 286],
                           [543, 462],
                           [262, 546]],
                          dtype=float)

v["STORE_R"] = np.asarray([[272, 223],
                           [196, 472],
                           [524, 272],
                           [481, 532]],
                          dtype=float)

v["STORE_C"] = np.asarray([[358, 199],
                           [168, 368],
                           [555, 360],
                           [363, 566]],
                          dtype=float)

head = ""
src = ""
for key, value in v.items():
    f.write(key, value)
    head += ("{} {} = {};\n".format(str(type(value)).
                                    replace("<class '", "").replace("'>", "").
                                    replace("float", "double").
                                    replace("numpy.ndarray", "cv::Mat"),
                                    key,
                                    value))
    src += ('f["{0}"] >> {0};\n'.format(key))


print("// >>>>>> HEAD")
print(head.replace("[[", "{Point2f(").
      replace(". ", "., ").
      replace("[", "Point2f(").
      replace("]]", ")}").
      replace("]", "), "), end="")
print("// <<<<<< HEAD")
print("")

print("// >>>>>> SRC")
print(src, end="")
print("// <<<<<< SRC")
print("")

print("set config done, config path is {}".format(CONFIG_PATH))
