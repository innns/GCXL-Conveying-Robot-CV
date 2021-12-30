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
 * @Descripttion: 
 * @version: 1.0
 * @Author: innns
 * @Email: ureinsecure@outlook.com
 * @Date: 2021-07-02 17:48:28
 * @LastEditors: innns
 * @LastEditTime: 2021-08-11 00:07:25
 * @FilePath: /junkcar/include/Cam_MT.h
*/
#ifndef CAM_MT_H
#define CAM_MT_H

#include "essential.h"

using namespace std;
using namespace cv;


// template <class T>
class Cam_MT
{
public:
    Cam_MT(const char *index, bool use_config = true, int width = 640, int height = 480);
    ~Cam_MT();

    bool isOpened()
    {
        return _Is_Opened;
    }
    void release()
    {
        _Is_Opened = false;
    }
    bool read(cv::Mat &frame);
    Mat read();

    bool switchCam(const char *index, bool use_config = true);

private:
    void initCam(const char *index, bool use_config = true);
    void updateFrame();
    cv::VideoCapture *_Capture;
    cv::Mat *_Frame;
    std::mutex *_Mutex;
    std::thread *_Thread;
    std::atomic_bool _Is_Opened;
    string _index = "/dev/camera0";
    //camera resolution
private:
    int res_w = 640; // width
    int res_h = 480; // height
    int exposure = 50;

private:
    std::string DATE_NOW, PIC_DIR, time_now;
#pragma region DateAndTime
    /**
     * @name: getDate()
     * @msg: 内部调用，返回当前日期文件夹名
     * @return {yyyy-mm-dd/}
    */
    std::string getDate()
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
    std::string getTime()
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
    void mkdir_Date()
    {
        DATE_NOW = getDate();
        PIC_DIR = SAVE_DIR + DATE_NOW;
        // 确定文件和文件夹是否存在和访问权限
        //（_AccessMode参数：00表示只判断是否存在，02表示文件是否可执行， 04表示文件是否可写，06表示文件是否可读）
        // 有指定访问权限则返回0，否则函数返回-1
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
};

#endif