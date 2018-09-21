/*
 * 9-21-2018 by kk
 * 最简单的socket聊天小程序（服务器）
 * 一个作为服务器，另外一个作为客户端
 * 由于当前使用的是局域网，因此需要在路由器中设置虚拟服务器，
 * 把内网IP和端口与路由器端口映射
 * note:需要把局域网中的电脑IP设为手动设置模式
 * 输入exit()退出聊天
 * */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>

#define MAX_MSG_SIZE 256
#define  SERVER_PORT 8000
#define BACKLOG 2

int main()
{
    int sock_fd, client_fd;
    struct sockaddr_in ser_addr;
    struct sockaddr_in cli_addr;
    char msg[MAX_MSG_SIZE];
    int ser_sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if(ser_sockfd < 0)
    {
        fprintf(stderr, "Bind Error:%s\n", strerror(errno));
        exit(1);
    }
    socklen_t  addrlen = sizeof(struct sockaddr_in);
    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    //ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_addr.s_addr = inet_addr("192.168.1.100");//内网IP，需在路由器中进行映射后
    ser_addr.sin_port = htons(SERVER_PORT);
    if (bind(ser_sockfd, (struct sockaddr *) &ser_addr, sizeof(ser_addr)) == -1)
    {
        fprintf(stderr, "Bind Error:%s\n",strerror(errno));
        exit(1);
    }
    if(listen(ser_sockfd, BACKLOG) < 0)
    {
        fprintf(stderr, "Listen Error:%s\n",strerror(errno));
        close(ser_sockfd);
        exit(1);
    }

    int cli_sockfd = accept(ser_sockfd, (struct sockaddr *) &cli_addr, &addrlen);
    if (cli_sockfd < 0) {
        fprintf(stderr, "Accept Error:%s\n", strerror(errno));
    } else {
        while(1){
            recv(cli_sockfd, msg, (size_t) MAX_MSG_SIZE, 0);
            printf("client:%s", msg);
            printf("server:");
            fgets(msg, MAX_MSG_SIZE, stdin);
            if(strcmp(msg, "exit()\n")==0) break;
            send(cli_sockfd, msg, sizeof(msg), 0);
        }
        close(cli_sockfd);
    }
    close(ser_sockfd);
    return 0;
}