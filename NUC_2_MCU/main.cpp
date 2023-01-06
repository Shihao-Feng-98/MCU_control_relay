#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <termios.h>
#include <string.h>
#include <iostream>

/* 115200, 8, N, 1 */
int uart_setup(int fd)
{
    struct termios options;

    // 获取原有串口配置
    if  (tcgetattr(fd, &options) < 0) {
        return -1;
    }

    // 修改控制模式，保证程序不会占用串口
    options.c_cflag  |=  CLOCAL;

    // 修改控制模式，能够从串口读取数据
    options.c_cflag  |=  CREAD;

    // 不使用流控制
    options.c_cflag &= ~CRTSCTS;

    // 设置数据位
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    // 设置奇偶校验位
    options.c_cflag &= ~PARENB;
    options.c_iflag &= ~INPCK; 

    // 设置停止位
    options.c_cflag &= ~CSTOPB;

    // 设置最少字符和等待时间
    options.c_cc[VMIN] = 1;     // 读数据的最小字节数
    options.c_cc[VTIME]  = 0;   //等待第1个数据，单位是1s
    
    // 修改输出模式，原始数据输出
    options.c_oflag &= ~OPOST;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // 设置波特率
    cfsetispeed(&options, B115200); 
    cfsetospeed(&options, B115200);

    // 清空终端未完成的数据
    tcflush(fd, TCIFLUSH);

    // 设置新属性
    if(tcsetattr(fd, TCSANOW, &options) < 0) {
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int fd;
    int ret;
    int send_len = 5; // 4 + 1
    int recv_len = 10; 
    char send_buf[send_len];
    char recv_buf[recv_len];

    if (argc != 2) {
        std::cout << "Usage: ./main [device]\n";
        return -1;
    }

    /* 打开串口 */
    fd = open(argv[1], O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        std::cout << "Open dev fail!\n";
        return -1;
    } else {
        fcntl(fd, F_SETFL, 0);
    }

    /* 设置串口 */
    ret = uart_setup(fd);
    if (ret < 0) {
        std::cout << "Uart setup fail!\n";
        close(fd);
        return -1;
    }

    /* 串口回传实验 */
    while (1) {
        std::cin.getline(send_buf, send_len); // 多读了一个 '\0'
        ret = write(fd, send_buf, send_len-1);
        // std::cout << "Write: " << send_buf << ", ret: " << ret << "\r\n";

        ret = read(fd, recv_buf, recv_len);
        // if (ret < recv_len) {
        //     std::cout << "Read fail, ret: " <<  ret << "\r\n";
        // } else {
        //     std::cout << "Read: " << recv_buf << "\r\n";
        // }
    }

    close(fd);
}

//TODO: 通讯要等1s才能执行, check
