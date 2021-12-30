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
 * @version: 2.0
 * @Author: innns
 * @Email: ureinsecure@outlook.com
 * @Date: 2021-02-03 14:39:54
 * @LastEditors: innns
 * @LastEditTime: 2021-08-24 19:43:25
 * @FilePath: /GCXL-Conveying-Robot-CV/src/USB_Cam.cpp
*/

#include "USB_Cam.h"

/**
 * @brief: 构造函数 用于初始化。
 * @param[in] device_inex
 * 声明时已经给device_name赋默认值 "/dev/video0" 了
*/
USB_Cam::USB_Cam(const char *device_name, int num)
{
    mkdir_Date();
    capture.open(device_name);
    if (initCam(num))
    {
        cout << "online" << endl;
    }
    else
    {
        cout << device_name << " DEAD!!!" << endl;
    }
}

USB_Cam::~USB_Cam()
{
    cv::destroyAllWindows();
    capture.release();
}

/**
 * @brief: switch camera by index. Return true if switch camera successfully.
 * @name: switchCam
 * 声明时已经给device_name赋默认值 "/dev/camera0" 了
*/
bool USB_Cam::switchCam(const char *device_name, int num)
{
    cv::destroyAllWindows();
    capture.open(device_name);

    if (initCam(num))
    {
        cout << "online" << endl;
        return true;
    }
    else
    {
        cout << device_name << " DEAD!!!" << endl;
    }
    return false;
}

#pragma region DateAndTime
/**
 * @name: getDate()
 * @msg: 内部调用，返回当前日期文件夹名
 * @return {yyyy-mm-dd/}
*/
std::string USB_Cam::getDate()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm *pTime;
    pTime = localtime(&tv.tv_sec);
    char sTemp[40] = {0};
    snprintf(sTemp, sizeof(sTemp), "%04d-%02d-%02d/", (int)pTime->tm_year + 1900,
             (int)pTime->tm_mon + 1, (int)pTime->tm_mday);
    return (std::string)sTemp;
}

/** 
 * @name: getTime()
 * @msg: 内部调用，返回当前日期文件夹名/当前时间
 * @return {yyyy-mm-dd/hh-mm-ss-ms-us}, ms us为3位
*/
std::string USB_Cam::getTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm *pTime;
    pTime = localtime(&tv.tv_sec);
    char sTemp[40] = {0};
    snprintf(sTemp, sizeof(sTemp), "%04d-%02d-%02d/%02d-%02d-%02d-%03d-%03d", (int)pTime->tm_year + 1900,
             (int)pTime->tm_mon + 1, (int)pTime->tm_mday, (int)pTime->tm_hour, (int)pTime->tm_min, (int)pTime->tm_sec,
             (int)tv.tv_usec / 1000, (int)tv.tv_usec % 1000);
    return (std::string)sTemp;
}

/*
@name: mkdir_Date
@msg: 内部调用，创建新的日期文件夹
*/
void USB_Cam::mkdir_Date()
{
    // 确定文件和文件夹是否存在和访问权限
    //（_AccessMode参数：00表示只判断是否存在，02表示文件是否可执行， 04表示文件是否可写，06表示文件是否可读）
    // 有指定访问权限则返回0，否则函数返回-1
    DATE_NOW = getDate();
    PIC_DIR = SAVE_DIR + DATE_NOW;
    if (access((PIC_DIR.c_str()), 04) == -1)
    {
        mkdir((PIC_DIR.c_str()), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        cout << "mkdir " << (PIC_DIR.c_str()) << "\n";
    }
    else
    {
        cout << "dir [" << PIC_DIR << "] exist\n";
    }
}
#pragma endregion

/** Init camera. returns ture if success.先拍摄几张图片等图像稳定。
 * @brief: 初始化相机
 * @name: initCam(int num)
 * @param {int}num
 * @return {bool}
*/
bool USB_Cam::initCam(int num)
{
    cv::FileStorage f(CONFIG_PATH, cv::FileStorage::READ);
    if (!f.isOpened())
    {
        cout << "FAIL TO CONFIG\n"
             << res_w << "*" << res_h << "\n";
        capture.set(CAP_PROP_FRAME_WIDTH, res_w);
        capture.set(CAP_PROP_FRAME_HEIGHT, res_h);
    }
    else
    {
        f["res_w"] >> res_w;
        f["res_h"] >> res_h;
        // f["white_balance"] >> white_balance;
        f["exposure"] >> exposure;
        cout << res_w << "*" << res_h << " wb " << white_balance << "\n";
        capture.set(CAP_PROP_FRAME_WIDTH, res_w);
        capture.set(CAP_PROP_FRAME_HEIGHT, res_h);
        capture.set(CAP_PROP_EXPOSURE, 50);  // 曝光 50
        capture.set(CAP_PROP_BUFFERSIZE, 3); // 缓冲区3张
        // capture.set(CAP_PROP_WB_TEMPERATURE, white_balance);
        // note: 相机参数设定查阅 https://blog.csdn.net/qq_41498261/article/details/103300531
    }
    f.release();
    num = AMP_LIMIT(num, 5, 1);
    pic_count = 0;
    // 初始化
    bool init_done = false;
    // 拍几张照片等相机稳定
    for (int init_cnt = 0; init_cnt < num; init_cnt++)
    {
        capture >> frame;
        init_done = !frame.empty();
    }
    // cv::namedWindow(MAIN_WIN, WINDOW_AUTOSIZE);
    return init_done;
}

/** 
 * @brief return frame.
 * @name read()
*/
Mat USB_Cam::read()
{
    if (LIKELY(capture.isOpened()))
    {
        capture >> frame;
        return frame;
    }
    std::cout << "ERROR !!! cap closed\n";
    return Mat();
}

/** 
 * @brief return ture if update frame successfully.
 * @name read()
*/
bool USB_Cam::read(Mat &frame)
{
    frame = read();
    return (!frame.empty());
}

bool USB_Cam::grab()
{
    return capture.grab();
}
