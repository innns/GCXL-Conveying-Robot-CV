/*
 * Copyright (C) @innns.
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * @version: 1.0
 * @Author: innns
 * @Email: ureinsecure@outlook.com
 * @Date: 2021-02-03 14:39:54
 * @LastEditors: innns
 * @LastEditTime: 2021-09-17 21:36:57
 * @FilePath: /junkcar/test/test_detector.cpp
*/
#include <opencv2/imgproc.hpp>
#include <opencv2/barcode.hpp>
#include "My_Test.hpp"

int main()
{
    int key = 0;
    bool is_quiting = false;
    while (!is_quiting)
    {
        cout << "0 task\n1 uart\n2 allcolor\n3 target\n4 barcode\nEsc quiting\n";
        cin >> key;
        switch (key)
        {
        case 0:
        {
            test_task();
        }
        break;
        case 1:
        {
            test_uart();
        }
        break;
        case 2:
        {
            test_all_color();
        }
        break;
        case 3:
        {
            test_get_center();
        }
        break;
        case 4:
        {
            Mat src_img = imread("1.jpg");
            Ptr<barcode::BarcodeDetector> bardet = makePtr<barcode::BarcodeDetector>(); //如果不使用超分辨率则可以不指定模型路径
            vector<Point2f> corners;                                                    //返回的检测框的四个角点坐标，如果检测到N个条码，那么维度应该是[N][4][2]
            std::vector<std::string> decoded_info;                                      //返回的解码结果，如果解码失败，则为空string
            std::vector<barcode::BarcodeType> decoded_format;                           //返回的条码类型，如果解码失败，则为BarcodeType::NONE
             * bardet->detectAndDecode(src_img, decoded_info, decoded_format, corners);
            if (bardet->detectAndDecode(src_img, decoded_info, decoded_format, corners))
                cout << decoded_info[0];

            if (corners.size() != 0)
            {
                cout << "suck2\n";
                 * note: result 会将cross_center 返回给center
                Point2f cross_center = Point2f((corners.at(0).x + corners.at(2).x) / 2, (corners.at(0).y + corners.at(2).y) / 2);
                cout << cross_center.x << " " << cross_center.y << "\n";
            }
        }
        break;
        case 5:
        {
             * Mat src_img = imread("/home/ins/junkcar/cam_save/center_R.jpg");
             * test_get_center(src_img);
            test_show_mission();
        }
        break;
        case 27:
        {
            is_quiting = true;
            return 0;
        }
        break;
        default:
            break;
        }
    }
    return 0;
}
