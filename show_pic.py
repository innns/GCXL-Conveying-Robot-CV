#!/usr/bin/python
# encoding:utf-8
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
Descripttion:
version: 1.0
Author: innns
Email: ureinsecure@outlook.com
Date: 2021-08-11 18:51:04
LastEditors: innns
LastEditTime: 2021-08-11 20:01:16
FilePath: /junkcar/show_pic.py
"""
# <<<<<<
import cv2


cap = cv2.VideoCapture("/dev/camera0")
cap.set(cv2.CAP_PROP_FOURCC,
        cv2.VideoWriter_fourcc('M', 'J', 'P', 'G'))
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 800)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 600)
# cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
# cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
# cap.set(cv2.CAP_PROP_EXPOSURE, 50)
cap.set(cv2.CAP_PROP_BRIGHTNESS, 6)


if __name__ == "__main__":
    for i in range(4):
        cap.grab()
        cv2.waitKey(60)
    while (True):
        ret, img = cap.read()
        if ret:
            try:
                # img_ = cv2.resize(img, (640, 480))
                img_ = img.copy()
                cv2.imshow("1.jpg", img_)
            except Exception:
                print("cannot imshow")
                pass
        else:
            print("EMPTY")
        k = int(cv2.waitKey(60))
        if k == ord('s'):
            cv2.imwrite("1.jpg", img_)
        elif k == 27:
            break
