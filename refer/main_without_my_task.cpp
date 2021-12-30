#include "Detector.h"

using namespace std;
using namespace cv;

void recv_message();

// cross_img show_img center center_color mode
RESULT_T result = {Mat(), Mat(), Point(), TARGET_MODE, 0, {NONE, NONE, NONE}};

// img save_dir cam_dead isQuiting
CAM_DATA usb_cam_data = {Mat(), SAVE_DIR, false, false};

int main()
{
    thread recv_thread(recv_message);
    //以下线程用于发送线程测试
    string serial_path_send = "/dev/ttyUSB0";
    robot_base::SerialInterface serial_interface;
    serial_interface.init(serial_path_send);

    USB_Cam cam;
    Detector detector;

    while (LIKELY(!usb_cam_data.cam_dead && !usb_cam_data.isQuiting)) // 相机没有掉线以及没有按下Q准备退出
    {
        usb_cam_data = cam.getData();          // 函数内会更新Data
        detector.solveImg(usb_cam_data.frame); // 检查靶子
        result = detector.getResult();
        char k = waitKey(20);
#ifdef CONTROL_PIC
        cam.reactKey(k);
#endif
    }

    recv_thread.join();
    return 0;
}

#pragma region Recv
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
#pragma endregion Recv