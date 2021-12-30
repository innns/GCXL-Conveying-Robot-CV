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
 * @Date: 2021-07-02 18:05:17
 * @LastEditors: innns
 * @LastEditTime: 2021-08-23 15:31:54
 * @FilePath: /junkcar/src/Cam_MT.cpp
*/
#include "Cam_MT.h"

Cam_MT::Cam_MT(const char *index, bool use_config, int width, int height)
{
    mkdir_Date();
    if (use_config)
    {
        initCam(index);
    }
    else
    {
        res_w = width;
        res_h = height;
        initCam(index, false);
    }
}

Cam_MT::~Cam_MT()
{
    _Is_Opened = false;
    _Thread->join();
    if (_Capture->isOpened())
    {
        _Capture->release();
    }

    delete _Thread;
    delete _Mutex;
    delete _Capture;
    delete _Frame;
}

void Cam_MT::initCam(const char *index, bool use_config)
{
    _index = index;
    _Capture = new cv::VideoCapture(index);
    _Capture->set(CAP_PROP_FOURCC, ('M', 'J', 'P', 'G'));
    _Capture->set(CAP_PROP_FPS, 20);      // FPS 30
    if (use_config)
    {
        cv::FileStorage f(CONFIG_PATH, cv::FileStorage::READ);
        if (f.isOpened())
        {
            f["res_w"] >> res_w;
            f["res_h"] >> res_h;
            // f["white_balance"] >> white_balance;
            f["exposure"] >> exposure;
            cout << "USING config\nRes : " << res_w << "*" << res_h << "\n";
            _Capture->set(CAP_PROP_FRAME_WIDTH, res_w);
            _Capture->set(CAP_PROP_FRAME_HEIGHT, res_h);
            _Capture->set(CAP_PROP_EXPOSURE, exposure); // 曝光 50
            // _Capture->set(CAP_PROP_BUFFERSIZE, 5); // 缓冲区5张
            // 相机参数设定查阅 https://blog.csdn.net/qq_41498261/article/details/103300531
        }
        f.release();
    }
    else
    {
        cout << "WITHOUT config\nRes : " << res_w << "*" << res_h << "\n";
        _Capture->set(cv::CAP_PROP_FRAME_WIDTH, res_w);
        _Capture->set(cv::CAP_PROP_FRAME_HEIGHT, res_h);
    }
    _Is_Opened = true;
    _Frame = new cv::Mat();
    _Mutex = new std::mutex();

    for (int i = 0; i < 5; i++)
    {
        // 先过五张图片使图像稳定
        cout << "==";
        cv::waitKey(30);
        _Capture->grab();
    }
    cout << "\n";

    _Thread = new std::thread(&Cam_MT::updateFrame, this);
    cout << "Begin Thread\n";
}

bool Cam_MT::switchCam(const char *index, bool use_config)
{
    _Is_Opened = false;
    _Thread->join();
    if (_Capture->isOpened())
    {
        _Capture->release();
    }

    delete _Thread;
    delete _Mutex;
    delete _Capture;
    delete _Frame;
    cout << "Deleted old cam\nSwitching to new cam\n";

    initCam(index, use_config);
    return true;
}

void Cam_MT::updateFrame()
{
    while (_Is_Opened)
    {
        if (_Mutex->try_lock())
        {
            _Capture->read(*_Frame);
            _Mutex->unlock();
        }
        cv::waitKey(10);
    }
    cout << "OUT OF LOOP\n";
}

// int cnt = 0;
bool Cam_MT::read(cv::Mat &frame)
{
    int cnt = 0;
    bool try_reconnect = false;
    while (_Frame->empty())
    {
        cout << "_Frame EMPTY\n";
        cv::waitKey(500);
        cnt++;
        if (cnt >= 4) // 2s
        {
            if (!try_reconnect)
            {
                switchCam(_index.c_str());
                cv::waitKey(5000); // 5s
                try_reconnect = true;
            }
            if (cnt >= 6) // 3s
            {
                _Is_Opened = false;
                return _Is_Opened;
            }
        }
    }
    _Mutex->lock();
    // cout << "lock\n";
    _Frame->copyTo(frame);
    _Mutex->unlock();
    cv::waitKey(40);
    return _Is_Opened;
}

Mat Cam_MT::read()
{
    Mat temp_img = Mat();
    read(temp_img);
    return temp_img;
}
