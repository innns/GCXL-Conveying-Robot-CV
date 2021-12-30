////////////////////////////////////////////////////////////////////////////////
///Copyright(c)     UESTC ROBOMASTER2018      SerialPort Code for robot
///ALL RIGHTS RESERVED
///@file:serial_interface.cpp
///@brief: 机器人控制基本接口源文件，包含对车底盘及云台的基本接口。
///@vesion 1.0
///@author: gezp
///@email: 1350824033@qq.com
///@date: 18-3-4
///修订历史：
////////////////////////////////////////////////////////////////////////////////
#include "serial_port/serial_interface.h"

using namespace robot_base;

SerialInterface::SerialInterface() {}

SerialInterface::~SerialInterface() {}

bool SerialInterface::init(std::string devPath)
{
    if (mSerialPortTransDev.init(devPath) == 0)
    {
        setTransDev(&mSerialPortTransDev);
        return true;
    }
    else
    {
        return false;
    }
}

bool SerialInterface::current_color(char color_num)
{
    FixedPacket packet;
    packet.loadData(CMD_CURRENT_COLOR, 1);
    packet.loadData(color_num, 2);
    packet.pack();
    return sendPacket(packet) == 0;
}

bool SerialInterface::all_color(uchar_t m1, uchar_t m2, uchar_t m3, uchar_t m4, uchar_t m5, uchar_t m6)
{
    FixedPacket packet;
    packet.loadData(CMD_ALL_COLOR, 1);
    packet.loadData(m1, 2);
    packet.loadData(m2, 3);
    packet.loadData(m3, 4);
    packet.loadData(m4, 5);
    packet.loadData(m5, 6);
    packet.loadData(m6, 7);
    packet.pack();
    return sendPacket(packet) == 0;
}

bool SerialInterface::target_pos(int target_x, int target_y, int target_z)
{
    FixedPacket packet;
    packet.loadData(CMD_TARGET_POS, 1);
    packet.loadData(target_x, 2);
    packet.loadData(target_y, 6);
    packet.loadData(target_z, 10);
    packet.pack();
    return sendPacket(packet) == 0;
}

bool SerialInterface::QR_mission(uchar_t m1,uchar_t m2,uchar_t m3,uchar_t m4,uchar_t m5,uchar_t m6)
{
    FixedPacket packet;
    packet.loadData(CMD_QR_MISSION, 1);
    packet.loadData(m1, 2);
    packet.loadData(m2, 3);
    packet.loadData(m3, 4);
    packet.loadData(m4, 5);
    packet.loadData(m5, 6);
    packet.loadData(m6, 7);
    packet.pack();
    return sendPacket(packet) == 0;
}

bool SerialInterface::ACK(uchar_t ack){
    FixedPacket packet;
    packet.loadData(CMD_ACK,1);
    packet.loadData(ack,2);
    packet.pack();
    return sendPacket(packet) == 0;
}


bool SerialInterface::store_pos(int target_x,int target_y,int target_z){
    FixedPacket packet;
    packet.loadData(CMD_TARGET_POS,1);
    packet.loadData(target_x,2);
    packet.loadData(target_y,6);
    packet.loadData(target_z,10);
    packet.pack();
    return sendPacket(packet) == 0;
}
