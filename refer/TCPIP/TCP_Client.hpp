#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

#define SERVER_PORT 8888      //服务器端口
#define SERVER_IP "127.0.0.1" //服务器ip

using namespace std;

class TCP_Client
{
private:
    int clitfd;                   //文件描述符
    struct sockaddr_in serv_addr; //目的服务端地址结构体
    char sdbuf[1024];
    char rvbuf[1024];

public:
    TCP_Client(/* args */);
    ~TCP_Client();
    bool connected = false;
    bool connect2Server();
    int send2Server();
};

TCP_Client::TCP_Client(/* args */)
{
    memset(&serv_addr, 0, sizeof(serv_addr));
    if ((clitfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) //创建套接字
    {
        cout << "creat socket failed : " << strerror(errno) << endl;
        return;
    }
    //将目的服务端的地址信息赋值给地址结构体
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    connect2Server();
}

TCP_Client::~TCP_Client()
{
    close(clitfd);
}

bool TCP_Client::connect2Server()
{
    cout << "try to connect ... " << endl;
    //通过套接字发起连接请求，成功后clitfd套接字则表示此次成功的连接
    if (connect(clitfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        cout << "connet failed : " << strerror(errno) << endl;
        connected = false;
        return connected;
    }
    cout << "connect success !" << endl;
    connected = true;
    return connected;
}

int TCP_Client::send2Server()
{
    int rdlen, sdlen, i = 0;
    cout << "(Client)send : ";

    while ((sdbuf[i] = getchar()) != '\n')
        i++;
    if (i == 0)
        return -1; // 防止客户端只发一个换行符，此时i=0，write不发送数据，服务端就不回回发数据，然后客户端就一直阻塞在read处。因此如果i=0，则直接重新输入

    sdlen = write(clitfd, sdbuf, i); // 向套接字中写入数据发送
                                     // 可能会出现发送端把长度为sdlen的字符串分为多次发送，调用一次read就很有可能不能读取完全，就有以下两种方式进行读取：
                                     // write了多少字节就读取多少字节长的字符串
    rdlen = 0;
    while (rdlen < sdlen) //防止发送端将数据分开发送
    {
        int rdcnt = read(clitfd, &rvbuf[rdlen], sizeof(rvbuf));
        if (rdcnt == -1)
        {
            perror(NULL);
            continue;
        }
        rdlen += rdcnt;
    }
    if (rdlen)
    {
        rvbuf[rdlen] = '\0';
        cout << "(Client)recv : " << rvbuf << endl;
    }
    else
    {
        cout << "Server has closed ! " << endl;
        cout << "Client will close..." << endl;
        connected = false;
        return -1;
    }
    return 0;
}