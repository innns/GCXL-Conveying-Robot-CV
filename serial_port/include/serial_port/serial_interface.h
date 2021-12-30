/*
 * @Descripttion: 
 * @version: 1.0
 * @Author: Gezp
 * @Email: ureinsecure@outlook.com
 * @Date: 2019-12-14 10:24:24
 * @LastEditors: innns
 * @LastEditTime: 2020-11-12 23:36:16
 * @FilePath: /junkcar_ws/serial_port/include/serial_port/serial_interface.h
 */
#ifndef RMDEMO_SERIAL_INTERFACE_H
#define RMDEMO_SERIAL_INTERFACE_H

#include "serial_port/fixed_common_base.h"
#include "serial_port/serialport_trans_dev.h"
#include <mutex>

typedef unsigned char uchar_t;
//协议
namespace robot_base
{

    static std::mutex serial_info_mutex;

    // 小主机和32之间的通信 CMD 设置
    typedef enum : unsigned char
    {
        // >>>>>> 发送用
        CMD_CURRENT_COLOR = 0x01, // 当前检测到的颜色
        CMD_TARGET_POS = 0x02,    // 当前检测到的靶心
        CMD_QR_MISSION = 0x03,    // 二维码读取的任务
        CMD_ALL_COLOR = 0x04,     // 所有颜色读取
        CMD_ACK = 0x05,           // 确认 切换相机 切换靶心等
        CMD_STORE_POS = 0x06,     // 库存区域位置

        // >>>>>> 接收用
        CMD_SET_MODE = 0x11,      // 检测颜色/检测靶心
        CMD_SWITCH_CAM = 0x12     // 切换相机
    } SerialPortCMD;

    class SerialInterface : public robot_base::FixedCommonBase
    {
    public:
        SerialInterface(void);
        ~SerialInterface(void);

        //private:
        robot_base::SerialPortTransDev mSerialPortTransDev;

    public:
        /** 初始化函数
        *  @param:  std::string devPath :串口设备路径
        *  @return: int :错误号，0代表无错误，１代表发生错误。
        */
#pragma region interface
        bool init(std::string devPath);

        bool current_color(char color_num);

        bool all_color(uchar_t m1, uchar_t m2, uchar_t m3, uchar_t m4, uchar_t m5, uchar_t m6);

        bool target_pos(int target_x, int target_y, int target_z);

        bool QR_mission(uchar_t m1, uchar_t m2, uchar_t m3, uchar_t m4, uchar_t m5, uchar_t m6);

        bool ACK(uchar_t ack = 0x00);

        bool store_pos(int target_x, int target_y, int target_z);

#pragma endregion
    };
}; // namespace robot_base

#endif
