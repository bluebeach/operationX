#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#define PORT 4567
#define BUFLEN 1000

char buf_read[BUFLEN];
char buf_write[BUFLEN];

void handle_read(int sock) {
		int len;
		while(1) {
			memset(buf_read, 0, BUFLEN);
			len = recv(sock, buf_read, BUFLEN, 0);
			if(len > 0) {
					if(strncasecmp(buf_read, "quit", 4) == 0) {
							printf("【Client Quit!】\n");
							close(sock);
							break;
					}
					printf("客户端：%s. (字节数：%d)\n",buf_read, len);
			}
			else {
					if(len < 0)
							printf("【消息接收失败！】\n");
					else
							printf("【客户端退出了，聊天终止！】\n");
					break;
			}
		}
}

void *handle_write(void *arg) {
		int sock = *((int *) arg);
		int len;
		while(1) {
				memset(&buf_write, 0, BUFLEN);
				fgets(buf_write, BUFLEN, stdin);

				if(!strncmp(buf_write, "\n", 1)) {
						printf("请重新输入:\n");
						continue;
				}
				if(strchr(buf_write, '\n'))
						len = send(sock, buf_write, strlen(buf_write)-1, 0);
				else
						len = send(sock, buf_write, strlen(buf_write), 0);
				if(len > 0) {
						printf("(发送成功。发送字节数：%d)\n", len);
						if(strncasecmp(buf_write, "quit", 4) == 0) {
								printf("【Server Quit !】\n");
								break;
						}
				}
				else {
						printf("【发送失败。】\n");
						break;
				}
		}
		//printf("Write thread exit!\n");
		close(sock);
		pthread_exit((void*) 1);
}

void Chat(int sock) {
		int ret_thread;
		pthread_t write_thread;
		ret_thread = pthread_create(&write_thread, NULL, handle_write, (void *)&sock);
		if(ret_thread != 0)
				printf("write 线程创建失败。\n");
		handle_read(sock);
		printf(" =====End of Chat===== \n");
}

int main(int argc, char **argv) {

		int listen_socket, accept_socket;
		socklen_t len;
		struct sockaddr_in local_address, remote_address;
		char buf[BUFLEN];
		if( (listen_socket = socket( AF_INET, SOCK_STREAM, 0 )) == -1 ) {
				perror( "listen_socket" );
				exit(errno);
		}
			
		memset(&local_address, 0, sizeof(local_address));
		local_address.sin_family = AF_INET;
		local_address.sin_port = htons(PORT);		
		local_address.sin_addr.s_addr = INADDR_ANY;

		if( bind( listen_socket, (struct sockaddr *) &local_address, sizeof(struct sockaddr) ) == -1 ) {
				perror("bind");
				exit(errno);
		}
		if( listen( listen_socket, 5 ) == -1 ) {
				perror("listen");
				exit(errno);
		}
		printf("Server is listening================================>>>\n");

		while(1) {
				len = sizeof( struct sockaddr );
				accept_socket = accept(listen_socket, (struct sockaddr *) &remote_address, &len );
				if( accept_socket == -1 ) {
						perror(" accept ");
						exit(errno);
				}
				printf("==========聊天开始==========\n(连接来自：%s. 端口号：%d.)\n退出聊天：quit (不分大小写).\n", inet_ntoa(remote_address.sin_addr), ntohs(remote_address.sin_port) );
				Chat(accept_socket); 
				printf("【Ctrl + C】 退出服务器...\n");
		}
		close(listen_socket);
		return 0;
}
