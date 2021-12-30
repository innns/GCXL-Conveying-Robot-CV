import cv2 as cv
import numpy as np

image = cv.imread("/home/pi/junkcar_ws/cam_save/red.jpg")
cv.imshow("input", image)
gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)
ret, binary = cv.threshold(gray, 0, 255, cv.THRESH_OTSU | cv.THRESH_BINARY_INV)
se1 = cv.getStructuringElement(cv.MORPH_CROSS, (50, 1))
se2 = cv.getStructuringElement(cv.MORPH_CROSS, (1, 50))
hline = cv.morphologyEx(binary, cv.MORPH_OPEN, se1)
vline = cv.morphologyEx(binary, cv.MORPH_OPEN, se2)
contours, hireachy = cv.findContours(hline, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
mask = np.zeros_like(hline)
max = -1
index = 0
for cnt in range(len(contours)):
    x, y, w, h = cv.boundingRect(contours[cnt])
    if max < w:
        max = w
        index = cnt
cv.drawContours(mask, contours, index, (255), -1, 8)

cv.imshow("vline", vline)
contours, hireachy = cv.findContours(vline, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
max = -1
index = 0
for cnt in range(len(contours)):
    x, y, w, h = cv.boundingRect(contours[cnt])
    if max < h and x < int(gray.shape[1]*0.75):
        max = h
        index = cnt

cv.drawContours(mask, contours, index, (255), -1, 8)
cv.imshow("mask", mask)

se3 = cv.getStructuringElement(cv.MORPH_CROSS, (13, 13))
mask = cv.morphologyEx(mask, cv.MORPH_OPEN, se3)
cv.imshow("corss", mask)
contours, hireachy = cv.findContours(mask, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
for cnt in range(len(contours)):
    x, y, w, h = cv.boundingRect(contours[cnt])
    print(x, y, w, h)
    cx = (x + w//2)
    cy = (y + h//2)
    cv.circle(image, (cx, cy), 4, (0, 0, 255), 4, 8, 0)
cv.imshow("result", image)
cv.imwrite("D:/find_cross.png", image)
cv.waitKey(0)
cv.destroyAllWindows()