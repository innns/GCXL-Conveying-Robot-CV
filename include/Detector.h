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
 * @Descripttion: 相机参数640*480
 * @version: 2.0
 * @Author: innns
 * @Email: ureinsecure@outlook.com
 * @Date: 2021-02-03 14:39:54
 * @LastEditors: innns
 * @LastEditTime: 2021-09-17 18:14:02
 * @FilePath: /junkcar/include/Detector.h
*/

#ifndef DETECTOR_H
#define DETECTOR_H

#include "essential.h"

using namespace std;
using namespace cv;

#define AREA_THRES ((double)100.0)  // 颜色大于它才算检测到
#define AREA_THRES_S ((double)50.0) // 颜色大于它才算检测到

#define SHOW_WITH_PLUS

// enum COLOR: N R G B
enum COLOR : unsigned char
{
    NONE = 0x00,
    RED = 0x01,
    GREEN = 0x02,
    BLUE = 0x03
};

// enum COLOR: N R G B
enum TARGET_POS : unsigned char
{
    NONE_TAR = 0x00,
    LEFT_TAR = 0x01,
    CENTER_TAR = 0x02,
    RIGHT_TAR = 0x03
};

const string COLOR_LIST[4] = {"N",
                              "R",
                              "G",
                              "B"};

enum DETECT_MODE : unsigned char
{
    DEFAULT_MODE = 0x00,   // 空模式 0
    COLOR_MODE = 0x01,     // 读取颜色 1
    TARGET_MODE = 0x02,    // 寻找靶心 2
    QR_MODE = 0x03,        // 读取二维码 3
    ALL_COLOR_MODE = 0x04, // 读取所有颜色 4
    STORE_MODE = 0x06      // 放置码垛识别
};

/** 返回结果的结构体
 *  Point3i center;         靶心相对坐标
 *  DETECT_MODE mode;       检测模式
 *  COLOR mission_color[6]; 任务顺序
 *  COLOR now_color;        当前颜色
 *  COLOR all_color[6];     所有颜色
 *  Mat img;                图片
*/
typedef struct
{
    Point3i center;         // 靶心相对坐标
    DETECT_MODE mode;       // 检测模式
    COLOR mission_color[6]; // 任务顺序
    COLOR now_color;        // 当前颜色
    COLOR all_color[6];     // 所有颜色
    Mat img;                // 图片
} RESULT_T;

// Scarlar(H_min,S_min,V_min)
// Scarlar(H_max,S_max,V_max)
typedef struct
{
    cv::Scalar min;
    cv::Scalar max;
} HSV_T;

// CV_EXPORTS_W void HoughCircles( InputArray image, OutputArray circles,
//                                int method, double dp, double minDist,
//                                double param1 = 100, double param2 = 100,
//                                int minRadius = 0, int maxRadius = 0 );
typedef struct
{
    double dp;
    double minDist;
    double param1;
    double param2;
    int minRadius;
    int maxRadius;
} HC_T;

class Detector
{
private:
    Mat src_img, gray_img, binary_img, re_img;
    Mat H_element, V_element, C_element, M_element, M_element_s; //Horizon Vertical Center
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    int res_w = 800, res_h = 600;
    Point2i target_center;
    Point3i movement;
    RESULT_T result_T = {Point3i(), DEFAULT_MODE, {NONE}, NONE, {NONE}, Mat()};
    vector<Mat> bin_img_vec; // R G B
    HSV_T HSV_R = {Scalar(156, 80, 15), Scalar(180, 255, 255)};
    HSV_T HSV_R_ = {Scalar(0, 80, 15), Scalar(10, 255, 255)}; // 红色需要两段
    HSV_T HSV_G = {Scalar(45, 180, 15), Scalar(85, 255, 255)};
    HSV_T HSV_B = {Scalar(100, 80, 15), Scalar(124, 255, 255)};
    // HoughCircles 参数
    HC_T HC = {1., 10., 100., 120., 20, 200};
    // find target 参数
    vector<Point2f> SRC_PTS_C{Point2f(373., 136.),
                              Point2f(156., 309.),
                              Point2f(581., 317.),
                              Point2f(368., 533.)};
    vector<Point2f> SRC_PTS_L{Point2f(450., 144.),
                              Point2f(179., 228.),
                              Point2f(568., 397.),
                              Point2f(271., 509.)};
    vector<Point2f> SRC_PTS_R{Point2f(295., 161.),
                              Point2f(184., 408.),
                              Point2f(559., 246.),
                              Point2f(476., 517.)};
    vector<Point2f> DST_PTS{Point2f(300., 100.),
                            Point2f(100., 300.),
                            Point2f(500., 300.),
                            Point2f(300., 500.)};

    Mat PT_l, PT_reverse_l, PT_c, PT_reverse_c, PT_r, PT_reverse_r;      // 靶心 PerspectiveTransform


// TODO: TO TEST
    vector<Point2f> STORE_C{Point2f(373., 136.),
                            Point2f(156., 309.),
                            Point2f(581., 317.),
                            Point2f(368., 533.)};
    vector<Point2f> STORE_L{Point2f(450., 144.),
                            Point2f(179., 228.),
                            Point2f(568., 397.),
                            Point2f(271., 509.)};
    vector<Point2f> STORE_R{Point2f(295., 161.),
                            Point2f(184., 408.),
                            Point2f(559., 246.),
                            Point2f(476., 517.)};

    Mat S_PT_l, S_PT_c, S_PT_r;                                          // 库存区域 PerspectiveTransform

    Point CEN_L = Point(300, 300), CEN_C = Point(300, 300), CEN_R = Point(300, 300); // 目标的中心
    Point getPTPoint(Point src_point, Mat warpMatrix);
    Point3i calcTargetPos(Point cam_center, TARGET_POS tar = NONE_TAR);
    Point3i calcStorePos(Point cam_center, TARGET_POS tar = NONE_TAR);

    void solveTargetOld();
    void solveTarget(TARGET_POS tar = NONE_TAR);
    void solveColor();
    void preprocessImg(Mat img_, bool morph_process = true); // 默认会处理小噪声（一次扫六个用）
    void solveAllColorROI();
    void solveAllColorAuto();                           // 不用给出坐标测出六个色块的颜色
    vector<Point2f> findMaterial(Mat img, int num = 0); // 根据二值化图像判断是否是物块
    void correctAllColor();
    enum COLOR solveColor_(Mat img, double thres = AREA_THRES, bool morph_process = true);
    void solveStorePos(TARGET_POS tar);
    Mat R_bin_img, R_bin_img_, G_bin_img, B_bin_img, HSV_img;
    Mat getMorph(Mat bin_img_);
    double getMaxArea(Mat bin_img_);
    double B_max_area, G_max_area, R_max_area;
    enum COLOR max_color_cnt = NONE;
    enum COLOR max_color_cnt_all[6] = {NONE};
    void setMission(enum COLOR c[]);
    void getMatDest();
    void readGM65();
    void readQRCode();
    void readBarcode();
    void readUDP();
    void readStore();
    // Our constants:
    string IP = "localhost";
    uint16_t PORT = 8888;

public:
    Detector();
    ~Detector();
    void solveImg(Mat input_img, TARGET_POS tar = NONE_TAR);
    RESULT_T getResult();
    enum DETECT_MODE updateMode(enum DETECT_MODE mode_);
    void drawCross(CvArr *img, CvPoint point, CvScalar color, int size, int thickness);
    void showMission();

private:
#pragma region PIC_DIR
    std::string DATE_NOW, PIC_DIR;
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

    /*
    @name: setPicDir
    @msg: 内部调用，创建新的日期文件夹
    */
    void setPicDir()
    {
        DATE_NOW = getDate();
        PIC_DIR = SAVE_DIR + DATE_NOW;
    }
#pragma endregion
// #define STAR_WAR_FONT
// #ifdef STAR_WAR_FONT
#define FONT_ROWS 6
    string RGB_2PRINT[4][FONT_ROWS] =
        {
            {
                "        ",
                "   _    ",
                " _| |_  ",
                "|_   _| ",
                "  |_|   ",
                "        ",
            },
            {
                " __  ",
                "/_ | ",
                " | | ",
                " | | ",
                " | | ",
                " |_| ",
            },
            {
                " ___   ",
                "|__ \\  ",
                "   ) | ",
                "  / /  ",
                " / /_  ",
                "|____| ",
            },
            {
                " ____   ",
                "|___ \\  ",
                "  __) | ",
                " |__ <  ",
                " ___) | ",
                "|____/  ",
            }};
    // #endif
};

#endif
