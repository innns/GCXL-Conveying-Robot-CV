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
 * @version: 1.0
 * @Author: innns
 * @Email: ureinsecure@outlook.com
 * @Date: 2021-07-02 19:05:53
 * @LastEditors: innns
 * @LastEditTime: 2021-07-30 17:45:24
 * @FilePath: /junkcar/test/test_MT.cpp
*/
#include "Cam_MT.h"

int main(int argc, char *argv[])
{
    namedWindow("WIN");
    Cam_MT capture("/dev/camera1");
    cv::Mat frame;
    this_thread::sleep_for(std::chrono::milliseconds(5000));

    while (capture.isOpened())
    {
        if (!capture.read(frame))
        {
            cout << "EMPTY \n";
            continue;
        }
        // cout << "HERE \n";
        cv::imshow("WIN", frame);

        // cv::imwrite("image_L.jpg", frame);
        // cv::imshow("image R", frame1);
        if (waitKey(0) == 27)
        {
            break;
        }
    }
    capture.release();
    // capture1.release();
    return 0;
}
