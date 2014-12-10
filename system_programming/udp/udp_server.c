/* Sample UDP server */
/* Similar to TCP, but no listen and accept calls */

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define PORT "3490"  // the port users will be connecting to

int main() {

    const char* hostname="localhost";
    struct addrinfo hints, *p;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_DGRAM;
    hints.ai_protocol=0;
    hints.ai_flags=AI_PASSIVE|AI_ADDRCONFIG;
    struct addrinfo* servinfo=0;
    int sockfd;
    int err=getaddrinfo(hostname,PORT,&hints,&servinfo);
    if (err!=0) {printf("getaddrinfo fail\n");}

    for(p = servinfo; p != NULL; p = p->ai_next) {

        char host[256],service[256];
        getnameinfo(p->ai_addr,p->ai_addrlen, host, sizeof(host), service,sizeof(service),NI_NUMERICHOST );
        puts(host);
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                        p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }
        break;
    }

    free(servinfo);
    printf("server bound and waiting\n"); 
    char* word="!";
    char* buffer = calloc(549, 1);
    while(1) {
        ssize_t count=recv(sockfd,buffer,sizeof(buffer),0);
        if (count==-1) {
            printf("recv fail\n");
        }
        else {
            printf("%s\n", buffer);
        }
        if(strstr(buffer, word) != NULL) {
            break;
        }
    }
    close(sockfd);
    return 0;
}
