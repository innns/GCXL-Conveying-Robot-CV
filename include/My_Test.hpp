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
 * @Descripttion: 测试模块。测试任务。
 * @version: 1.0
 * @Author: innns
 * @Email: ureinsecure@outlook.com
 * @Date: 2021-05-10 20:02:22
 * @LastEditors: innns
 * @LastEditTime: 2021-09-17 10:22:40
 * @FilePath: /GCXL-Conveying-Robot-CV/include/My_Test.hpp
*/

#ifndef MY_TEST_HPP
#define MY_TEST_HPP

#include "USB_Cam.h"
#include "Detector.h"

#define SERIAL_PATH "/dev/ttyAMA0"

using namespace std;
using namespace cv;

// typedef struct
// {
//     Point3i center;
//     DETECT_MODE mode;
//     COLOR mission_color[6];
//     COLOR now_color;
//     COLOR all_color[6];
//     Mat img;
// } RESULT_T;
RESULT_T result = {Point3i(), DEFAULT_MODE, {NONE}, NONE, {NONE}, Mat()};

enum DETECT_MODE current_mode = DEFAULT_MODE;

const char *CAM[2] = {"/dev/camera0", "/dev/camera1"};

const char *MAIN_WINDOWS = "MAIN_WINDOWS"; // windows name

bool isQuiting = false;
unsigned char temp_mode_num = 0x00;
unsigned char temp_camera_num = 0x00;

/** enum COLOR 2 char
 * @name: i2c_COLOR
 * @param {COLOR} *num
 * @param {int} size
 * @param {char} *re
 * @return {*}
*/
void i2c_COLOR(enum COLOR *num, int size, char *re)
{
    for (size_t i = 0; i < size; i++)
    {
        re[i] = num[i] + '0';
    }
}

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
        unsigned char cmd;
        if (!recv_pack.unloadData(cmd, 1))
        {
            cout << "unloads WRONG\n";
            continue;
        }
        if (cmd == robot_base::CMD_SET_MODE)
        {
            robot_base::serial_info_mutex.lock();
            // recv_pack 正确解包为真 否为非
            // 解包模式
            if (!recv_pack.unloadData(temp_mode_num, 2))
            {
                robot_base::serial_info_mutex.unlock();
                continue;
            }
            else
            {
                if (temp_mode_num == AMP_LIMIT(temp_mode_num, 0x04, 0x00))
                {
                    current_mode = DETECT_MODE(temp_mode_num);
                }
            }
            if (current_mode)
                std::cout << "Receive: MODE " << current_mode << "\n";

            if (!recv_pack.unloadData(temp_camera_num, 6))
            {
                robot_base::serial_info_mutex.unlock();
                continue;
            }
            else
            {
                temp_camera_num = AMP_LIMIT(temp_camera_num, 0x01, 0x00);
            }
            if (temp_camera_num)
                std::cout << "CAM: " << temp_camera_num << "\n";
            robot_base::serial_info_mutex.unlock();
            continue;
        }
    }
}

/**
 * @name: 
 * @msg: 
 * @param {*}
 * @return {*}
*/
bool test_task()
{

    USB_Cam cam(CAM[0]);
    Detector detector;

    cout << ">>>>>>\nmy task init done\n<<<<<<\n";
    //current_mode = QR_MODE;
    while (LIKELY(cam.isOpened())) // 相机没有掉线以及没有按下Q准备退出
    {
        int int_mode = 0;
        cout << "DEFAULT_MODE = 0x00,  // 空模式 0 \nCOLOR_MODE = 0x01,    // 读取颜色 1 \nTARGET_MODE = 0x02,   // 寻找靶心 2 \nQR_MODE = 0x03,       // 读取二维码 3 \nALL_COLOR_MODE = 0x04 // 读取所有颜色 4\n";
        cin >> int_mode;
        current_mode = DETECT_MODE(int_mode);
        detector.updateMode(current_mode); // 更新MODE
        Mat mat;
        cam.read(mat);
        detector.solveImg(mat);
        result = detector.getResult();

        switch (result.mode)
        {
        case COLOR_MODE:
        {
            // serial_interface.current_color(result.now_color);
            current_mode = DEFAULT_MODE;
        }
        break;
        case TARGET_MODE:
        {
            // serial_interface.target_pos((result.center.x - 320), (240 - result.center.y));
            current_mode = DEFAULT_MODE;
        }
        break;
        case QR_MODE:
        {
            int sum = 0;
            for (int cnt = 0; cnt < 6; cnt++)
            {
                sum += result.mission_color[cnt];
            }
            if (sum == 12)
            {
                char mission[6] = {'1', '2', '3', '1', '2', '3'}; //init mission
                i2c_COLOR(result.mission_color, 6, mission);
                // serial_interface.QR_mission(mission[0], mission[1], mission[2], mission[3], mission[4], mission[5]);
                cout << "Sent: Mission" << COLOR_LIST[result.mission_color[0]] << COLOR_LIST[result.mission_color[1]] << COLOR_LIST[result.mission_color[2]] << "\n";
                current_mode = DEFAULT_MODE;
            }
        }
        break;
        case ALL_COLOR_MODE:
        {
            char all_c[6] = {'1', '2', '3', '1', '2', '3'}; //init all_c(olor)
            i2c_COLOR(result.all_color, 6, all_c);
            // cout<<"Sent: ALL"<<all_c[0]<<all_c[1]<<all_c[2]<<"+"<<all_c[3]<<all_c[4]<<all_c[5]<<endl;
            // serial_interface.all_color(all_c[0], all_c[1], all_c[2], all_c[3], all_c[4], all_c[5]);
            current_mode = DEFAULT_MODE;
        }
        break;
        default:
            // current_mode = DEFAULT_MODE;
            break;
        }
        // system("clear");
        cv::waitKey(20);
    }
    // recv_thread.join();
    return false;
}

bool test_all_color()
{
    Detector detector;
    // VideoCapture cap("bgrgrb_.avi");

    // Mat test_img;
    // Mat test_img = imread("hsv.jpg");
    USB_Cam cam(CAM[0]);

    current_mode = ALL_COLOR_MODE;
    detector.updateMode(current_mode);

    cout << ">>>>>>\ntest_all_color init done\n<<<<<<\n";
    Mat mat;
    while (LIKELY(cam.isOpened()))
    {
        // while (cap.read(test_img) && !test_img.empty())
        // {
        // imshow("1", test_img);
        // cv::waitKey(30);
        // resize(test_img,test_img,Size(640,480))
        cam.read(mat);
        detector.solveImg(mat);
        result = detector.getResult();

        if (result.mode == ALL_COLOR_MODE)
        {
            char all_c[6] = {'1', '2', '3', '1', '2', '3'}; //init all_c(olor)
            i2c_COLOR(result.all_color, 6, all_c);
            cout << all_c[0] << all_c[1] << all_c[2] << "+" << all_c[3] << all_c[4] << all_c[5] << endl;

            cout << COLOR_LIST[result.all_color[0]] << " " << COLOR_LIST[result.all_color[1]] << " " << COLOR_LIST[result.all_color[2]];
            cout << " + ";
            cout << COLOR_LIST[result.all_color[3]] << " " << COLOR_LIST[result.all_color[4]] << " " << COLOR_LIST[result.all_color[5]] << endl;

            // serial_interface.all_color(all_c[0], all_c[1], all_c[2], all_c[3], all_c[4], all_c[5]);
            try
            {
                imshow("src", mat);
                imshow("1", result.img);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            
        }
        // system("clear");
        // return false;
        if (cv::waitKey(30) == 27) //ESC
            return false;
    }
    cout << "DONE\n";

    // recv_thread.join();
    return false;
}

bool test_cam(int index)
{
    // thread recv_thread(recv_message);
    // //以下线程用于发送线程测试
    // string serial_path_send = SERIAL_PATH;
    // robot_base::SerialInterface serial_interface;
    // serial_interface.init(serial_path_send);
    USB_Cam cam(CAM[index]);
    Detector detector;
    Mat mat;
    cout << ">>>>>>\nmy task init done\n<<<<<<\n";
    current_mode = ALL_COLOR_MODE;
    while (LIKELY(cam.isOpened())) // 相机没有掉线以及没有按下Q准备退出
    {
        cam.read(mat); // 函数内会更新Data
        detector.updateMode(current_mode);
        detector.solveImg(mat);
        result = detector.getResult();
        cv::imshow("cam" + to_string(index), mat);

        cv::imwrite("cam" + to_string(index) + ".jpg", mat);
    }
    // recv_thread.join();
    return false;
}

bool auto_test_all_cam()
{
    bool inited = true;
    set<int> cam_dead;
    USB_Cam cam("donot_open_cam", 1);

    cout << ">>>>>>\nAuto Test All Cam\n<<<<<<\n";
    for (int i = 0; i < 8; i++)
    {
        int index = i % 4;
        if (cam.switchCam(CAM[index]), 2)
        {
            // cout << "Cam " << index << " online" << endl;
            Mat mat;
            cam.read(mat); // 函数内会更新Data
            if (mat.empty())
            {
                cout << "Cam " << index << " WRONG!!! Plz Check it!!!" << endl;
                cam_dead.insert(index);
                inited = false;
            }
        }
        else
        {
            cout << "Cam " << index << " DEAD!!! Plz Check it!!!" << endl;
            cam_dead.insert(index);
            inited = false;
        }
    }
    if (inited)
    {
        cout << ">>>>>>>>>>>>>>>>>>>>\n";
        cout << "All camera is online\n";
        cout << "<<<<<<<<<<<<<<<<<<<<\n";
    }
    else
    {
        cout << ">>>>>>>>>>>>>>>>>>>>\n";
        cout << "Cam";
        for (auto it : cam_dead)
        {
            cout << " " << it << " ";
        }
        cout << "DEAD! CHECK IT!!!\n";
        cout << "<<<<<<<<<<<<<<<<<<<<\n";
    }
    // recv_thread.join();
    return inited;
}

bool test_uart()
{
    thread recv_thread(recv_message);
    //以下线程用于发送线程测试
    string serial_path_send = SERIAL_PATH;
    robot_base::SerialInterface serial_interface;
    serial_interface.init(serial_path_send);
    // USB_Cam cam(CAM[0]);
    // Detector detector;

    cout << ">>>>>>\nmy task init done\n<<<<<<\n";
    //current_mode = QR_MODE;
    while (1) // 相机没有掉线以及没有按下Q准备退出
    {

        if (current_mode == COLOR_MODE)
        {
            // cout << "COLOR_MODE\n";
            serial_interface.current_color('2');
        }
        if (current_mode == TARGET_MODE)
        {
            serial_interface.target_pos((320), (240), 0);
            // cout << "Sent: X Y" << (result.center.x - 320) << " " << (240 - result.center.y) << "\n";
        }
        if (current_mode == QR_MODE)
        {
            char mission[6] = {'1', '2', '3', '1', '2', '3'}; //init mission
            // i2c_COLOR(result.mission_color, 6, mission);
            serial_interface.QR_mission(mission[0], mission[1], mission[2], mission[3], mission[4], mission[5]);
            // cout << "Sent: Mission" << COLOR_LIST[result.mission_color[0]] << COLOR_LIST[result.mission_color[1]] << COLOR_LIST[result.mission_color[2]] << "\n";
            // current_mode = DEFAULT_MODE;
        }
        if (current_mode == ALL_COLOR_MODE)
        {
            char all_c[6] = {'2', '2', '3', '1', '2', '3'}; //init all_c(olor)
            // i2c_COLOR(result.all_color, 6, all_c);
            serial_interface.all_color(all_c[0], all_c[1], all_c[2], all_c[3], all_c[4], all_c[5]);
        }
        else
        {
            // destroyAllWindows();
        }
        // system("clear");
        cv::waitKey(20);
    }
    recv_thread.join();
    return false;
}

bool test_get_QR_center()
{
    thread recv_thread(recv_message);
    //以下线程用于发送线程测试
    string serial_path_send = SERIAL_PATH;
    robot_base::SerialInterface serial_interface;
    serial_interface.init(serial_path_send);
    // Detector detector;
    USB_Cam cam(CAM[3]);
    // string TARGET_EG[3] = {"R.jpg", "G.jpg", "B.jpg"};
    cout << ">>>>>>\ntest center init done\n<<<<<<\n";
    current_mode = DEFAULT_MODE;
    double ratio_x = 5.5;
    double ratio_y = 4;
    cv::FileStorage f(CONFIG_PATH, cv::FileStorage::READ);

    if (f.isOpened())
    {
        cout << "Using config\n";

        f["ratio_x"] >> ratio_x;
        f["ratio_y"] >> ratio_y;
    }
    f.release();

    int NOTHING = 0;
    QRCodeDetector QRCD;
    float last_x = 0;
    float last_y = 0;
    while (true)
    {
        Mat mat;
        cam.read(mat); // 函数内会更新Data
        Mat img = mat.clone();
        if (img.empty() || current_mode != TARGET_MODE)
        {
            continue;
        }
        Mat gray;
        cvtColor(img, gray, CV_BGR2GRAY);
        vector<Point> points;
        bool isQRcode = QRCD.detect(gray, points); // 检测二维码位置
        if (isQRcode)
        {
            float sum_x = 0;
            float sum_y = 0;
            for (auto &p : points)
            {
                circle(img, p, 2, CV_RGB(0, 255, 255));
                cv::imwrite("img.jpg", img);
                // cout <<"x y" <<p.x << " "<<p.y<<"\n";
                sum_x += p.x / 4;
                sum_y += p.y / 4;
            }
            cout << "x " << sum_x << " y " << sum_y << "\n";
            if (abs(last_x + last_y - sum_x - sum_y) > 2)
            {
                last_x = sum_x;
                last_y = sum_y;
                continue;
            }
            sum_x = (401 - sum_x) / ratio_x;
            sum_y = (205 - sum_y) / ratio_y;

            if (sum_x > 6)
                sum_x = sum_x * pow(0.95, abs(sum_x / 10));

            if (sum_y > 6)
                sum_y = sum_y * pow(0.95, abs(sum_y / 10));

            serial_interface.target_pos((int)sum_x, (int)sum_y, 0);
            cout << " SEND XY " << (int)sum_x << " " << (int)sum_y << "\n";
            current_mode = DEFAULT_MODE;
        }
        else
        {
            cout << "NO QR\n";
        }
        // detector.updateMode(current_mode);
        // detector.solveImg(img);
        // result = detector.getResult();
    }
    recv_thread.join();
    return false;
}

bool test_get_center(Mat img)
{
    Detector detector;
    detector.updateMode(TARGET_MODE);
    detector.solveImg(img, TARGET_POS(2));
    return false;
}

bool test_get_center()
{
    Detector detector;
    USB_Cam cam(CAM[1]);
    cout << ">>>>>>\ncam init done\n<<<<<<\n";
    namedWindow("MAIN");
    waitKey(3000);

    current_mode = TARGET_MODE;
    detector.updateMode(current_mode);

    cout << ">>>>>>\ntest_target init done\n<<<<<<\n";
    int num = 1;

    while (LIKELY(cam.isOpened()))
    {
        Mat mat;
        cam.read(mat); // 函数内会更新Data
        num = AMP_LIMIT(num, 3, 0);
        detector.solveImg(mat, TARGET_POS(num));
        result = detector.getResult();
        cv::imshow("MAIN", result.img);
        printf("x: %d  y: %d  z: %d \n", result.center.x, result.center.y, result.center.z);
        num = waitKey(0);
        switch (num)
        {
        case 27:
            return false;
        case 48:
            num = 0;
            break;
        case 49:
            num = 1;
            break;
        case 50:
            num = 2;
            break;
        default:
            cout << num << "\n";
            break;
        }
    }
    cout << "DONE\n";

    // recv_thread.join();
    return false;
}

void test_show_mission()
{
    Detector detector;
    detector.updateMode(QR_MODE);
    detector.solveImg(Mat());
    detector.showMission();
}
#endif