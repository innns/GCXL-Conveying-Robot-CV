# junkcar

Author: innns

[简体中文版](./README.zh-CN.md)

![car](config/oc.jpg)

+ The vision code of GCXL conveying robot.
+ The filepath needs to be replaced in some places.
+ It is recommended to add your own ROI to the identified color area, paying attention to the image size and the conditions for judging the material
+ The coefficient of the distance deviation of the pixels from the center of the target from the distance moved by the robotic arm is calculated on our vehicle, and needs to be recalculated for different vehicles shooting at different angles (we have a camera that shoots the target diagonally downwards)
+ The serial code uses content from `2019 RoboMasterOS` written by `gezp`, GPL license.
---

## [Chinese docs](report/report.pdf)

using [UESTC_Lab_Report_LaTeX_Template](https://github.com/zshufan/UESTC_Lab_Report_LaTeX_Template)

## Environment

+ Raspberry Pi 4 *-raspios-buster-arm64  
+ OpenCV 4.5.2 + contrib （read barcode need contrib）

### Specific
[junkcar Env](config/environment/junkcar_Env.md)

### How to Build
```shell
# cd /path-to-CMakeLists/
mkdir build
cd build
cmake ..
make -j8
# main is the executable file
```

## Communication Protocol

### Data Frame Format

16 Byte data

| Head*1   | CMD*1    | DATA*12     | CHECK*1   | TAIL*1    |
| -------- | -------- | ----------- | --------- | --------- |
| [0] 0xAA | [1] CMD  | [2-13] DATA | [14] 0xF0 | [15] 0xBB |

Little-Endian

### Specific Protocol

```cpp
typedef enum : unsigned char
{
    // >>>>>> send
    CMD_CURRENT_COLOR = 0x01, // current color
    CMD_TARGET_POS = 0x02,    // target
    CMD_QR_MISSION = 0x03,    // mission from barcode
    CMD_ALL_COLOR = 0x04,     // colors
    CMD_ACK = 0x05,           // ack
    CMD_STORE_POS = 0x06,     // storage pos

    // >>>>>> recv
    CMD_SET_MODE = 0x11,      // set mode
    CMD_SWITCH_CAM = 0x12     // switch camera
} SerialPortCMD;

//>>>>>> Pi >>>>>
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

#### Pi to 32

##### Current Color

| Head*1   | CMD*1    | DATA*12          | CHECK*1   | TAIL*1    |
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

##### Target Pos

| Head*1   | CMD*1    | DATA*12                                                     | CHECK*1   | TAIL*1    |
| -------- | -------- | ----------------------------------------------------------- | --------- | --------- |
| [0] 0xAA | [1] 0x02 | [2-5] (int) pos_x ; [6-9] (int) pos_y ; [10-13] (int) pos_z | [14] 0xF0 | [15] 0xBB |

+ Left pos_x +
+ Forward pos_y +
+ Counterclockwise pos_z +

##### Mission Order

| Head*1   | CMD*1    | DATA*12                | CHECK*1   | TAIL*1    |
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
##### Scan All Colors

| Head*1   | CMD*1    | DATA*12                | CHECK*1   | TAIL*1    |
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

#### 32 to Pi

##### Set Mode

| Head*1   | CMD*1    | DATA*12                          | CHECK*1   | TAIL*1    |
| -------- | -------- | -------------------------------- | --------- | --------- |
| [0] 0xAA | [1] 0x11 | [2] DETECT_MODE ; [3] TARGET_POS | [14] 0xF0 | [15] 0xBB |

```cpp
enum DETECT_MODE : unsigned char
{
    DEFAULT_MODE = 0x00,    // Null
    COLOR_MODE = 0x01,      // read color
    TARGET_MODE = 0x02,     // find target
    QR_MODE = 0x03,         // read barcode
    ALL_COLOR_MODE = 0x04,  // detect all color
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


##### Switch Camera

| Head*1   | CMD*1    | DATA*12     | CHECK*1   | TAIL*1    |
| -------- | -------- | ----------- | --------- | --------- |
| [0] 0xAA | [1] 0x12 | [2] CAM_NUM | [14] 0xF0 | [15] 0xBB |

```cpp
enum CAM_NUM : unsigned char
{
    COLOR_CAM = 0x00,
    TARGET_CAM = 0x01
};
```
