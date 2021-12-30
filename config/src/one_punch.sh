#!/bin/sh
# >>> swap
sudo cp /etc/dphys-swapfile /etc/dphys-swapfile_
sudo sh -c 'echo "CONF_SWAPSIZE=2048" > /etc/dphys-swapfile'
sudo /etc/init.d/dphys-swapfile restart

# >>> 换源
sudo echo "deb https://mirrors.tuna.tsinghua.edu.cn/debian/ buster main contrib non-free
deb-src https://mirrors.tuna.tsinghua.edu.cn/debian/ buster main contrib non-free
deb https://mirrors.tuna.tsinghua.edu.cn/debian/ buster-updates main contrib non-free
deb-src https://mirrors.tuna.tsinghua.edu.cn/debian/ buster-updates main contrib non-free
deb https://mirrors.tuna.tsinghua.edu.cn/debian/ buster-backports main contrib non-free
deb-src https://mirrors.tuna.tsinghua.edu.cn/debian/ buster-backports main contrib non-free
deb https://mirrors.tuna.tsinghua.edu.cn/debian-security buster/updates main contrib non-freerm
deb-src https://mirrors.tuna.tsinghua.edu.cn/debian-security buster/updates main contrib non-free" > /etc/apt/sources.list

sudo echo "deb http://mirrors.tuna.tsinghua.edu.cn/raspberrypi/ buster main ui" > /etc/apt/sources.list.d/raspi.list

# >>> 更新
sudo apt update

# >>> 安装依赖&工具
sudo apt install build-essential cmake -y
sudo apt install build-essential libgtk2.0-dev libavcodec-dev libavformat-dev libjpeg-dev libswscale-dev libtiff5-dev libgtk2.0-dev pkg-config libv4l-dev libhdf5-dev libhdf5-103 -y
sudo apt install libjpeg8-dev -y
sudo apt install libjasper-dev -y
sudo apt install libpng12-dev -y
sudo apt install libatlas-base-dev gfortran -y
sudo apt install net-tools wget curl -y
sudo apt install cheese fswebcam v4l-utils -y
sudo apt install xrdp screen -y

# >>> 下载OpenCV
cd ~/Documents
git clone https://github.com/opencv/opencv.git
git clone https://github.com/opencv/opencv_contrib.git

# >>> 编译OpenCV
cd opencv
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

# >>> restore swap
sudo mv /etc/dphys-swapfile_ /etc/dphys-swapfile
sudo /etc/init.d/dphys-swapfile restart