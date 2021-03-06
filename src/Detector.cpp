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
 * @Descripttion: 
 * @version: 1.0
 * @Author: innns
 * @Email: ureinsecure@outlook.com
 * @Date: 2021-09-17 17:25:26
 * @LastEditors: innns
 * @LastEditTime: 2021-09-18 06:57:06
 * @FilePath: /GCXL-Conveying-Robot-CV/src/Detector.cpp
*/
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
 * @Descripttion: 
 * @version: 2.0
 * @Author: innns
 * @Email: ureinsecure@outlook.com
 * @Date: 2021-02-03 14:39:54
 * @LastEditors: innns
 * @LastEditTime: 2021-09-17 18:19:57
 * @FilePath: /GCXL-Conveying-Robot-CV/src/Detector.cpp
*/

#include "Detector.h"

Detector::Detector()
{
    setPicDir();
    H_element = getStructuringElement(MORPH_CROSS, Size(50, 1));
    V_element = getStructuringElement(MORPH_CROSS, Size(1, 50));
    C_element = getStructuringElement(MORPH_CROSS, Size(15, 15));
    M_element = getStructuringElement(MORPH_ELLIPSE, Size(15, 15));
    M_element_s = getStructuringElement(MORPH_ELLIPSE, Size(10, 10));
    cv::FileStorage f(CONFIG_PATH, cv::FileStorage::READ);
    if (f.isOpened())
    {
        cout << "Using config\n";

        f["res_w"] >> res_w;
        f["res_h"] >> res_h;

        Mat t; //temp

        f["HSV_R"] >> t;
        HSV_R.min = Scalar(t.at<double>(0, 0), t.at<double>(0, 1), t.at<double>(0, 2));
        HSV_R.max = Scalar(t.at<double>(1, 0), t.at<double>(1, 1), t.at<double>(1, 2));
        f["HSV_R_"] >> t;
        HSV_R_.min = Scalar(t.at<double>(0, 0), t.at<double>(0, 1), t.at<double>(0, 2));
        HSV_R_.max = Scalar(t.at<double>(1, 0), t.at<double>(1, 1), t.at<double>(1, 2));

        f["HSV_G"] >> t;
        HSV_G.min = Scalar(t.at<double>(0, 0), t.at<double>(0, 1), t.at<double>(0, 2));
        HSV_G.max = Scalar(t.at<double>(1, 0), t.at<double>(1, 1), t.at<double>(1, 2));

        f["HSV_B"] >> t;
        HSV_B.min = Scalar(t.at<double>(0, 0), t.at<double>(0, 1), t.at<double>(0, 2));
        HSV_B.max = Scalar(t.at<double>(1, 0), t.at<double>(1, 1), t.at<double>(1, 2));

        f["HC"] >> t;
        HC.dp = t.at<double>(0);
        HC.minDist = t.at<double>(1);
        HC.param1 = t.at<double>(2);
        HC.param2 = t.at<double>(3);
        HC.minRadius = (int)t.at<double>(4);
        HC.maxRadius = (int)t.at<double>(5);

        f["SRC_PTS_L"] >> t;
        SRC_PTS_L[0] = Point2f(t.at<double>(0, 0), t.at<double>(0, 1));
        SRC_PTS_L[1] = Point2f(t.at<double>(1, 0), t.at<double>(1, 1));
        SRC_PTS_L[2] = Point2f(t.at<double>(2, 0), t.at<double>(2, 1));
        SRC_PTS_L[3] = Point2f(t.at<double>(3, 0), t.at<double>(3, 1));

        f["SRC_PTS_C"] >> t;
        SRC_PTS_C[0] = Point2f(t.at<double>(0, 0), t.at<double>(0, 1));
        SRC_PTS_C[1] = Point2f(t.at<double>(1, 0), t.at<double>(1, 1));
        SRC_PTS_C[2] = Point2f(t.at<double>(2, 0), t.at<double>(2, 1));
        SRC_PTS_C[3] = Point2f(t.at<double>(3, 0), t.at<double>(3, 1));

        f["SRC_PTS_R"] >> t;
        SRC_PTS_R[0] = Point2f(t.at<double>(0, 0), t.at<double>(0, 1));
        SRC_PTS_R[1] = Point2f(t.at<double>(1, 0), t.at<double>(1, 1));
        SRC_PTS_R[2] = Point2f(t.at<double>(2, 0), t.at<double>(2, 1));
        SRC_PTS_R[3] = Point2f(t.at<double>(3, 0), t.at<double>(3, 1));

        f["DST_PTS"] >> t;
        DST_PTS[0] = Point2f(t.at<double>(0, 0), t.at<double>(0, 1));
        DST_PTS[1] = Point2f(t.at<double>(1, 0), t.at<double>(1, 1));
        DST_PTS[2] = Point2f(t.at<double>(2, 0), t.at<double>(2, 1));
        DST_PTS[3] = Point2f(t.at<double>(3, 0), t.at<double>(3, 1));

        f["CEN_L"] >> t;
        CEN_L.x = t.at<double>(0);
        CEN_L.y = t.at<double>(1);

        f["CEN_C"] >> t;
        CEN_C.x = t.at<double>(0);
        CEN_C.y = t.at<double>(1);

        f["CEN_R"] >> t;
        CEN_R.x = t.at<double>(0);
        CEN_R.y = t.at<double>(1);

        f["STORE_L"] >> t;
        STORE_L[0] = Point2f(t.at<double>(0, 0), t.at<double>(0, 1));
        STORE_L[1] = Point2f(t.at<double>(1, 0), t.at<double>(1, 1));
        STORE_L[2] = Point2f(t.at<double>(2, 0), t.at<double>(2, 1));
        STORE_L[3] = Point2f(t.at<double>(3, 0), t.at<double>(3, 1));

        f["STORE_C"] >> t;
        STORE_C[0] = Point2f(t.at<double>(0, 0), t.at<double>(0, 1));
        STORE_C[1] = Point2f(t.at<double>(1, 0), t.at<double>(1, 1));
        STORE_C[2] = Point2f(t.at<double>(2, 0), t.at<double>(2, 1));
        STORE_C[3] = Point2f(t.at<double>(3, 0), t.at<double>(3, 1));

        f["STORE_R"] >> t;
        STORE_R[0] = Point2f(t.at<double>(0, 0), t.at<double>(0, 1));
        STORE_R[1] = Point2f(t.at<double>(1, 0), t.at<double>(1, 1));
        STORE_R[2] = Point2f(t.at<double>(2, 0), t.at<double>(2, 1));
        STORE_R[3] = Point2f(t.at<double>(3, 0), t.at<double>(3, 1));

        f["IP"] >> IP;
        f["PORT"] >> PORT;
    }
    else
    {
        cout << "Using default config\n";
    }
    f.release();
    // LEFT TARGET
    PT_l = getPerspectiveTransform(SRC_PTS_L, DST_PTS);
    PT_reverse_l = getPerspectiveTransform(DST_PTS, SRC_PTS_L);
    // CENTER TARGET
    PT_c = getPerspectiveTransform(SRC_PTS_C, DST_PTS);
    PT_reverse_c = getPerspectiveTransform(DST_PTS, SRC_PTS_C);
    // RIGHT TARGET
    PT_r = getPerspectiveTransform(SRC_PTS_R, DST_PTS);
    PT_reverse_r = getPerspectiveTransform(DST_PTS, SRC_PTS_R);

    // LEFT TARGET
    S_PT_l = getPerspectiveTransform(STORE_L, DST_PTS);
    // CENTER TARGET
    S_PT_c = getPerspectiveTransform(STORE_C, DST_PTS);
    // RIGHT TARGET
    S_PT_r = getPerspectiveTransform(STORE_R, DST_PTS);
}

Detector::~Detector()
{
}

/** ?????????????????????
 * @name: solveImg
 * @param {Mat} input_img
 * @param {TARGET_POS} tar ??????solveTaget???????????? tar
*/
void Detector::solveImg(Mat input_img, TARGET_POS tar)
{
    switch (result_T.mode)
    {
    case DEFAULT_MODE:
        usleep(10000); // sleep 10ms
        return;
    case COLOR_MODE:
#ifdef SINGLE_COLOR
        if (!input_img.empty())
        {
            resize(input_img, src_img, Size(800, 600));
            re_img = src_img.clone();
        }
        return solveColor();
#endif
#ifndef PIC_BARCODE
        return readStore();
#endif
    case TARGET_MODE:
        if (!input_img.empty())
        {
            resize(input_img, src_img, Size(800, 600));
            re_img = src_img.clone();
        }
        return solveTarget(tar);
    case QR_MODE:
        if (result_T.mission_color[0] + result_T.mission_color[1] + result_T.mission_color[2] == 6) //??????????????????????????????????????????
        {
            // cout << "inited mission\n";
            return showMission();
        }
#if MY_MISSION_METHOD == USE_PIC
        if (!input_img.empty())
        {
            resize(input_img, src_img, Size(800, 600));
            re_img = src_img.clone();
        }
        return readQRCode();
#elif MY_MISSION_METHOD == USE_UDP
        return readUDP();
#else
        // MY_MISSION_METHOD == USE_GM65
        return readGM65();
#endif
    case ALL_COLOR_MODE:
        if (!input_img.empty())
        {
            resize(input_img, src_img, Size(800, 600));
            re_img = src_img.clone();
        }
        cout << "solve all\n";
        // return solveAllColorROI();
        return solveAllColorAuto();
    case STORE_MODE:
        if (!input_img.empty())
        {
            resize(input_img, src_img, Size(800, 600));
            re_img = src_img.clone();
        }
        return solveStorePos(tar);
    default:
        return;
    }
}

#pragma region OLD
/** ????????????????????????????????????????????? 
 * @name: solveTargetOld
*/
void Detector::solveTargetOld()
{
    target_center = Point(0, 0);
    Mat mask = Mat::zeros(src_img.size(), CV_8UC1);
    cvtColor(src_img, gray_img, CV_RGB2GRAY);
    threshold(gray_img, binary_img, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
    Mat H_img, V_img;
    morphologyEx(binary_img, H_img, MORPH_OPEN, H_element);
    morphologyEx(binary_img, V_img, MORPH_OPEN, V_element);
    findContours(H_img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    int max_width = -1;
    int index = 0;
    Rect temp_rect;
    for (int i = 0; i < contours.size(); i++)
    {
        temp_rect = boundingRect(contours[i]);
        if (max_width < temp_rect.width)
        {
            max_width = temp_rect.width;
            index = i;
            // cout << "w: " << index << endl;
        }
    }
    drawContours(mask, contours, index, (255), -1, 8); //?
#ifdef DTCR_TMP_PIC
    cv::imwrite("V_line", V_img);
#endif
    findContours(V_img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    int max_height = -1;
    index = 0;
    for (int i = 0; i < contours.size(); i++)
    {
        temp_rect = boundingRect(contours[i]);
        if (max_height < temp_rect.height && temp_rect.x < gray_img.cols * 0.75)
        {
            max_height = temp_rect.height;
            index = i;
        }
    }
    drawContours(mask, contours, index, (255), -1, 8); //?
#ifdef DTCR_TMP_PIC
    cv::imwrite("mask.jpg", mask);
#endif
    Mat cross_mask;
    morphologyEx(mask, cross_mask, MORPH_OPEN, C_element);
    // int ele = 5;
    // createTrackbar("ele", "cross_mask", &ele, 50);
    // Mat ele_5_5 = getStructuringElement(MORPH_CROSS, Size(ele, ele));
    // morphologyEx(cross_mask, cross_mask, MORPH_OPEN, ele_5_5);
#ifdef DTCR_TMP_PIC
    cv::imwrite("cross_mask", cross_mask);
#endif
    findContours(cross_mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    for (int i = 0; i < contours.size(); i++)
    {
        temp_rect = boundingRect(contours[i]);
        if (temp_rect.width > 30 || temp_rect.height > 30)
        {
            cout << "Size :" << temp_rect.width << " " << temp_rect.height << endl;
            continue;
        }
        cout << temp_rect << endl;
        target_center.x = (temp_rect.x + temp_rect.width / 2);
        target_center.y = (temp_rect.y + temp_rect.height / 2);
        circle(src_img, Point2i(320, 240), 2, (255, 0, 0), 4, 8, 0);
        circle(src_img, target_center, 4, (0, 255, 0), 2, 8, 0);
        // #ifdef __SAVE_BIN__
        // cv::imwrite("center.jpg",img);
        // #endif
    }
}
#pragma endregion OLD

/** ??????????????????????????????????????????
 * @name: solveTarget
 * @param {TARGET_POS} tar???????????????????????????????????? ????????????????????????
 * @return {*}
*/
void Detector::solveTarget(TARGET_POS tar)
{
    Mat PT, PT_reverse;
    // Point CEN=Point(300,300);
    switch (tar)
    {
    case NONE_TAR:
        return;
    case LEFT_TAR:
    {
        PT = PT_l;
        PT_reverse = PT_reverse_l;
    }
    break;
    case CENTER_TAR:
    {
        PT = PT_c;
        PT_reverse = PT_reverse_c;
    }
    break;
    case RIGHT_TAR:
    {
        PT = PT_r;
        PT_reverse = PT_reverse_r;
    }
    break;
    default:
        return;
    }
    target_center = Point(300, 300);
    vector<Vec3f> circles; // ???????????????????????? vecctor<x,y,r>
    // preprocessImg(src_img); // ??????????????????????????????
    // ????????????
    warpPerspective(src_img, re_img, PT, Size2i(600, 600));

    //     // ????????? ???????????????????????????????????????????????????????????????????????????
    //     int target_color = solveColor_(re_img, 20, false);
    //     cout << "Target is : " << COLOR_LIST[target_color] << "\n";
    //     target_color -= 1;
    //     target_color = AMP_LIMIT(target_color, 2, 0); //012
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));

    // #ifdef _SAVE_TEMP_
    //     imwrite("re.jpg", re_img);
    //     imwrite("temp0.jpg", bin_img_vec[target_color]);
    // #endif
    //     morphologyEx(bin_img_vec[target_color], bin_img_vec[target_color], MORPH_DILATE, element, Point(-1, -1));
    //     morphologyEx(bin_img_vec[target_color], bin_img_vec[target_color], MORPH_ERODE, element, Point(-1, -1));
    // #ifdef _SAVE_TEMP_
    //     imwrite("temp_morph.jpg", bin_img_vec[target_color]);
    // #endif
    //     HoughCircles(bin_img_vec[target_color], circles, HOUGH_GRADIENT, HC.dp, HC.minDist, HC.param1, HC.param2, HC.minRadius, HC.maxRadius);

    morphologyEx(re_img, re_img, MORPH_ERODE, element, Point(-1, -1));
    morphologyEx(re_img, re_img, MORPH_DILATE, element, Point(-1, -1));
    cvtColor(re_img, re_img, CV_BGR2GRAY);
    HoughCircles(re_img, circles, HOUGH_GRADIENT, HC.dp, HC.minDist, HC.param1, HC.param2, HC.minRadius, HC.maxRadius);

    if (circles.size())
    {
        float x = 0, y = 0;
        cout << "circle num : " << circles.size() << "\n";
        for (size_t i = 0; i < circles.size(); i++)
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            x += center.x;
            y += center.y;
            circle(re_img, center, 2, CV_RGB(255, 0, 0), 1, 8, 0);
            circle(re_img, center, cvRound(circles[i][2]), CV_RGB(0, 255, 255), 1, 8, 0);
        }
        x /= circles.size();
        y /= circles.size();
        circle(re_img, Point(x, y), 3, CV_RGB(0, 255, 0), 2, 8, 0);
#ifdef RETURN_CENTER_WITH_PT
        target_center = getPTPoint(Point(x, y), PT_reverse);
#else
        target_center = Point(x, y);
#endif
        movement = calcTargetPos(Point(x, y), tar);
    }
    else
    {
        cout << "NO CIRCLES\n";
        target_center = Point(0, 0);
        movement = Point3i(0, 0, 0);
    }
}

/** ??????????????? ????????????????????????
 * @name: 
 * @param {Mat} img
 * @return {*}
*/
void Detector::preprocessImg(Mat img_, bool morph_process)
{
    // Mat img_ = img.clone();
    vector<Mat> hsvSplit;                   //???????????????????????????HSV??????????????????
    cvtColor(img_, HSV_img, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
    // >>default
    // split(HSV_img, hsvSplit);               //??????????????????HSV?????????
    // equalizeHist(hsvSplit[2], hsvSplit[2]); //???HSV????????????????????????????????????
    // merge(hsvSplit, HSV_img);               //??????????????????
    // <<default
    // >>> RED
    inRange(HSV_img, HSV_R.min, HSV_R.max, R_bin_img);    //??????1
    inRange(HSV_img, HSV_R_.min, HSV_R_.max, R_bin_img_); //??????2
    cv::bitwise_or(R_bin_img, R_bin_img_, R_bin_img);

    // >>> GREEN
    inRange(HSV_img, HSV_G.min, HSV_G.max, G_bin_img); //??????

    // >>> BLUE
    inRange(HSV_img, HSV_B.min, HSV_B.max, B_bin_img); //??????

    if (morph_process)
    {
        R_bin_img = getMorph(R_bin_img);
        G_bin_img = getMorph(G_bin_img);
        B_bin_img = getMorph(B_bin_img);
    }

#ifdef DTCR_TMP_PIC
    cv::imshow("B", B_bin_img);
    cv::imshow("G", G_bin_img);
    cv::imshow("R", R_bin_img);
#endif
    // ??????????????? R G B
    bin_img_vec = {R_bin_img, G_bin_img, B_bin_img};
}

/**
 * @name: solveColor()
 * @msg: ????????????????????????????????????????????????????????????R/G/B
 * @return {max_area}
*/
void Detector::solveColor()
{
    Mat ROI_img = src_img(Rect(120, 40, 400, 400)); //640 * 480 ??????????????????
    solveColor_(ROI_img);
}

/**
 * @name: solveColor()
 * @msg: ????????????????????????????????????????????????????????????R/G/B
 * @return {max_color_cnt}
*/
enum COLOR Detector::solveColor_(Mat img, double thres, bool morph_process)
{
    preprocessImg(img, morph_process);
    R_max_area = getMaxArea(R_bin_img);
    G_max_area = getMaxArea(G_bin_img);
    B_max_area = getMaxArea(B_bin_img);
    vector<double> area_vec = {thres, R_max_area, G_max_area, B_max_area};          //???????????????RGB 123
    int max_cnt = max_element(area_vec.begin(), area_vec.end()) - area_vec.begin(); //???????????????
    max_color_cnt = (enum COLOR)(max_cnt);                                          //RGB123
#ifdef DTCR_TMP_PIC
    cout << "MAX" << COLOR_LIST[max_cnt] << endl;
    cout << "\nBA " << B_max_area << "  GA " << G_max_area << "  RA " << R_max_area << endl;
#endif
    return max_color_cnt;
}

/** ????????????????????????????????????????????????????????????R/G/B
 * @name: solveAllColor()
 * @msg: ????????????????????????????????????????????????????????????R/G/B
 * @return {max_area}
*/
void Detector::solveAllColorROI()
{
    Mat ROI_img[6];
    // TODO: ROI????????????
    ROI_img[0] = src_img(Rect(168, 168, 200, 200));
    ROI_img[1] = src_img(Rect(380, 168, 200, 200));
    ROI_img[2] = src_img(Rect(600, 168, 200, 200));
    ROI_img[3] = src_img(Rect(168, 400, 200, 200));
    ROI_img[4] = src_img(Rect(380, 400, 200, 200));
    ROI_img[5] = src_img(Rect(300, 400, 200, 200));
    for (int i = 0; i < 6; i++)
    {
        max_color_cnt_all[i] = solveColor_(ROI_img[i], AREA_THRES_S);
    }
    cout << "UN CRT" << endl;
    cout << max_color_cnt_all[0] << max_color_cnt_all[1] << max_color_cnt_all[2] << "+"
         << max_color_cnt_all[3] << max_color_cnt_all[4] << max_color_cnt_all[5] << endl;
    // ????????????????????????????????????
    correctAllColor();
}

/** ???????????????????????????????????????????????????ROI???
 * @name: solveAllColorAuto
 * @param {Mat} img
 * @param {double} thres
*/
void Detector::solveAllColorAuto()
{
    preprocessImg(src_img);
#ifdef _SAVE_TEMP_
    Mat HSV_save;
    cvtColor(HSV_img, HSV_save, CV_HSV2BGR);
    cv::imwrite(PIC_DIR + string("hsv.jpg"), HSV_save);
    cv::imwrite(PIC_DIR + string("bin_b.jpg"), B_bin_img);
    cv::imwrite(PIC_DIR + string("bin_g.jpg"), G_bin_img);
    cv::imwrite(PIC_DIR + string("bin_r.jpg"), R_bin_img);
#endif
    map<float, COLOR> material_color_map; // ?????????x????????????????????????????????????x??????+=1000
    material_color_map.clear();
    for (int i = 0; i < 3; i++)
    {
        cout << "Now is " << COLOR_LIST[i + 1] << endl;
        vector<Point2f> material_vector(findMaterial(bin_img_vec.at(i), i)); // ?????????
        if (material_vector.at(0).x != 0 && material_vector.at(1).x != 0)    //????????????????????????
        {
            float upper, lowwer;
            if (material_vector.at(0).y < material_vector.at(1).y) // ??????0??????y?????????????????????????????????????????????????????????
            {
                upper = material_vector.at(0).x;
                lowwer = material_vector.at(1).x + 2000;
            }
            else
            {
                upper = material_vector.at(1).x;
                lowwer = material_vector.at(0).x + 2000;
            }
            material_color_map.insert(make_pair(upper, COLOR(i + 1)));
            material_color_map.insert(make_pair(lowwer, COLOR(i + 1)));
            cout << "up " << upper << " low " << lowwer << endl;
        }
        else
        {
            // ????????????????????????????????????
            cout << "WRONG auto, use ROI\n";
            return solveAllColorROI();
        }
    }
    int cnt = 0;
    int last_x = 0;
    for (auto &i : material_color_map)
    {
        if (last_x != 0 && (i.first - last_x) < 80) // ????????????80????????????
        {
            cout << "WRONG dis<80, use ROI\n";
            return solveAllColorROI();
        }
        last_x = i.second;
        max_color_cnt_all[cnt] = i.second;
        cnt++;
    }
}

/** ??????????????????????????????????????????vector???????????????Point2f
 * @name: findMaterial(Mat img)
 * @param {Mat} img
 * @param {int} num
 * @return {vector<Point2f>}material_vec
*/
vector<Point2f> Detector::findMaterial(Mat img, int num)
{
    // ????????????????????? R G B
    vector<Point2f> material_vec;
    multimap<float, Point2f, std::greater<float>> m_area_center_map;
    material_vec.clear();
    contours.clear();
    hierarchy.clear();
    findContours(img, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    cv::RotatedRect temp;
    // note: ?????????????????????
    if (contours.size() < 40)
    {
#ifdef _SAVE_TEMP_
        Mat drawCs = src_img.clone();
        Mat drawRR = src_img.clone();
#endif
        for (int i = 0; i < contours.size(); i++)
        {
            if (contours[i].size() > 120)
            {
                temp = cv::fitEllipseDirect(contours[i]);
#ifdef _SAVE_TEMP_
                ellipse(drawRR, temp, CV_RGB(0, 255, 255));
#endif
                if (temp.size.height<(res_h / 12) | temp.size.height>(res_h / 3.5))
                {
                    cout << "[W] H: " << temp.size.height << endl;
                    continue;
                }
                if (temp.size.width<(res_w / 25) | temp.size.width>(res_w / 4))
                {
                    cout << "[W] W: " << temp.size.width << endl;
                    continue;
                }
                if (temp.size.height / temp.size.width > 3 | temp.size.height / temp.size.width < 1)
                {
                    cout << "[W] Ration: " << temp.size.height / temp.size.width << endl;
                    continue;
                }
                // if (temp.angle >= 90)
                //     temp.angle = temp.angle - 180;
                // if (abs(temp.angle) > 15)
                // {
                //     cout << "angle " << abs(temp.angle) << endl;
                //     continue;
                // }
                // if (temp.size.area() < (float)AREA_THRES)
                //     continue;
                // cout << "size  " << contours[i].size() << endl;
#ifdef _SAVE_TEMP_

                drawContours(drawCs, contours, i, CV_RGB(0, 255, 255));
#endif
                // if (temp.size.area() > 1000)
                //     continue;
                // material_vec.push_back(temp.center);
                m_area_center_map.insert(make_pair(temp.size.area(), temp.center));
            }
        }
#ifdef _SAVE_TEMP_
        num = AMP_LIMIT(num, 2, 0);
        imwrite(PIC_DIR + string("draw_RR_") + COLOR_LIST[num + 1] + ".jpg", drawRR);
        imwrite(PIC_DIR + string("draw_Cs_") + COLOR_LIST[num + 1] + ".jpg", drawCs);
#endif
        cout << "m_area_center_map size " << m_area_center_map.size() << endl;

        while (m_area_center_map.size() < 2)
        {
            // cout <<"m_area_center_map.size "<<m_area_center_map.size()<<endl;
            m_area_center_map.insert(make_pair(0.0f, Point2f(0, 0)));
            // m_area_center_map.insert(make_pair(0.0f, Point2f(0, 0)));
        }
        cout << "now size " << m_area_center_map.size() << endl;

        for (auto it = m_area_center_map.begin(); it != m_area_center_map.end(); it++)
        {
            material_vec.push_back(it->second);
            if (material_vec.size() == 2)
                break;
        }
        if (UNLIKELY(material_vec.size() < 2))
        {
            cout << "material_vec.size() < 2" << endl;
            // material_vec.push_back(it->second);
        }
    }
    return material_vec;
}

/** ???????????? ???????????????????????????
 * @brief: ?????????????????????max_color_cnt_all
 * @name: correctAllColor()
 * @param {NULL}
 * @return {NULL}
*/
void Detector::correctAllColor()
{
    int ALL_RIGHT_CO_LIST[6][3] = {
        {1, 2, 3},
        {1, 3, 2},
        {2, 1, 3},
        {2, 3, 1},
        {3, 1, 2},
        {3, 2, 1}};

    int up_max = 0;   // ?????????????????????????????? up_max <= 3
    int up_index = 0; // ??????????????????????????????

    int low_max = 0;   // ?????????????????????????????? up_max <= 3
    int low_index = 0; // ??????????????????????????????
    // ??????????????????????????????
    for (int i = 0; i < 6; i++)
    {
        int up_cnt = 0;
        int low_cnt = 0;
        // up (012)
        for (int j = 0; j < 3; j++)
        {
            if (max_color_cnt_all[j] == (COLOR)ALL_RIGHT_CO_LIST[i][j])
            {
                up_cnt++;
            }
        }
        // low (345)
        for (int j = 3; j < 6; j++)
        {
            if (max_color_cnt_all[j] == (COLOR)ALL_RIGHT_CO_LIST[i][j - 3])
            {
                low_cnt++;
            }
        }
        // ?????????????????????
        if (up_max < up_cnt)
        {
            up_max = up_cnt;
            up_index = i;
        }
        if (low_max < low_cnt)
        {
            low_max = low_cnt;
            low_index = i;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        max_color_cnt_all[i] = (COLOR)ALL_RIGHT_CO_LIST[up_index][i];
    }
    for (int i = 3; i < 6; i++)
    {
        max_color_cnt_all[i] = (COLOR)ALL_RIGHT_CO_LIST[low_index][i - 3];
    }
}

/** ???morphologyEx????????????
 * @name: getMorph
 * @param {Mat} bin_img_
 * @return {Mat} img
*/
Mat Detector::getMorph(Mat bin_img_)
{
    Mat temp_img = bin_img_.clone();
#ifdef DTCR_TMP_PIC
    imshow("temp", bin_img_);
    // cv::waitKey(0);
#endif
    morphologyEx(temp_img, temp_img, MORPH_OPEN, M_element_s);
    morphologyEx(temp_img, temp_img, MORPH_CLOSE, M_element);
#ifdef DTCR_TMP_PIC
    // imshow("Morph_img", temp_img);
    // waitKey();
#endif
    return temp_img;
}

/** 
 * @name: getMaxArea()
 * @msg: ???????????????????????????????????????
 * @param {Mat} bin_img_???????????????
 * @return {double} max_area??????????????????
*/
double Detector::getMaxArea(Mat bin_img_)
{
    cv::findContours(bin_img_, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    // ?????????????????????
    double max_area = 0;
    // vector<cv::Point> max_contour;
    for (size_t i = 0; i < contours.size(); i++)
    {
        double area = cv::contourArea(contours[i]);
        if (area > max_area)
        {
            max_area = area;
            // max_contour = contours[i];
        }
    }
    return max_area;
}

RESULT_T Detector::getResult()
{
    showMission();
    result_T.center = movement;
    result_T.now_color = max_color_cnt;
    memcpy(result_T.all_color, max_color_cnt_all, 6 * sizeof(enum COLOR));
    result_T.img = re_img;
    return result_T;
}

enum DETECT_MODE Detector::updateMode(enum DETECT_MODE mode_)
{
    result_T.mode = mode_;
    return mode_;
};

void Detector::setMission(enum COLOR c[])
{
    for (int i = 0; i < 6; i++)
    {
        result_T.mission_color[i] = c[i];
    }
}

void Detector::readGM65()
{
    enum COLOR t[6];
    while (1)
    {
        scanf("%1d%1d%1d+%1d%1d%1d", &t[0], &t[1], &t[2], &t[3], &t[4], &t[5]);
        if (t[0] + t[1] + t[2] + t[3] + t[4] + t[5] == 12)
            break;
    }
    setMission(t);
    showMission();
}

void Detector::readQRCode()
{
    Mat ROI_img;
    cvtColor(src_img, ROI_img, CV_BGR2GRAY); // ?????????????????????
    vector<Point> points;
    QRCodeDetector QRCD;
    bool isQRcode = QRCD.detect(ROI_img, points); // ?????????????????????
    if (isQRcode)
    {
        string info = QRCD.decode(ROI_img, points); // ???????????????
        cout << info << endl;
        if ((int)info.size() == 7 && info[3] == '+')
        {
            enum COLOR t[6];
            for (size_t i = 0; i < 3; i++)
            {
                t[i] = COLOR(info[i] - '0');
            }
            for (size_t i = 4; i < 7; i++)
            {
                t[i - 1] = COLOR(info[i] - '0');
            }
            if (t[0] + t[1] + t[2] + t[3] + t[4] + t[5] == 12)
            {
                setMission(t); // ????????? ??????????????????
            }
            else
            {
                cout << "check sum != 12\n";
            }
        }
    }
    else
    {
        cout << "no QR" << endl;
        return;
    }
    showMission();
}

// TODO: FIX!
void Detector::readUDP()
{
    // Initialize server socket..
    UDPServer udpServer;
    bool is_inited = false;

    // onMessageReceived will run when a message received with information of ip & port of sender:
    udpServer.onMessageReceived = [&](string info, string ipv4, uint16_t port)
    {
        cout << ipv4 << ":" << port << " => " << info << "(" << (int)info.size() << ")" << endl;
        if ((int)info.size() == 7 && info[3] == '+')
        {
            enum COLOR t[6];
            for (size_t i = 0; i < 3; i++)
            {
                t[i] = COLOR(info[i] - '0');
            }
            for (size_t i = 4; i < 7; i++)
            {
                t[i - 1] = COLOR(info[i] - '0');
            }
            if (t[0] + t[1] + t[2] + t[3] + t[4] + t[5] == 12)
            {
                setMission(t); // ????????? ??????????????????
                is_inited = true;
                udpServer.Close();
                return showMission();
            }
            else
            {
                cout << "check sum != 12\n";
            }
        }
    };

    // Bind the server to a port.
    udpServer.Bind(PORT, [](int errorCode, string errorMessage)
                   { cout << errorCode << " : " << errorMessage << endl; });
    int count = 0;
    while (!is_inited)
    {
        count++;
        if (count > 5000)
        {
            cout << "Not recv \n";
        }
        usleep(10000);
    }

    udpServer.Close();
    showMission();
}

void Detector::readBarcode()
{
    return;
}

// TODO: readStore
/** readStore with GM65
 * @name: readStore()
*/
void Detector::readStore()
{
    int i = 0;
    scanf("%1d", &i);
    if (i == AMP_LIMIT(i, 3, 1))
        max_color_cnt = COLOR(i);
}

/** ????????????
 * @param[in] img ????????????
 * @param[in] point ???????????????
 * @param[in] color ??????
 * @param[in] size ????????????
 * @param[in] thickness ??????
*/
void Detector::drawCross(CvArr *img, CvPoint point, CvScalar color, int size, int thickness)
{
    //????????????
    cvLine(img, cvPoint(point.x - size / 2, point.y), cvPoint(point.x + size / 2, point.y), color, thickness, 8, 0);
    //????????????
    cvLine(img, cvPoint(point.x, point.y - size / 2), cvPoint(point.x, point.y + size / 2), color, thickness, 8, 0);
    return;
}

/** ????????????????????????????????????
 * @name: getPTPoint
 * @param {Point} src_point
 * @param {Mat} warpMatrix
 * @return {Point} result
*/
Point Detector::getPTPoint(Point src_point, Mat warpMatrix)
{
    Mat_<double> mat_pt(3, 1);
    mat_pt(0, 0) = src_point.x;
    mat_pt(1, 0) = src_point.y;
    mat_pt(2, 0) = 1;
    Mat mat_pt_view = warpMatrix * mat_pt;
    double a1 = mat_pt_view.at<double>(0, 0);
    double a2 = mat_pt_view.at<double>(1, 0);
    double a3 = mat_pt_view.at<double>(2, 0);
    return Point(a1 * 1.0 / a3, a2 * 1.0 / a3);
}

void Detector::showMission()
{
    if (!result_T.mode)
    {
        return; // Default mode
    }
    if (UNLIKELY(result_T.mission_color[0] + result_T.mission_color[1] + result_T.mission_color[2] != 6))
    {
        return;
    }
    string s = "";
    for (size_t row = 0; row < FONT_ROWS; row++) // 6???
    {
#ifdef SHOW_WITH_PLUS
        for (size_t m = 0; m < 3; m++) // ???3?????????
        {
            s += RGB_2PRINT[result_T.mission_color[m]][row];
        }
        s += RGB_2PRINT[0][row];       // ??????
        for (size_t m = 3; m < 6; m++) // ???3?????????
        {
            s += RGB_2PRINT[result_T.mission_color[m]][row];
        }
#else
        for (size_t m = 0; m < 6; m++) // 6?????????
        {
            // s += RGB_2PRINT[??????][??????]
            s += RGB_2PRINT[result_T.mission_color[m]][row];
        }
#endif
        s += "\n";
    }
    cout << s;
    // result_T.mission_color[3] result_T.mission_color[4] result_T.mission_color[5]
}

Point3i Detector::calcTargetPos(Point cam_center, TARGET_POS tar)
{
    //TODO: calcTargetPos
    Point3i re = Point3i(0, 0, 0);

    switch (tar)
    {
    case NONE_TAR:
        break;
    case LEFT_TAR:
    {
        re.x = int(round(double(CEN_L.x - cam_center.x) / 4.5));
        re.y = int(round(double(CEN_L.y - cam_center.y) / 4.5));
        re.z = 0;
    }
    break;
    case CENTER_TAR:
    {
        re.x = int(round(double(CEN_C.x - cam_center.x) / 4.5));
        re.y = int(round(double(CEN_C.y - cam_center.y) / 4.5));
        re.z = 0;
    }
    break;
    case RIGHT_TAR:
    {
        re.x = int(round(double(CEN_R.x - cam_center.x) / 4.5));
        re.y = int(round(double(CEN_R.y - cam_center.y) / 4.5));
        re.z = 0;
    }
    break;
    default:
        break;
    }
    printf("movement:(%d, %d, %d)\n", re.x, re.y, re.z);
    return re;
}

Point3i Detector::calcStorePos(Point cam_center, TARGET_POS tar)
{
    //TODO: calcStorePos
    Point3i re = Point3i(0, 0, 0);

    switch (tar)
    {
    case NONE_TAR:
        break;
    case LEFT_TAR:
    {
        re.x = int(round(double(CEN_L.x - cam_center.x) / 2));
        re.y = int(round(double(CEN_L.y - cam_center.y) / 2));
        re.z = 0;
    }
    break;
    case CENTER_TAR:
    {
        re.x = int(round(double(CEN_C.x - cam_center.x) / 2));
        re.y = int(round(double(CEN_C.y - cam_center.y) / 2));
        re.z = 0;
    }
    break;
    case RIGHT_TAR:
    {
        re.x = int(round(double(CEN_R.x - cam_center.x) / 2));
        re.y = int(round(double(CEN_R.y - cam_center.y) / 2));
        re.z = 0;
    }
    break;
    default:
        break;
    }
    printf("movement:(%d, %d, %d)\n", re.x, re.y, re.z);
    return re;
}

void Detector::solveStorePos(TARGET_POS tar)
{
    Mat PT, PT_reverse;
    // Point CEN=Point(300,300);
    switch (tar)
    {
    case NONE_TAR:
        return;
    case LEFT_TAR:
    {
        PT = S_PT_l;
    }
    break;
    case CENTER_TAR:
    {
        PT = S_PT_c;
    }
    break;
    case RIGHT_TAR:
    {
        PT = S_PT_r;
    }
    break;
    default:
        return;
    }
    target_center = Point(300, 300);
    vector<Vec3f> circles;  // ???????????????????????? vecctor<x,y,r>
    preprocessImg(src_img); // ??????????????????????????????
    // ????????????
    warpPerspective(src_img, re_img, PT, Size2i(600, 600));

    //     // ????????? ???????????????????????????????????????????????????????????????????????????
    int target_color = solveColor_(re_img, 20, false);
    cout << "Target is : " << COLOR_LIST[target_color] << "\n";
    target_color -= 1;
    target_color = AMP_LIMIT(target_color, 2, 0); // 012
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));

    morphologyEx(bin_img_vec[target_color], re_img, MORPH_DILATE, element, Point(-1, -1));
    morphologyEx(re_img, re_img, MORPH_ERODE, element, Point(-1, -1));
    contours.clear();
    hierarchy.clear();
    findContours(re_img, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    cv::RotatedRect temp;
    map<int, int> store_map; // ??????????????? size, color

    if (contours.size() < 40)
    {
        for (int i = 0; i < contours.size(); i++)
        {
            if (contours[i].size() > 120)
            {
                temp = cv::fitEllipseDirect(contours[i]);
                if (temp.size.height / temp.size.width > 1.5 | temp.size.height / temp.size.width < 0.67)
                {
                    cout << "[Warning] Ration: " << temp.size.height / temp.size.width << endl;
                    continue;
                }
            }
            store_map.insert(make_pair(contours[i].size(), i));
        }

        auto it = store_map.begin();
        temp = cv::fitEllipseDirect(contours[it->second]);
        movement = calcStorePos(temp.center, tar);
    }
    else
    {
        movement = Point3i(0, 0, 0);
    }
}