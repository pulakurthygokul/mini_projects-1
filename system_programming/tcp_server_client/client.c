#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(){
    int cfd;
    char ch = 'r';
    struct sockaddr_in addr;
    cfd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family=AF_INET; //ipv4
    addr.sin_addr.s_addr=inet_addr("127.0.0.1"); //htons is host to network
    addr.sin_port=htons(29008); //any server > 1024 

    if(connect(cfd, (struct sockaddr*)&addr, sizeof(addr))<0) {
        perror("connect error\n");
        return -1;
    }
    if(write(cfd, &ch, 1)<0) perror("write");
    if(read(cfd, &ch, 1)<0) perror("read");
    printf("Reply from server: %c\n", ch);
    close(cfd);
    return 0;
}
