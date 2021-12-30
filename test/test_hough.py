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
# >>>>>>
# Descripttion:
# version: 1.0
# Author: innns
# Email: ureinsecure@outlook.com
# Date: 2021-07-02 11:04:28
# LastEditors: innns
# LastEditTime: 2021-08-04 16:46:35
# FilePath: /junkcar/test/test_hough.py
# <<<<<<
import cv2
import numpy as np

# 创建窗口
cv2.namedWindow('image_win')

# IS_R = 0

# if (IS_R):
#     src = cv2.imread("R.jpg")
#     src = cv2.resize(src, (640, 480))
#     pts1 = np.float32([[323, 183],
#                        [259, 337],
#                        [488, 243],
#                        [440, 409]])
#     pts2 = np.float32([[240, 120], [120, 240], [360, 240], [240, 360]])
# else:
#     src = cv2.imread("L.jpg")
#     src = cv2.resize(src, (640, 480))
#     pts1 = np.float32([[310, 178],
#                        [140, 234],
#                        [371, 337],
#                        [189, 402]])
#     pts2 = np.float32([[240, 120], [120, 240], [360, 240], [240, 360]])

# # 生成变换矩阵
# M = cv2.getPerspectiveTransform(pts1, pts2)
# M_reverse = cv2.getPerspectiveTransform(pts2, pts1)

# # 进行透视变换
# src = cv2.warpPerspective(src, M, (480, 480))


# def cvt_pos(pos, cvt_mat_t):
#     # 获取坐标
#     u = pos[0]
#     v = pos[1]
#     x = (cvt_mat_t[0][0]*u+cvt_mat_t[0][1]*v+cvt_mat_t[0][2]) / \
#         (cvt_mat_t[2][0]*u+cvt_mat_t[2][1]*v+cvt_mat_t[2][2])
#     y = (cvt_mat_t[1][0]*u+cvt_mat_t[1][1]*v+cvt_mat_t[1][2]) / \
#         (cvt_mat_t[2][0]*u+cvt_mat_t[2][1]*v+cvt_mat_t[2][2])
#     return (int(x), int(y))


v = {}
v["k"] = 2
v["i_e"] = 1
v["i_d"] = 1
v["minDist"] = 2
v["param1"] = 100
v["param2"] = 180


src = cv2.imread("/home/pi/junkcar/build/temp0.jpg")


def update(x):
    # 回调函数 更新v的值
    global v

    k = cv2.getTrackbarPos('k', 'image_win')
    i_e = cv2.getTrackbarPos('i_e', 'image_win')
    i_d = cv2.getTrackbarPos('i_d', 'image_win')
    minDist = cv2.getTrackbarPos('minDist', 'image_win')
    param1 = cv2.getTrackbarPos('param1', 'image_win')
    param2 = cv2.getTrackbarPos('param2', 'image_win')

    v["k"] = k
    v["i_e"] = i_e
    v["i_d"] = i_d
    v["minDist"] = minDist
    v["param1"] = param1
    v["param2"] = param2

    kernel = cv2.getStructuringElement(
        cv2.MORPH_ELLIPSE, (int(2*k - 1), int(2*k - 1)))

    erode_img = cv2.morphologyEx(src, cv2.MORPH_ERODE, kernel, iterations=i_e)
    dilate_img = cv2.morphologyEx(
        erode_img, cv2.MORPH_DILATE, kernel, None, iterations=i_d)

    cv2.imshow("erode_img", erode_img)
    cv2.imshow("dilate_img", dilate_img)
    # lap = cv2.Laplacian(dilate_img, cv2.CV_8U, ksize=int(2*k - 1))
    # cv2.imshow("lap", lap)
    out = cv2.cvtColor(dilate_img, cv2.COLOR_BGR2GRAY)
    circles = cv2.HoughCircles(out, cv2.HOUGH_GRADIENT, 1,
                               minDist, param1=param1, param2=param2)
    if circles is not None:
        # Get the (x, y, r) as integers
        circles = np.round(circles[0, :]).astype("int")
        # print(circles)
        # loop over the circles
        src_ = src.copy()
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
        cv2.imshow("lapMask", src_)
    # cv2.imshow("re", dst_)
    print(v)


cv2.createTrackbar('k', 'image_win', 1, 10, update)
cv2.createTrackbar('i_e', 'image_win', 1, 10, update)
cv2.createTrackbar('i_d', 'image_win', 1, 10, update)
cv2.createTrackbar('minDist', 'image_win', 1, 50, update)
cv2.createTrackbar('param1', 'image_win', 40, 200, update)
cv2.createTrackbar('param2', 'image_win', 100, 400, update)

cv2.setTrackbarPos('k', 'image_win', 2)
cv2.setTrackbarPos('i_e', 'image_win', 1)
cv2.setTrackbarPos('i_d', 'image_win', 1)
cv2.setTrackbarPos('minDist', 'image_win', 2)
cv2.setTrackbarPos('param1', 'image_win', 100)
cv2.setTrackbarPos('param2', 'image_win', 180)

while True:
    if 'q' == cv2.waitKey(30):
        break
# 等待按键按下
cv2.waitKey(0)
# 销毁窗口
cv2.destroyAllWindows()
