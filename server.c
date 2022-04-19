#include "stdio.h"
#include "errno.h"
#include "string.h"
#include "strings.h"
#include "unistd.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "netinet/ip.h"
#include "arpa/inet.h"
#include "pthread.h"
#include "signal.h"
#define SERV_PORT 5001
#define QUIT_STR "quit"
#define MAX_CLIENT_NUM 20
int main(int argc, char *argv[]){
    int server_fd, client_fd, rec, rev;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;
    char buf[BUFSIZ];
    char ipv4_addr[16];
    /*1.创建socket*/
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(1);
    }
    else{
        printf("Success to socket\n");
    }
    /*2.配置地址结构*/
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    bzero(&(server_addr.sin_zero), 8);
    /*3.把地址和套接字绑定*/
    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("bind");
        close(server_fd);
        exit(1);
    }
    else{
        printf("Success to bind\n");
    }
    /*4.设置套接字监听*/
    if(listen(server_fd, MAX_CLIENT_NUM) < 0){
        perror("listen");
        close(server_fd);
        exit(1);
    }
    else{
        printf("Success to listen\n");
    }
    /*5.接受客户端请求，与客户端连接*/
    len = sizeof(client_addr);
    if((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len)) < 0){
        perror("accept");
        close(server_fd);
        exit(1);
    }
    else{
        inet_ntop(AF_INET, (void *)&client_addr.sin_addr, ipv4_addr, sizeof(client_addr));
        printf("IPV4_IP:%s:%d\n", ipv4_addr, htons(client_addr.sin_port));
        printf("Success to accept\n");
    }
    /*6.接受用户发送的数据*/
    while(1){
        bzero(&buf, BUFSIZ);
        do{
            rec = read(client_fd, buf, BUFSIZ - 1);
        }
        while(rec < 0 && EINTR == errno);
        printf("Receive data = %s\n", buf);
        
        if(strncasecmp(buf, QUIT_STR, strlen(QUIT_STR)) == 0){
            printf("Client exit\n");
            break;
        }
    }
}