/*
 *  Copyright (C) @innns 2021.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 * @Descripttion: 
 * @version: 1.0
 * @Author: innns
 * @Email: ureinsecure@outlook.com
 * @Date: 2021-05-08 19:12:41
 * @LastEditors: innns
 * @LastEditTime: 2021-05-08 23:11:40
 * @FilePath: /GCXL-Conveying-Robot-CV/test/test_config.cpp
*/

#include "My_Test.hpp"

HSV_T HSV_R = {Scalar(156, 80, 15), Scalar(180, 255, 255)};
HSV_T HSV_R_ = {Scalar(0, 80, 15), Scalar(10, 255, 255)}; // 红色需要两段
HSV_T HSV_G = {Scalar(45, 180, 15), Scalar(85, 255, 255)};
HSV_T HSV_B = {Scalar(100, 80, 15), Scalar(124, 255, 255)};

int main()
{
    // cv::FileStorage f("/home/ins/Documents/junkcar/config/test.yaml", cv::FileStorage::WRITE);
    // // test_all_color();
    // f << "res_w" << 640;
    // f << "res_h" << 640;
    // f << "white_balance" << 4800;
    // Mat r = (cv::Mat_<double>(3, 2) << R_HSV.min[0], R_HSV.min[1], R_HSV.min[2], R_HSV.max[0], R_HSV.max[1], R_HSV.max[2]);
    // f << "HSV_R" << r;
    // Mat g = (cv::Mat_<double>(3, 2) << G_HSV.min[0], G_HSV.min[1], G_HSV.min[2], G_HSV.max[0], G_HSV.max[1], G_HSV.max[2]);
    // f << "HSV_G" << g;
    // Mat b = (cv::Mat_<double>(3, 2) << B_HSV.min[0], B_HSV.min[1], B_HSV.min[2], B_HSV.max[0], B_HSV.max[1], B_HSV.max[2]);
    // f << "HSV_B" << b;

    cv::FileStorage f("/home/ins/Documents/junkcar/config/cam.yaml", cv::FileStorage::READ);
    if (f.isOpened())
    {
        Mat t;
        f["HSV_R"] >> t;
        // R_HSV = {(t.at<uchar>(1, 1), t.at<uchar>(1, 2), t.at<uchar>(1, 3)), (t.at<uchar>(1, 4), t.at<uchar>(1, 5), t.at<uchar>(1, 6))};
        cout << t.rows << "  " << t.cols << "\n";
        cout << t.at<double>(0, 0) << "  " << t.at<double>(0, 1) << "  " << t.at<double>(0, 2) << "\n";
        cout << t.at<double>(1, 0) << "  " << t.at<double>(1, 1) << "  " << t.at<double>(1, 2) << "\n";
        HSV_R.min = Scalar(t.at<double>(0, 0), t.at<double>(0, 1), t.at<double>(0, 2));
        HSV_R.max = Scalar(t.at<double>(1, 0), t.at<double>(1, 1), t.at<double>(1, 2));

        cout << HSV_R.min << HSV_R.max;
        // f["res_h"] >> res_h;
    }

    // return 0;
}