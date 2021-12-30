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
 * @Descripttion: 头文件依赖放一起了
 * @version: 1.0
 * @Author: innns
 * @Email: ureinsecure@outlook.com
 * @Date: 2021-08-04 16:06:41
 * @LastEditors: innns
 * @LastEditTime: 2021-09-17 23:31:56
 * @FilePath: /GCXL-Conveying-Robot-CV/include/essential.h
*/
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/xphoto/white_balance.hpp>
#include <opencv2/barcode.hpp>
#include <opencv2/imgproc.hpp>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <math.h>
#include <stdint.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <cmath>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

// 可能需要修改路径
#ifdef __x86_64__
#define SAVE_DIR "/home/ins/junkcar/cam_save/"
#define CONFIG_PATH "/home/ins/junkcar/config/cam.yaml"
#else
#define SAVE_DIR "/home/pi/junkcar/cam_save/"
#define CONFIG_PATH "/home/pi/junkcar/config/cam.yaml"
#endif

#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#define AMP_LIMIT(x, max, min) (((x) > (max)) ? (max) : (((x) < (min)) ? (min) : (x)))

#include <serial_port/serial_interface.h>
#include "async-sockets/udpsocket.hpp"
#include "async-sockets/udpserver.hpp"

#ifndef RELEASE
// #define DEBUG
#define _SAVE_TEMP_
#define _SHOW_TARGET_
// #define CONTROL_PIC
// #define DTCR_TMP_PIC
#endif


#define USE_GM65       1
#define USE_UDP        2
#define USE_PIC        3

#define MY_MISSION_METHOD 2
#define PIC_BARCODE