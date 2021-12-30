/*
 * @Descripttion: 
 * @version: 1.0
 * @Author: RM
 * @Email: ureinsecure@outlook.com
 * @Date: 2019-12-14 10:24:24
 * @LastEditors: innns
 * @LastEditTime: 2020-10-02 17:18:45
 * @FilePath: /catkin_rm/src/armor_detect/nodes/TR_detect_demo.cpp
 */

// #include "armor_detect/detect.h"
// #include "ros/package.h"
// #include <ros/ros.h>
#include <iostream>
#include <thread>
// #include <robot_cam_uestc/mind_vision_cam_dev.h>
// #include <armor_detect/coor_tools.hpp>
#include <math.h>
#include <serial_port/serial_interface.h>

using namespace std;
using namespace robot_cam_uestc;

#define rootMeanSquare(i, j) pow((i) * (i) + (j) * (j), 0.5)

bool cam_init_done = false; //等几帧让相机输出稳定
int cam_cout = 0;
float pre_dis = 0.1, pre_angle = 0.1;
bool pos_change = true;

void recv_message()
{
    string serial_path_send = "/dev/ttyUSB0";
    robot_base::SerialInterface serial_interface;
    serial_interface.init(serial_path_send);

    robot_base::FixedPacket recv_packet;
    usleep(800);
    while (true)
    {
        robot_base::FixedPacket recv_pack;
        serial_interface.recvPacket(recv_pack);
        unsigned char cmd;
        if (recv_pack.unloadData(cmd, 1))
            ;
        else
            continue;
        if (cmd == 0x01)
        { //云台绝对角度
            robot_base::serial_info_mutex.lock();
            float data_32_x, data_32_y, data_32_z;
            // recv_pack 正确解包为真 否为非
            if (!recv_pack.unloadData(data_32_x, 2))
            {
                robot_base::serial_info_mutex.unlock();
                continue;
            }
            if (!recv_pack.unloadData(data_32_y, 6))
            {
                robot_base::serial_info_mutex.unlock();
                continue;
            }
            if (!recv_pack.unloadData(data_32_z, 10))
            {
                robot_base::serial_info_mutex.unlock();
                continue;
            }
            // cout << "X " << data_32_x << " "
            //      << "Y " << data_32_y << " "
            //      << "Z " << data_32_z << "\n";
            robot_base::serial_info_mutex.unlock();
        }
    }
    // cout<<"without recv_message\n";
}

int main(int argc, char **argv)
{
    //以下线程用于接收线程测试
    thread recv_thread(recv_message);
    //以下线程用于发送线程测试
    string serial_path_send = "/dev/ttyUSB0";
    robot_base::SerialInterface serial_interface;
    serial_interface.init(serial_path_send);
    const char *tracker_win = "TR_tracker";
    cv::namedWindow(tracker_win, WINDOW_AUTOSIZE);
    cv::Point2f point_2D;
    ArmorDetect armor_detect_;
    armor_detect_.init();
    cout << "Initing------------\n";
    MindVisionCamDev cap;
    std::string config_path = ros::package::getPath("robot_cam_uestc") + "/res/cam_config_dev.yaml";
    cap.init(config_path);
    cv::Mat img;
    cv::Mat output_img;
    char key = 0;
    float x, y, z, theta;

    while (key != 'q' && cvGetWindowHandle(tracker_win))
    {
        Mat img;
        if ((!cap.capImg(img)) && cam_init_done)
        {
            armor_detect_.setImage(img);
            armor_detect_.solveImg();
            //发布四个点
            armor_detect_.getPnpResult(&x, &y, &z, &theta);
            point_2D.x = x;
            point_2D.y = z;

            output_img = armor_detect_.getImg();

            cv::circle(output_img, point_2D, 10, (255, 255, 0));
            float distance = rootMeanSquare(x, z);
            float angle = x / distance;
            
            pos_change =
                (abs((distance - pre_dis) / distance)) > 0.004
                    ? true
                    : ((abs((angle - pre_angle) / angle) > 0.002) ? true : false);
            if (pos_change)
            {
                cout << "A: " << (abs((distance - pre_dis) / distance)) << " D: " << (abs((angle - pre_angle) / angle)) << "\n";
                pre_dis = distance;
                pre_angle = angle;
                if (distance < 16000)
                {
                    serial_interface.sendPolarPosition(angle, distance);
                }
            }

            cout<<"x\n"<<x<<"y\n"<<y<<"z\n"<<z<<"t\n"<<theta<<"\n";
            //pub_point2D.publish(point_2D);
            //发布绘制后的图片
            // armor_detect_.showResult();
            // img_pub.header.stamp = ros::Time::now();
            // img_pub.encoding = sensor_msgs::image_encodings::BGR8;
            // img_pub.image = armor_detect_.getImg();
            // pub.publish(img_pub.toImageMsg());
            // armor_detect_.showResult("HAHA");
            cv::resize(output_img,output_img,Size(600,400));
            cv::imshow(tracker_win, output_img);

            key = waitKey(5);
        }
        else
        {
            cam_cout++;
            if (cam_cout > 5)
            {
                cam_init_done = true;
            }
        }
    }
    recv_thread.join();
    return EXIT_SUCCESS;
}
