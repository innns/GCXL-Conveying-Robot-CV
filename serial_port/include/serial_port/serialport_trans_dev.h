/*
 * @Descripttion: 
 * @version: 1.0
 * @Author: Gezp
 * @Email: ureinsecure@outlook.com
 * @Date: 2019-12-14 10:24:24
 * @LastEditors: innns
 * @LastEditTime: 2020-01-18 16:06:15
 * @FilePath: /catkin_rm/src/serial_port/include/serial_port/serialport_trans_dev.h
 */
/****************************************************************************
 *  Copyright (C) 2019 RoboMasterOS.
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
 *  file  : trans_dev_serialport.h
 *  brief : 消息传输设备具体实现
 *  author: gezp
 *  email : 1350824033@qq.com
 *  date  : 2019-1-02
 ***************************************************************************/
#ifndef ROBOT_BASE_SERIALPORT_TRANS_DEV_H
#define ROBOT_BASE_SERIALPORT_TRANS_DEV_H

#include "serial_port/trans_dev_interface.h"
#include "serial_port/serial_port.h"

namespace robot_base
{

    //对象适配器设计模式
    class SerialPortTransDev : public TransDevInterface
    {
    public:
        SerialPortTransDev();
        ~SerialPortTransDev();

    public:
        int init(std::string dev_path = "/dev/ttyUSB0");
        bool isOpen();
        int dataRecv(unsigned char *recv_buf, int data_len);
        int dataSend(unsigned char *send_buf, int data_len);

    private:
        SerialPort mMcuSerialPort;
    };

} // namespace robot_base

#endif //ROBOT_BASE_TRANS_DEV_SERIALPORT_H
