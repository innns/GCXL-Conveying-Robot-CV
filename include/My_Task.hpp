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
 * @version: 2.0
 * @Author: innns
 * @Email: ureinsecure@outlook.com
 * @Date: 2021-02-03 14:39:54
 * @LastEditors: innns
 * @LastEditTime: 2021-09-17 23:08:23
 * @FilePath: /junkcar/include/My_Task.hpp
*/

#ifndef MY_TASK_HPP
#define MY_TASK_HPP

#pragma region INIT
#include "Detector.h"
#include "USB_Cam.h"

#define SERIAL_PATH "/dev/ttyAMA0"

using namespace std;
using namespace cv;

RESULT_T result = {Point3i(), DEFAULT_MODE, {NONE}, NONE, {NONE}, Mat()};

enum CAM_NUM : unsigned char
{
    COLOR_CAM = 0x00,
    TARGET_CAM = 0x01
};

enum DETECT_MODE current_mode = DEFAULT_MODE;
enum TARGET_POS current_target = NONE_TAR;
enum CAM_NUM current_cam = COLOR_CAM;
bool change_cam = false;

const char *CAM[2] = {"/dev/camera0", "/dev/camera1"};
const char *TARGET[4] = {"None", "Left", "Center", "Right"};
const char *MAIN_WINDOWS = "MAIN_WINDOWS"; // windows name

USB_Cam cam(CAM[current_cam]);
Detector detector;

uchar_t _mode = 0x00;
uchar_t _target = 0x00;
uchar_t _cam = 0x00;

void recv_message();
#pragma endregion

bool my_task()
{
    thread recv_thread(recv_message);
    //以下线程用于发送线程
    string serial_path_send = SERIAL_PATH;
    robot_base::SerialInterface serial_interface;
    serial_interface.init(serial_path_send);
    Mat frame;
    bool all_color_wrong = false; //如果wrong会再扫一遍颜色

#ifdef PIC_BARCODE
    Ptr<barcode::BarcodeDetector> bardet = makePtr<barcode::BarcodeDetector>(); //如果不使用超分辨率则可以不指定模型路径
    vector<Point2f> corners;                                                    //返回的检测框的四个角点坐标，如果检测到N个条码，那么维度应该是[N][4][2]
    std::vector<std::string> decoded_info;                                      //返回的解码结果，如果解码失败，则为空string
    std::vector<barcode::BarcodeType> decoded_format;                           //返回的条码类型，如果解码失败，则为BarcodeType::NONE
    // bardet->detectAndDecode(src_img, decoded_info, decoded_format, corners);
#endif

    cout << ">>>>>>\nmy task init done\n<<<<<<\n";
    while (true) // 相机没有掉线以及没有按下Q准备退出
    {
        if (change_cam == true)
        {
            cam.switchCam(CAM[current_cam]);
            printf("Current cam: %d\n", current_cam);
            serial_interface.ACK();
            change_cam = false;
        }
        frame = cam.read();
        detector.updateMode(current_mode);
        detector.solveImg(frame, current_target);
        result = detector.getResult();
        switch (current_mode) // 更新MODE
        {
        case COLOR_MODE:
        {
#ifdef PIC_BARCODE
            if (bardet->detectAndDecode(frame, decoded_info, decoded_format, corners))
            {
                std::cout << "Bar: " << decoded_info[0] << "\n";
                if (decoded_info[0][0] == '1' | decoded_info[0][0] == '2' | decoded_info[0][0] == '3')
                {
                    result.now_color = COLOR(decoded_info[0][0] - '0');
                }
            }
            else
            {
                cout << "No bar\n";
            }
            // imshow("frame", frame);
            // imwrite("bar.jpg", frame);
            // waitKey(30);
#endif
            serial_interface.current_color(result.now_color);
            current_mode = DEFAULT_MODE;
        }
        break;
        case TARGET_MODE:
        {
            // TODO: (result.center.x, result.center.y, result.center.z)
            serial_interface.target_pos(result.center.x, result.center.y, result.center.z);
#ifdef _SHOW_TARGET_
            try
            {
                // imshow("img", result.img);
                destroyAllWindows();
                imshow("img_" + string(TARGET[current_target]), result.img);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
#endif
            current_mode = DEFAULT_MODE;
            current_target = NONE_TAR;
            cam.grab();
            cam.grab();
            cam.grab();
        }
        break;
        case QR_MODE:
        {
            // print info
            if (result.mission_color[0] == 0)
                break;
            cout << "Sent: Mission " << COLOR_LIST[result.mission_color[0]] << COLOR_LIST[result.mission_color[1]] << COLOR_LIST[result.mission_color[2]] << "+";
            cout << COLOR_LIST[result.mission_color[3]] << COLOR_LIST[result.mission_color[4]] << COLOR_LIST[result.mission_color[5]] << "\n";
            serial_interface.QR_mission(result.mission_color[0],
                                        result.mission_color[1],
                                        result.mission_color[2],
                                        result.mission_color[3],
                                        result.mission_color[4],
                                        result.mission_color[5]);
            current_mode = DEFAULT_MODE;
        }
        break;
        case ALL_COLOR_MODE:
        {
            // print info
            // 国赛发送顺序改动为下层+上层
            if (result.all_color[3] != 0)
            {
                cout << "Sent: ALL " << COLOR_LIST[result.all_color[3]] << COLOR_LIST[result.all_color[4]] << COLOR_LIST[result.all_color[5]] << "+";
                cout << COLOR_LIST[result.all_color[0]] << COLOR_LIST[result.all_color[1]] << COLOR_LIST[result.all_color[2]] << "\n";
                serial_interface.all_color(result.all_color[3],
                                           result.all_color[4],
                                           result.all_color[5],
                                           result.all_color[0],
                                           result.all_color[1],
                                           result.all_color[2]);
                current_mode = DEFAULT_MODE;
            }
            else if (!all_color_wrong)
            {
                all_color_wrong = true;
                break;
            }
            current_mode = DEFAULT_MODE;
            // imwrite("all.jpg", result.img);
        }
        break;
        case STORE_MODE:
            // TODO: (result.center.x, result.center.y, result.center.z)
            serial_interface.store_pos(result.center.x, result.center.y, result.center.z);
#ifdef _SHOW_TARGET_
            try
            {
                // imshow("img", result.img);
                destroyAllWindows();
                imshow("img_" + string(TARGET[current_target]), result.img);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
#endif
            current_mode = DEFAULT_MODE;
            current_target = NONE_TAR;
            cam.grab();
            cam.grab();
            cam.grab();
        default:
            break;
        }
        // system("clear");
        // cv::waitKey(20);
        usleep(8000); // sleep 8ms
    }
    recv_thread.join();
    return false;
}

/** 接受串口进程函数
 * @name: recv_message()
*/
void recv_message()
{
    string serial_path_send = SERIAL_PATH;
    robot_base::SerialInterface serial_interface;
    serial_interface.init(serial_path_send);
    robot_base::FixedPacket recv_packet;
    usleep(800);
    std::cout << "RECV ACTIVATE \n";
    while (true)
    {
        robot_base::FixedPacket recv_pack;
        serial_interface.recvPacket(recv_pack);
        uchar_t cmd;
        // recv_pack 正确解包为真 否为非
        // >>>>> 解包 CMD
        if (!recv_pack.unloadData(cmd, 1))
        {
            cout << "unloads WRONG\n";
            continue;
        }
        switch (cmd)
        {
        case robot_base::CMD_SET_MODE:
        {
            robot_base::serial_info_mutex.lock();

            // >>>>> 解包 DETECTOR MODE
            if (!recv_pack.unloadData(_mode, 2))
            {
                robot_base::serial_info_mutex.unlock();
                continue;
            }
            if (_mode == AMP_LIMIT(_mode, 0x06, 0x01))
            {
                current_mode = DETECT_MODE(_mode);
                printf("Recv MODE: %d\n", current_mode);
            }

            // >>>>> 解包 TARGET POS
            if (!recv_pack.unloadData(_target, 3))
            {
                robot_base::serial_info_mutex.unlock();
                continue;
            }
            if (_target == AMP_LIMIT(_target, 0x03, 0x01))
            {
                current_target = TARGET_POS(_target);
                printf("Recv TAR: %s\n", TARGET[current_target]);
            }

            robot_base::serial_info_mutex.unlock();
            continue;
        }
        break;
        case robot_base::CMD_SWITCH_CAM:
        {
            robot_base::serial_info_mutex.lock();

            // >>>>> 解包 CAMERA NUM
            if (!recv_pack.unloadData(_cam, 2))
            {
                robot_base::serial_info_mutex.unlock();
                continue;
            }
            if (_cam == AMP_LIMIT(_cam, 0x01, 0x00))
            {
                current_cam = CAM_NUM(_cam);
                printf("Recv CAM: %d\n", current_cam);
                change_cam = true;
            }

            robot_base::serial_info_mutex.unlock();
            continue;
        }
        break;
        default:
            break;
        }
    }
}

#endif
