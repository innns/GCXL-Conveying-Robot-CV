# -*- coding: utf-8 -*-
import cv2
import threading
import numpy as np
import time


class RealTimeCap:
    # 接收摄影机串流影像，采用多线程的方式，降低缓冲区栈图帧的问题。
    def __init__(self, index, width=640, height=480):
        self.index = index
        self.width = width
        self.height = height
        self.lock = threading.Lock()
        self.pic_time = time.time()   # 时间戳
        self.now_time = time.time()   # 时间戳
        self.__my_init()
        self.__retry = 0

    def __del__(self):
        self.release()

    def __my_init(self):
        self.Frame = np.zeros((self.height, self.width, 3), np.uint8)
        self.status = False
        self.is_stop = False
        # 摄影机连接。
        self.capture = cv2.VideoCapture(self.index)
        self.capture.set(cv2.CAP_PROP_FOURCC,
                         cv2.VideoWriter_fourcc('M', 'J', 'P', 'G'))
        self.capture.set(cv2.CAP_PROP_FRAME_WIDTH, self.width)
        self.capture.set(cv2.CAP_PROP_FRAME_HEIGHT, self.height)
        self.__start()

    def __start(self):
        # 把程序放进子线程，daemon=True 表示该线程会随着主线程关闭而关闭。
        print('cam starting!')
        self.__my_thread = threading.Thread(
            target=self.queryFrame, daemon=True, args=())
        self.__my_thread.start()
        cv2.waitKey(3000)
        print('cam started!')
        print("Note: it's suggested to use\n cv2.namedWindow('XXX') \n before using cv2.imshow()")

    def release(self):
        # 记得要设计停止无限循环的开关。
        self.is_stop = True
        self.capture.release()
        self.__my_thread.join()
        print('cam stopped!')

    def read(self):
        # 当有需要影像时，再回传最新的影像。
        self.now_time = time.time()
        if ((not self.status) and self.__retry < 3):
            del self.__my_thread
            self.__my_thread = threading.Thread(target=self.queryFrame, daemon=True, args=())
            self.__my_thread.start()
            self.__retry += 1
            print("reconnect cam")
            cv2.waitKey(3000)
        self.lock.acquire(blocking=True)
        try:
            if ((self.now_time - self.pic_time) > 0.2):
                cv2.waitKey(100)
            status, frame = self.status, self.Frame
        finally:
            self.lock.release()
        return status, frame

    def queryFrame(self):
        self.status, temp_frame = self.capture.read()
        while (not self.is_stop):
            self.lock.acquire(blocking=False)
            try:
                self.status, self.Frame = self.capture.read()
                self.pic_time = time.time()   # 时间戳
            finally:
                self.lock.release()
            cv2.waitKey(30)


if __name__ == '__main__':
    cv2.namedWindow('Image')
    # 连接摄影机
    cam = RealTimeCap("/dev/camera1")

    # 使用无穷循环撷取影像，直到按下Esc键结束
    while True:
        # 使用 read 取得最新的影像

        ret, img = cam.read()
        if ret:
            try:
                cv2.imshow('Image', img)
            except():
                pass
        if cv2.waitKey(30) == 27:
            cv2.destroyAllWindows()
            break
