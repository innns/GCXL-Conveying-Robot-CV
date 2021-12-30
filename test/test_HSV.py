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
"""
# >>>>>>
# Descripttion:
# version: 1.0
# Author: innns
# Email: ureinsecure@outlook.com
# Date: 2021-06-16 00:20:13
# LastEditors: innns
# LastEditTime: 2021-06-16 01:43:01
# FilePath: /junkcar/test/test_HSV.py
# <<<<<<
import cv2
# import numpy as np
import platform

if (platform.machine() == "x86_64"):
    CONFIG_PATH = "/home/ins/junkcar/config/cam.yaml"
else:
    CONFIG_PATH = "/home/pi/junkcar/config/cam.yaml"

f = cv2.FileStorage(CONFIG_PATH, cv2.FileStorage_READ)

HSV_R = f.getNode('HSV_R').mat()
HSV_R_ = f.getNode('HSV_R_').mat()
HSV_G = f.getNode('HSV_G').mat()
HSV_B = f.getNode('HSV_B').mat()

HSV_R_min = HSV_R[0]
HSV_R_max = HSV_R[1]

print(HSV_R_min)
print(HSV_R_max)

img = cv2.imread("R.jpg")
