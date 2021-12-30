#!/usr/bin/env python
"""
####################################################################
Copyright (C) @innns.
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
"""
# Descripttion:
# version: 1.0
# Author: innns
# Email: ureinsecure@outlook.com
# Date: 2021-07-02 11:07:17
# LastEditors: innns
# LastEditTime: 2021-09-09 14:12:38
# FilePath: /junkcar/test/test_find_target.py
# <<<<<<
import cv2
import numpy as np

# 连接摄影机
cam = cv2.VideoCapture("/dev/camera1")


RESOLUTION = (800, 600)           # 相机的分辨率
RECT_WIDTH = int(RESOLUTION[1])          # 校正后图像边长（正方形）
HALF_RW = int(RECT_WIDTH // 2)           # 边长的一半
RADIUS = int(RECT_WIDTH // 3)            # 校正后圆形的半径
CENTER = [HALF_RW, HALF_RW]


LEFT_POS = 1
RIGHT_POS = 3


def getPT(img, pos=2):
    global CENTER
    img = cv2.resize(img, RESOLUTION)
    global M, M_recerse
    if pos == LEFT_POS:
        pts1 = np.float32([[454, 220],
                           [206, 286],
                           [543, 462],
                           [262, 546]])
        CENTER = [300, 300]
    elif pos == RIGHT_POS:
        pts1 = np.float32([[272, 223],
                           [196, 472],
                           [524, 272],
                           [481, 532]])  # down
        CENTER = [300, 300]
    else:  # pos == CENTER_POS, DEAFULT poss
        pts1 = np.float32([[358, 199],
                           [168, 368], 
                           [555, 360], 
                           [363, 566]])  # down
        CENTER = [300, 300]

    pts2 = np.float32([[HALF_RW, HALF_RW - RADIUS], [HALF_RW - RADIUS, HALF_RW],
                       [HALF_RW + RADIUS, HALF_RW], [HALF_RW, HALF_RW + RADIUS]])
    # 生成变换矩阵
    M = cv2.getPerspectiveTransform(pts1, pts2)
    # M_reverse = cv2.getPerspectiveTransform(pts2, pts1)
    # 进行透视变换
    img = cv2.warpPerspective(img, M, (RECT_WIDTH, RECT_WIDTH))

    return img


v = {}
k = 2
i_e = 1
i_d = 1
minDist = 1.5
param1 = 100
param2 = 130
kernel = cv2.getStructuringElement(
    cv2.MORPH_ELLIPSE, (int(2*k - 1), int(2*k - 1)))

blank_img = np.zeros((RECT_WIDTH, RECT_WIDTH, 3), np.uint8)

cv2.namedWindow("CAM")


def getCenter(img):
    global blank_img
    erode_img = cv2.morphologyEx(img, cv2.MORPH_ERODE, kernel, iterations=i_e)
    dilate_img = cv2.morphologyEx(
        erode_img, cv2.MORPH_DILATE, kernel, None, iterations=i_d)
    out = cv2.cvtColor(dilate_img, cv2.COLOR_BGR2GRAY)
    circles = cv2.HoughCircles(out, cv2.HOUGH_GRADIENT, 1,
                               minDist, param1=param1, param2=param2, minRadius=int(RADIUS//2.2))
    if circles is not None:
        # Get the (x, y, r) as integers
        circles = np.round(circles[0, :]).astype("int")
        # print(circles)
        # loop over the circles
        src_ = img.copy()
        xx = 0
        yy = 0
        for (x, y, r) in circles:
            cv2.circle(src_, (x, y), r, (0, 255, 0), 1)
            cv2.circle(src_, (x, y), 2, (0, 0, 255), 2)
            xx += x
            yy += y
        xx //= len(circles)
        yy //= len(circles)
        cv2.circle(src_, (xx, yy), 3, (255, 255, 0), 2)
        # re = cvt_pos((xx, yy), M_reverse)
        # dst_ = dst.copy()
        # cv2.circle(dst_, re, 3, (255, 255, 0), 2)

        cv2.circle(blank_img, (xx, yy), 3, (255, 255, 0), 2)
        cv2.putText(blank_img, str(str(xx)+", "+str(yy)),
                    (xx - 5, yy - 5), cv2.FONT_HERSHEY_TRIPLEX, 0.8, (255, 255, 0))
        cv2.imshow("CAM", src_)
        cv2.imshow("Re", blank_img)
        pos = "RADIUS"
        print("{}: x {}   y {}".format(pos, xx, yy))
        return xx, yy
    else:
        cv2.imshow("CAM", img)
        return -1, -1


if __name__ == '__main__':
    # 使用无穷循环撷取影像，直到按下Esc键结束
    message = """L1 C2 R3"""

    pos = 2
    while True:
        for i in range(5):
            cam.grab()
        # 使用 getFrame 取得最新的影像
        ret, img = cam.read()
        if ret:
            # cv2.imshow('Image', img_l)
            x, y = getCenter(getPT(img, pos))
            movement = [0, 0]
            if(x != -1 and y != -1):
                movement[0] = int(round((CENTER[0] - x) / 4.5))
                movement[1] = int(round((CENTER[1] - y) / 4.5))
            print("movement X: {} y:{}".format(movement[0], movement[1]))
        else:
            print("wrong frame")
            cv2.waitKey(100)
            continue
        key = int(cv2.waitKey(0))
        if key == 27:
            cv2.destroyAllWindows()
            break
        # key == 'c'
        elif key == 99:
            # print(key)
            blank_img = np.zeros((RECT_WIDTH, RECT_WIDTH, 3), np.uint8)
        # key == 'a'
        elif key == 97:
            cv2.destroyAllWindows()
        elif key == 98:
            if ret:
                CENTER = [x, y]
                print("CENTER: ", CENTER)
        elif key == ord('1'):
            pos = 1
        elif key == ord('2'):
            pos = 2
        elif key == ord('3'):
            pos = 3
