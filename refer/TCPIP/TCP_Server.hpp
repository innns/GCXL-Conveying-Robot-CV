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
 * @Date: 2021-05-05 17:42:35
 * @LastEditors: innns
 * @LastEditTime: 2021-05-06 19:17:00
 * @FilePath: /GCXL-Conveying-Robot-CV/include/TCP/TCP_Server.hpp
*/
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

#define SERVER_PORT 8888      //服务器端口
#define SERVER_IP "127.0.0.1" //服务器ip

using namespace std;

class TCP_Server
{
private:
    int servfd, clitfd;                      //创建两个文件描述符，servfd为监听套接字，clitfd用于数据传输
    struct sockaddr_in serv_addr, clit_addr; //创建地址结构体，分别用来存放服务端和客户端的地址信息
    int rdstate;
    socklen_t clit_size = 0; //用于accept函数中保存客户端的地址结构体大小
    char buf[1024];          //用于读写数据

public:
    TCP_Server(/* args */);
    ~TCP_Server();
    bool connected;
    bool connect2Client();
    bool recv();                     // 直接打印
    bool recv(std::string &message); // 打印并返回给message
};

TCP_Server::TCP_Server(/* args */)
{

    memset(&serv_addr, 0, sizeof(serv_addr)); //初始化
    memset(&clit_addr, 0, sizeof(clit_addr)); //初始化

    if ((servfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) //创建套接字
    {
        cout << "creat socket failed : " << strerror(errno) << endl; //如果出错则打印错误
        return;
    }

    //给服务端的地址结构体赋值
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);          //将主机上的小端字节序转换为网络传输的大端字节序（如果主机本身就是大端字节序就不用转换了）
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP); //将字符串形式的ip地址转换为点分十进制格式的ip地址

    //绑定地址信息到监听套接字上，第二个参数强转是因为形参类型为sockaddr ，而实参类型是sockaddr_in 型的
    if (bind(servfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        cout << "bind failed : " << strerror(errno) << endl;
        return;
    }

    //将servfd套接字置为监听状态
    if (listen(servfd, 1024) == -1)
    {
        cout << "listen failed : " << strerror(errno) << endl;
        return;
    }

    cout << "Init Success ! " << endl;
    cout << "ip : " << inet_ntoa(serv_addr.sin_addr) << "  port : " << ntohs(serv_addr.sin_port) << endl;
    connect2Client();
}

bool TCP_Server::connect2Client()
{
    cout << "Waiting for connecting ... " << endl;
    //accept成功后，clitfd则指向了这条服务端与客户端成功连接的”通路“
    if ((clitfd = accept(servfd, (sockaddr *)&clit_addr, &clit_size)) == -1)
    {
        cout << "accept failed : " << strerror(errno) << endl;
        connected = false;
    }
    else
    {
        cout << "Client access : " << inet_ntoa(clit_addr.sin_addr) << "  " << ntohs(clit_addr.sin_port) << endl;
        connected = true;
    }
    return connected;
}

TCP_Server::~TCP_Server()
{
    close(servfd); //关闭套接字
    close(clitfd);
}

bool TCP_Server::recv()
{
    if ((rdstate = read(clitfd, buf, sizeof(buf))) > 0) //通过clitfd来读取数据，返回值为读取的长度
    {
        int i = 0;
        cout << "(Server)recv : ";
        for (i = 0; i < rdstate; i++)
        {
            cout << buf[i];
            buf[i] = toupper(buf[i]); //转换为大写
        }
        buf[i] = '\0';
        cout << endl;
        write(clitfd, buf, strlen(buf)); //发回客户端
        return true;
    }
    else if (rdstate == 0) //客户端退出
    {
        cout << "client exit ! " << endl;
        connect2Client();
    }
    return false;
}

bool TCP_Server::recv(std::string &message)
{
    if ((rdstate = read(clitfd, buf, sizeof(buf))) > 0) //通过clitfd来读取数据，返回值为读取的长度
    {
        int i = 0;
        cout << "(Server)recv : ";
        for (i = 0; i < rdstate; i++)
        {
            cout << buf[i];
            buf[i] = toupper(buf[i]); //转换为大写
        }
        cout << endl;
        message = buf;
        buf[i] = '\0';                   //最后用\0结尾发回
        write(clitfd, buf, strlen(buf)); //发回客户端
        return true;
    }
    else if (rdstate == 0) //客户端退出
    {
        cout << "client exit ! " << endl;
        connect2Client();
    }
    return false;
}
