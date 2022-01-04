# junkcar

Author: innns

![car](config/oc.jpg)

+ 工程训练大赛，物流小车，视觉方案
+ 某些地方需要更换路径。建议先浏览一遍代码x
+ 建议给识别颜色区域加上自己的ROI，注意图像尺寸以及判断物料的条件
+ 靶心像素距离偏离与机械臂移动距离的系数是在我们车上计算出的，不同车不同角度拍摄都需要重新计算（我们有一个斜向下拍靶子的摄像头）
+ 串口代码使用了`gezp`写的`2019 RoboMasterOS`中的内容，GPL协议。

---

## [详细文档](report\report.pdf)（其实是个课设报告改的）
使用了[UESTC_Lab_Report_LaTeX_Template](https://github.com/zshufan/UESTC_Lab_Report_LaTeX_Template)的模板
## 环境

+ Raspberry Pi 4 2021-03-04-raspios-buster-arm64 （具体版本无所谓）
+ OpenCV 4.5.2 + contrib （条形码需要contrib）

### 具体环境配置
[junkcar Env](config/environment/junkcar_Env.md)

### 编译
```shell
# cd到包含CMakeLists.txt的这个目录
mkdir build
cd build
cmake ..
make -j8
# main 文件就是可执行文件
```

## 通信协议

### 数据帧封装

16字节数据帧

| 帧头*1   | 命令位*1 | 数据位*12   | 校验位*1  | 帧尾*1    |
| -------- | -------- | ----------- | --------- | --------- |
| [0] 0xAA | [1] CMD  | [2-13] DATA | [14] 0xF0 | [15] 0xBB |

小端模式

### 协议定义

```cpp
typedef enum : unsigned char
{
    // >>>>>> 发送用
    CMD_CURRENT_COLOR = 0x01, // 当前检测到的颜色 条形码读取的颜色
    CMD_TARGET_POS = 0x02,    // 当前检测到的靶心
    CMD_QR_MISSION = 0x03,    // 二维码读取的任务
    CMD_ALL_COLOR = 0x04,     // 所有颜色读取
    CMD_ACK = 0x05,           // 确认 切换相机 切换靶心等
    CMD_STORE_POS = 0x06,     // 库存区域位置

    // >>>>>> 接收用
    CMD_SET_MODE = 0x11,      // 检测颜色/检测靶心
    CMD_SWITCH_CAM = 0x12     // 切换相机
} SerialPortCMD;

//>>>>>>树莓派端数据接口>>>>>
bool SerialInterface::current_color(char color_num)
{
    FixedPacket packet;
    packet.loadData(CMD_CURRENT_COLOR, 1);
    packet.loadData(color_num, 2);
    packet.pack();
    return sendPacket(packet) == 0;
}

bool SerialInterface::all_color(char m1, char m2, char m3, char m4, char m5, char m6)
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

bool SerialInterface::QR_mission(char m1, char m2, char m3, char m4, char m5, char m6)
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

bool SerialInterface::temp(int target_x, int target_y, int target_z)
{
    FixedPacket packet;
    packet.loadData(CMD_TEMP, 1);
    packet.loadData(target_x, 2);
    packet.loadData(target_y, 6);
    packet.loadData(target_z, 10);
    packet.pack();
    return sendPacket(packet) == 0;
}

```

#### Pi发给32

##### 当前颜色

| 帧头*1   | 命令位*1 | 数据位*1         | 校验位*1  | 帧尾*1    |
| -------- | -------- | ---------------- | --------- | --------- |
| [0] 0xAA | [1] 0x01 | [2] (char) COLOR | [14] 0xF0 | [15] 0xBB |

```cpp
enum COLOR : unsigned char
{
    NONE = 0x00,
    RED = 0x01,
    GREEN = 0x02,
    BLUE = 0x03
};
```

##### 靶心位置

| 帧头*1   | 命令位*1  | 数据位*12                                                    | 校验位*1   | 帧尾*1    |
| -------- | -------- | ----------------------------------------------------------- | --------- | --------- |
| [0] 0xAA | [1] 0x02 | [2-5] (int) pos_x ; [6-9] (int) pos_y ; [10-13] (int) pos_z | [14] 0xF0 | [15] 0xBB |

+ 左 pos_x 正
+ 前 pos_y 正
+ 逆 pos_z 正

##### 任务顺序

| 帧头*1   | 命令位*1  | 数据位*12               | 校验位*1   | 帧尾*1    |
| -------- | -------- | ---------------------- | --------- | --------- |
| [0] 0xAA | [1] 0x03 | [2-7] (char) COLOR * 6 | [14] 0xF0 | [15] 0xBB |

```cpp
enum COLOR : unsigned char
{
    NONE = 0x00,
    RED = 0x01,
    GREEN = 0x02,
    BLUE = 0x03
};
```
##### 一次扫完所有颜色

| 帧头*1   | 命令位*1  | 数据位*12               | 校验位*1   | 帧尾*1    |
| -------- | -------- | ---------------------- | --------- | --------- |
| [0] 0xAA | [1] 0x04 | [2-7] (char) COLOR * 6 | [14] 0xF0 | [15] 0xBB |

```cpp
enum COLOR : unsigned char
{
    NONE = 0x00,
    RED = 0x01,
    GREEN = 0x02,
    BLUE = 0x03
};
```

#### 32发给Pi

##### 设置 MODE

| 帧头*1   | 命令位*1  | 数据位*12                         | 校验位*1   | 帧尾*1    |
| -------- | -------- | -------------------------------- | --------- | --------- |
| [0] 0xAA | [1] 0x11 | [2] DETECT_MODE ; [3] TARGET_POS | [14] 0xF0 | [15] 0xBB |

```cpp
enum DETECT_MODE : unsigned char
{
    DEFAULT_MODE = 0x00,   // 空模式 0
    COLOR_MODE = 0x01,     // 读取颜色 1 国赛读取条形码
    TARGET_MODE = 0x02,    // 寻找靶心 2
    QR_MODE = 0x03,        // 读取二维码 3
    ALL_COLOR_MODE = 0x04,  // 读取所有颜色 4
    STORE_MODE = 0x06
};

// enum COLOR: N R G B
enum TARGET_POS : unsigned char
{
    NONE_TAR = 0x00,
    LEFT_TAR = 0x01,
    CENTER_TAR = 0x02,
    RIGHT_TAR = 0x03
};
```


##### 切换相机

| 帧头*1   | 命令位*1  | 数据位*12    | 校验位*1   | 帧尾*1    |
| -------- | -------- | ----------- | --------- | --------- |
| [0] 0xAA | [1] 0x12 | [2] CAM_NUM | [14] 0xF0 | [15] 0xBB |

```cpp
enum CAM_NUM : unsigned char
{
    COLOR_CAM = 0x00,
    TARGET_CAM = 0x01
};
```
