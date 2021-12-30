#!/usr/bin/env python
# encoding:utf-8
import cv2
import platform


# PIC PATH
PIC_PATH = "/home/pi/junkcar/test/"
if (platform.machine() == "x86_64"):
    PIC_PATH = "/home/xiz/junkcar/test"

PIC_NAME = PIC_PATH + "0916_R.jpg"

img = cv2.imread(PIC_NAME)


def on_EVENT_LBUTTONDOWN(event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONDOWN:
        xy = "%d,%d" % (x, y)
        cv2.circle(img, (x, y), 1, (255, 0, 0), thickness=-1)
        cv2.putText(img, xy, (x, y), cv2.FONT_HERSHEY_PLAIN,
                    1.0, (0, 0, 0), thickness=1)
        print("[%d, %d]," % (x, y))
        cv2.imshow("image", img)


cv2.namedWindow("image")
cv2.setMouseCallback("image", on_EVENT_LBUTTONDOWN)
while(1):
    cv2.imshow("image", img)
    if cv2.waitKey(30) & 0xFF == 27:
        break
cv2.destroyAllWindows()
