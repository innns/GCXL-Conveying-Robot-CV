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
 *  file  : fixed_packet.cpp
 *  brief : 定长数据包封装
 *  author: gezp
 *  email : 1350824033@qq.com
 *  date  : 2019-1-02
 ***************************************************************************/
#include <cstring>
#include "serial_port/fixed_packet.h"

using namespace robot_base;

#include <iostream>

FixedPacket::FixedPacket()
{
    len_ = 16;
    recv_buf_len_ = 0;
    memset(buffer_, 0, len_);
    flag_ = false;
}

FixedPacket::~FixedPacket()
{
}

//用于打包事计算校验和
//检验和包括帧头帧尾
inline unsigned char FixedPacket::get_check_sum() const
{
    // int check_sum = 0;
    // for (int i = 0; i < len_ - 2; ++i)
    // {
    //     check_sum += buffer_[i];
    // }
    // check_sum += 0xBB;
    // return (unsigned char)(check_sum & 0xAA);
    return 0xf0; // 固定校验位=0xf0
}

inline bool FixedPacket::check_sum_noerror() const
{
    // bool no_error = true;
    // int check_sum = 0;
    // for (int i = 0; i < len_ - 2; ++i)
    // {
    //     check_sum += buffer_[i];
    // }
    // check_sum += 0xBB;
    // return ((unsigned char)(check_sum & 0xAA) == buffer_[len_ - 2]);
    return ((unsigned char)(0xF0) == buffer_[len_ - 2]);
}

int FixedPacket::pack()
{
    buffer_[0] = 0xAA;        //帧头
    buffer_[len_ - 1] = 0xBB; //帧尾
    buffer_[len_ - 2] = get_check_sum();
    flag_ = true;
    return 0; //edit by innns
}

//数据帧解析，包含数据帧检查
//返回-3校验失败
//通过将数据拷贝到buffer_中，在unload数据时对buffer_进行访问
int FixedPacket::unPack(unsigned char *tmp_buffer, int recv_len)
{
    //检查帧头，帧尾,长度,如果正常帧，直接unpack
    // printf("%02X %02X\n", tmp_buffer[0], tmp_buffer[15]);
    //std::cout<<std::hex<<tmp_buffer[0]<< "   "<<std::dec << tmp_buffer[0]<<std::endl;
    // std::cout << "len " << len_ << " tmp check \n";
    // for (int i = 0; i < 16; i++)
    // {
    //     printf("[%02d]:    %02X\n", i, tmp_buffer[i]);
    // }
    if (recv_len == len_ && tmp_buffer[0] == 0xAA && tmp_buffer[len_ - 1] == 0xBB)
    {
        memcpy(buffer_, tmp_buffer, len_);
        recv_buf_len_ = 0;
        if (check_sum_noerror())
            return 0;
        else
        {
            // std::cout << "check " << buffer_[len_ - 2] << std::endl;
            return -3;
        }
    }
    //断帧缓存待拼接，超过缓冲长度清空缓冲区
    if (recv_len + recv_buf_len_ > len_ * 3)
    {
        recv_buf_len_ = 0;
    }
    memcpy(recv_buffer_ + recv_buf_len_, tmp_buffer, recv_len); //缓存
    recv_buf_len_ += recv_len;
    for (int start_point = 0; (start_point + len_) <= recv_buf_len_; start_point++)
    {
        if (recv_buffer_[start_point] == 0xAA && recv_buffer_[start_point + len_ - 1] == 0xBB)
        {
            memcpy(buffer_, recv_buffer_ + start_point, len_);
            int k = 0;
            for (int j = start_point + len_; j < recv_buf_len_; j++)
            { //读取一帧后，更新缓存
                recv_buffer_[k] = recv_buffer_[j];
                k++;
            }
            recv_buf_len_ = k;
            if (check_sum_noerror())
                return 0;
            else
                return -3;
        }
    }
    return -2; //表明断帧，或错误帧。
}
//数据帧清空
void FixedPacket::clear()
{
    memset(buffer_, 0, len_); //每个字节都用0填充
    flag_ = false;
}
