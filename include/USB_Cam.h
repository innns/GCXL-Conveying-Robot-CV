/*
 *  Copyright (C) @innns.
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
 * @Descripttion: 现在参数在640*480下
 * @version: 2.0
 * @Author: innns
 * @Email: ureinsecure@outlook.com
 * @Date: 2021-02-03 14:39:54
 * @LastEditors: innns
 * @LastEditTime: 2021-09-14 00:49:42
 * @FilePath: /junkcar/include/USB_Cam.h
*/

#ifndef USB_Cam_H
#define USB_Cam_H

#include "essential.h"

using namespace std;
using namespace cv;

/**
 * @name: USB_Cam
 * @msg: USB 相机功能类
*/
class USB_Cam
{
private:
    cv::Mat frame = Mat();
    bool isQuiting; // flase
private:
    cv::VideoCapture capture;
    std::string DATE_NOW, PIC_DIR, time_now;
    std::string getDate();
    std::string getTime();
    void mkdir_Date(); //创建文件夹
    // const char *MAIN_WIN = "MAIN"; // windows name
    // bool isQuiting = false;
    char key = 0;
    bool initCam(int num = 5);
    void savePic();
    // Mat frame;
    int pic_count;
    //camera resolution
    int res_w = 640;          // width
    int res_h = 480;          // height
    int white_balance = 3500; // white balance
    int exposure = 50;

public:
    USB_Cam(const char *device_name = "/dev/camera0", int num = 5);
    ~USB_Cam();
    bool switchCam(const char *device_name = "/dev/camera0", int num = 5);
    bool grab();
    Mat read();
    bool read(Mat &frame);
    bool isOpened()
    {
        return capture.isOpened();
    }
};

#endif