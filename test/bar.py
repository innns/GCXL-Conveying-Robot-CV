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
# Date: 2021-05-15 11:04:23
# LastEditors: innns
# LastEditTime: 2021-06-13 16:59:56
# FilePath: /junkcar/test/bar.py
# <<<<<<
import cv2

bardet = cv2.barcode_BarcodeDetector()
img = cv2.imread("1.jpg")
ok, decoded_info, decoded_type, corners = bardet.detectAndDecode(img)
print(decoded_info)
