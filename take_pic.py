#!/usr/bin/env python
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
"""

import cv2

import time

PIC_NAME = "ALL.jpg"
cap = cv2.VideoCapture("/dev/camera0")
cap.set(cv2.CAP_PROP_FOURCC,
        cv2.VideoWriter_fourcc('M', 'J', 'P', 'G'))
# cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1600)
# cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1200)
# <<<<<<< HEAD
cap.set(cv2.CAP_PROP_EXPOSURE, 80)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 800)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 600)
cap.set(cv2.CAP_PROP_BRIGHTNESS, 15)


if __name__ == "__main__":
    for i in range(6):
        cv2.waitKey(60)
    start = time.time()   # 时间戳
    ret, img = cap.read()
    end = time.time()   # 时间戳
    if ret:
        print("开始时间：" + str(start))
        print("结束时间：" + str(end))
        print("拍摄用时：" + str(end - start))
        cv2.imwrite(PIC_NAME, img)
        try:
            cv2.imshow("1.jpg", img)
            cv2.waitKey(0)
        except Exception:
            pass
    else:
        print("EMPTY")
