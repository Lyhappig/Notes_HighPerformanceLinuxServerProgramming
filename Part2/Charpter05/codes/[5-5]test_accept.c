#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <strings.h>

int main() {
	const char* ip = "192.168.163.131";
    int port = 12345;

	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);

	int sock = socket(PF_INET, SOCK_STREAM, 0);
	assert(sock >= 0);

	int ret = bind(sock, (struct sockaddr*) &address, sizeof(address));
	assert(ret != -1);

	ret = listen(sock, 5);
	assert(ret != -1);

	/* 暂停一段时间使得客户端连接和相关操作（掉线或退出）完成 */
	sleep(20);

	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof(client);
	int connfd = accept(sock, (struct sockaddr*) &client, &client_addrlength);
	if (connfd < 0) {
		printf("error: %s\n", strerror(errno));
	} else {
		/* 接受连接成功则打印出客户端的 IP 地址和端口号 */
		char remote[INET_ADDRSTRLEN];
		const char *clinet_ip = inet_ntop(AF_INET, &client.sin_addr, remote, INET_ADDRSTRLEN);
		int clinet_port = ntohs(client.sin_port);
		printf("connected with ip: %s and port %d\n", clinet_ip, clinet_port);
		close(connfd);
	}

	close(sock);
	return 0;
}