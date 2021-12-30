<!--
 * @Descripttion: 
 * @version: 1.0
 * @Author: innns
 * @Email: ureinsecure@outlook.com
 * @Date: 2021-05-02 12:46:23
 * @LastEditors: innns
 * @LastEditTime: 2021-09-16 14:15:34
 * @FilePath: /GCXL-Conveying-Robot-CV/config/environment/junkcar_Env.md
-->
# junkcar Env

Author: Zhang

---

## 主机

### [VS Code](https://code.visualstudio.com/)

#### 插件

+ **ms-vscode.cpptools-extension-pack**
+ aaron-bond.better-comments
+ oderwat.indent-rainbow
+ coenraads.bracket-pair-colorizer
+ obkoro1.korofileheader
+ yzhang.markdown-all-in-one
+ shd101wyy.markdown-preview-enhanced
+ christian-kohler.path-intellisense
+ ms-python.python
+ gruntfuggly.todo-tree
+ visualstudioexptteam.vscodeintellicode
+ unbug.codelf
+ donjayamanne.githistory
+ eamodio.gitlens
+ github.vscode-pull-request-github

## 设备

Raspberry Pi 4 

## OS

+ 2021-03-04-raspios-buster-arm64

### 烧录

+ [Raspberry Pi Imager](https://www.raspberrypi.org/%20downloads/#:~:text=Raspberry%20Pi%20Imager%20provides%20an%20easy%20way%20to,completely%20erased%20of%20all%20data%20during%20this%20process.)
+ 添加`/boot/ssh`文件

## 配置环境

[一键配置脚本](src/one_punch.sh)
```shell
# 一键配置OpenCV环境
# 需要大概8h的时间完成
# 编译需要很久！
./one_punch.sh
```
或者可以根据下述步骤，一步一步来。


### 临时添加SWAP

```
sudo nano /etc/dphys-swapfile
```

更改

```shell
CONF_SWAPSIZE=2048
```

重启服务

```shell
sudo /etc/init.d/dphys-swapfile restart
```

**建议编译完`OpenCV`后改小**

### 配置环境

```shell
#>>>>> 换源
sudo echo "deb https://mirrors.tuna.tsinghua.edu.cn/debian/ buster main contrib non-free
deb-src https://mirrors.tuna.tsinghua.edu.cn/debian/ buster main contrib non-free
deb https://mirrors.tuna.tsinghua.edu.cn/debian/ buster-updates main contrib non-free
deb-src https://mirrors.tuna.tsinghua.edu.cn/debian/ buster-updates main contrib non-free
deb https://mirrors.tuna.tsinghua.edu.cn/debian/ buster-backports main contrib non-free
deb-src https://mirrors.tuna.tsinghua.edu.cn/debian/ buster-backports main contrib non-free
deb https://mirrors.tuna.tsinghua.edu.cn/debian-security buster/updates main contrib non-freerm
deb-src https://mirrors.tuna.tsinghua.edu.cn/debian-security buster/updates main contrib non-free" > /etc/apt/sources.list

#>>>>> 换源
sudo echo "deb http://mirrors.tuna.tsinghua.edu.cn/raspberrypi/ buster main ui" > /etc/apt/sources.list.d/raspi.list

echo "'sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys XXX' if shows 'NO_PUBKEY XXX'"
#>>>>> 更新
sudo apt update

#>>>>> 安装依赖&工具
sudo apt install build-essential cmake -y
sudo apt install git libgtk2.0-dev libavcodec-dev libavformat-dev libjpeg-dev libswscale-dev libtiff5-dev libgtk2.0-dev pkg-config libv4l-dev libhdf5-dev libhdf5-103 -y
sudo apt install libjpeg8-dev -y
sudo apt install libjasper-dev -y
sudo apt install libpng12-dev -y
sudo apt install libatlas-base-dev gfortran -y
sudo apt install net-tools wget curl -y
sudo apt install cheese fswebcam v4l-utils -y
sudo apt install xrdp screen -y

#>>>>> 下载OpenCV
cd ~/Documents
git clone https://github.com/opencv/opencv.git
git clone https://github.com/opencv/opencv_contrib.git

#>>>>> 编译OpenCV
cd ~/Documents/opencv
mkdir build
cd build
echo "Building OpeCV needs looooooooooooong time"
sudo cmake -D CMAKE_BUILD_TYPE=RELEASE \
	-D CMAKE_INSTALL_PREFIX=/usr/local \
	-D OPENCV_EXTRA_MODULES_PATH=/home/pi/Documents/opencv_contrib/modules \
	-D INSTALL_PYTHON_EXAMPLES=ON \
	-D INSTALL_CXX_EXAMPLES=ON \
	-D WITH_LIBV4L=ON \
	-D BUILD_EXAMPLES=ON ..

sudo screen make -j8

sudo screen make install

sudo ldconfig
```

## 其他

### 屏幕不休眠

```shell
sudo nano /etc/lightdm/lightdm.conf
```

```shell
[Seat:*]

#xserver-command=X
xserver-command=X -s 0 -dpms
```

### Zsh

#### [Zsh-Zinit](https://github.com/zdharma/zinit)

```shell
# install
sudo apt install zsh -y
sh -c "$(curl -fsSL https://raw.githubusercontent.com/zdharma/zinit/master/doc/install.sh)"
```

#### [p10k](https://github.com/romkatv/powerlevel10k)

##### [安装字体](src/install_font.sh)

```shell
#Download fonts
mkdir -p ~/Downloads/fonts_MesloLGS
cd ~/Downloads/fonts_MesloLGS
wget https://github.com/romkatv/powerlevel10k-media/raw/master/MesloLGS%20NF%20Regular.ttf
wget https://github.com/romkatv/powerlevel10k-media/raw/master/MesloLGS%20NF%20Bold.ttf
wget https://github.com/romkatv/powerlevel10k-media/raw/master/MesloLGS%20NF%20Italic.ttf
wget https://github.com/romkatv/powerlevel10k-media/raw/master/MesloLGS%20NF%20Bold%20Italic.ttf
sudo chmod 777 *
cd ..
#Install fonts
sudo mv fonts_MesloLGS /usr/share/fonts/
sudo mkfontscale
sudo mkfontdir
sudo fc-cache -fv
```

#### [.zshrc](src/.zshrc)

#### [.p10k.zsh](src/.p10k.zsh)

### 串口通信

[串口设置](https://zhuanlan.zhihu.com/p/106904186)

```
# 遇到Can't Open Serial Port(/dev/ttyAMA0): Permission denied 错误的话
sudo usermod -aG dialout pi
```

### udev设备绑定（可能出错！）

```shell
# sudo nano /etc/udev/rules.d/camera.rules
# sudo service udev restart

# upper USB 3 port Hub
KERNEL=="video[0,2,4,6]", ATTRS{devpath}=="1.1.1", ATTRS{idVendor}=="337b", ATTRS{idProduct}=="090c", MODE:="0777", SYMLINK+="camera0"

# lowwer USB 3 port Hub
KERNEL=="video[0,2,4,6]", ATTRS{devpath}=="1.2.1", ATTRS{idVendor}=="337b", ATTRS{idProduct}=="090c", MODE:="0777", SYMLINK+="camera0"

# upper USB 2 port Hub
KERNEL=="video[0,2,4,6]", ATTRS{devpath}=="1.3.1", ATTRS{idVendor}=="337b", ATTRS{idProduct}=="090c", MODE:="0777", SYMLINK+="camera0"

# lowwer USB 2 port Hub
KERNEL=="video[0,2,4,6]", ATTRS{devpath}=="1.4.1", ATTRS{idVendor}=="337b", ATTRS{idProduct}=="090c", MODE:="0777", SYMLINK+="camera0"

# 每个接一个USB port USB3上1下2 USB2上3下4
KERNEL=="video[0,2,4,6]", ATTRS{devpath}=="1.1", ATTRS{idVendor}=="337b", ATTRS{idProduct}=="090c", MODE:="0777", SYMLINK+="camera0"
```

### 软链接绑定（推荐）
```shell
# e.g.
# ll /dev/v4l/by-id 再用软链接绑定
ll /dev/v4l/by-id 
# $ tree /dev/v4l
# /dev/v4l
# ├── by-id
# │   └── usb-OmniVision._USB_Camera-B4.04.27.1-video-index0 -> ../../video0
# └── by-path
#     └── pci-0000:00:14.0-usb-0:2:1.0-video-index0 -> ../../video0
sudo ln -s /dev/v4l/by-id/usb-337b_HBVCAM_5M-AF-video-index0 /dev/camera1
```

### 自动启动

```shell
sudo nano /home/pi/.config/lxsession/LXDE-pi/autostart
```

加上可执行文件位置

### TCP/UDP 通信

```shell
cd ~
git clone https://github.com/eminfedar/async-sockets-cpp.git
cd async-sockets-cpp/async-sockets
su root -c "./install.sh"
```