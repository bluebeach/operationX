#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define PORT 4567
#define BUFLEN 1000

int main(int argc, char **argv) {

		int listen_socket, accept_socket;
		socklen_t len;
		struct sockaddr_in local_address, remote_address;
		char buf[BUFLEN];
		if( (listen_socket = socket( AF_INET, SOCK_STREAM, 0 )) == -1 ) {
				perror( "listen_socket" );
				exit(errno);
		}
		printf("after create socket\n");
			
		memset(&local_address, 0, sizeof(local_address));
		local_address.sin_family = AF_INET;
		local_address.sin_port = htons(PORT);		
		local_address.sin_addr.s_addr = INADDR_ANY;
		printf("after set local_address\n");

		if( bind( listen_socket, (struct sockaddr *) &local_address, sizeof(struct sockaddr) ) == -1 ) {
				perror("bind");
				exit(errno);
		}
		printf("after bind\n");
		if( listen( listen_socket, 5 ) == -1 ) {
				perror("listen");
				exit(errno);
		}
		printf("server is listening...\n");

		while(1) {
				printf("********聊天开始***********\n");
				len = sizeof( struct sockaddr );
				accept_socket = accept(listen_socket, (struct sockaddr *) &remote_address, &len );
				if( accept_socket == -1 ) {
						perror(" accept ");
						exit(errno);
				}
				else {
						printf("server: got connection from %s, port %d\n",inet_ntoa(remote_address.sin_addr),ntohs(remote_address.sin_port));
				}
				while(1) {
						memset(&buf, 0, BUFLEN);
						printf("请输入：");
						fgets(buf, BUFLEN, stdin);
						if(!strncasecmp(buf,"quit",4)) {
								printf("server quit！\n");
								break;
						}
						if(!strncmp(buf, "\n", 1)) {
								printf("请重新输入。\n");
								continue;
						}
						if(strchr(buf, '\n'))
								len = send(accept_socket, buf, strlen(buf)-1, 0);
						else
								len = send(accept_socket, buf, strlen(buf), 0);
						if(len > 0)
								printf("消息发送成功。本次共发送的字节数是：%d\n",len);
						else{
								printf("消息发送失败!\n");
								break;
						}

						memset(buf, 0, BUFLEN);
						len = recv(accept_socket, buf, BUFLEN, 0);
						if( len > 0 )
								printf("客户端：%s, 共有字节数是：%d\n", buf, len);
						else {
								if(len < 0)
										printf("消息接收失败！\n");
								else
										printf("客户端退出了，聊天终止！\n");
								break;
						}
				}
				close(accept_socket);
				printf("是否退出程序：y/n?");
				memset(buf, 0, BUFLEN);
				fgets(buf, BUFLEN, stdin);
				if(!strncasecmp(buf, "y", 1)) {
						printf("退出！\n");
						break;
				}
		}
		close(listen_socket);
		return 0;
}
