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
    int server_fd;
    struct sockaddr_in server_addr;
    char buf[BUFSIZ];
    int rec, port;
    if(argc != 3){
        printf("Please enter IP and Port\n");
    }
    /*1.创建socket*/
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(1);
    }
    else{
        printf("Success to socket\n");
    }
    /*2.设置地址结构*/
    port = atoi(argv[2]);
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if(inet_pton(AF_INET, argv[1], (void *)&server_addr.sin_addr) != 1){
        perror("inet_pton");
        exit(1);
    }
    else{
        printf("Success to inet_pton\n");
    }
    /*3.连接到服务器端*/
    if(connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("connect");
        exit(1);
    }
    else{
        printf("Success to connect\n");
    }
    /*4.读写数据*/
    while(1){
        bzero(&buf, BUFSIZ);
        if(fgets(buf, BUFSIZ - 1, stdin) == NULL){
            continue;
        }
        do{
            rec = write(server_fd, buf, strlen(buf));
        }
        while(rec < 0 && EINTR == errno);
        if(strncasecmp(buf, QUIT_STR, strlen(QUIT_STR)) == 0){
        printf("Client exit\n");
        break;
        }
    }
}