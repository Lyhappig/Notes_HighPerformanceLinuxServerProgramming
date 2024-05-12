#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char* argv[]) {
    const char* ip = "192.168.163.131";
    int port = 12345;
    int backlog = 5;

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(sock, backlog);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    while (1) {
        int connfd = accept(sock, (struct sockaddr*)&client, &client_addrlength);
        if (connfd < 0) {
            printf("errno is: %s\n", strerror(errno));
        } else {
            char buffer[BUF_SIZE];

            memset(buffer, '\0', BUF_SIZE);
            ret = recv(connfd, buffer, BUF_SIZE - 1, 0);
            printf("got %d bytes of normal data '%s'\n", ret, buffer);

            memset(buffer, '\0', BUF_SIZE);
            ret = recv(connfd, buffer, BUF_SIZE - 1, MSG_OOB);
            printf("got %d bytes of oob data '%s'\n", ret, buffer);

            memset(buffer, '\0', BUF_SIZE);
            ret = recv(connfd, buffer, BUF_SIZE - 1, 0);
            printf("got %d bytes of normal data '%s'\n", ret, buffer);

            close(connfd);
        }
    }
    close(sock);
    return 0;
}