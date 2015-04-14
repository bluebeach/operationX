#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define PORT 4567
#define BUFLEN 1000
#define LOCAL "127.0.0.1"

char buf_read[BUFLEN];
char buf_write[BUFLEN];

void handle_read(int sock) {
		int len;
		while(1) {
				memset(&buf_read, 0, BUFLEN);
				len = recv(sock, buf_read, BUFLEN, 0);
				if(len > 0) {
						if(strncasecmp(buf_read, "quit", 4) == 0) {
								printf("【Server Quit!】\n");
								break;
						}
						printf("服务器：%s. (字节数：%d)\n", buf_read, len);
				}
				else {
						if(len < 0)
								printf("【接收失败！】\n");
						else
								printf("【服务器退出聊天！】\n");
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

				if(!strncmp(buf_write, "\n", 1))  {
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
								printf("【Client Quit!】\n");
								break;
						}
				}
				else {
						printf("【发送失败！】\n");
						break;
				}
		}
		close(sock);
		//printf("Write thread exit!\n");
		pthread_exit((void *)1);
}

void Chat(int sock) {
		int ret_thread;
		pthread_t write_thread;
		ret_thread = pthread_create(&write_thread, NULL, handle_write, (void *)&sock);
		if(ret_thread != 0)
				printf("write 线程创建失败。\n");
		
		handle_read(sock);
		printf(" ---End of Chat--- \n");
}

int main(int argc, char *argv[]) {
		int connect_socket;
		char ip[16];
		struct sockaddr_in server_addr;
		char buf[BUFLEN];
		socklen_t len;

		printf("Please input the server address: ");
		printf("%s ? y/n ? ",LOCAL);
		fgets(ip, 16, stdin);
		if(strncasecmp(ip, "y", 1) == 0) {
				strcpy(ip, LOCAL);
		}
		else {
				printf("Address : ");
				memset(ip, 0, 16);
				fgets(ip, 16, stdin);
		}

		//创建socket
		connect_socket = socket(AF_INET, SOCK_STREAM, 0);
		if( connect_socket == -1 ) {
				perror(" connect_socket ");
				exit(errno);
		}

		//设置服务器ip和端口
		memset( &server_addr, 0, sizeof(server_addr) );
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(PORT);
		server_addr.sin_addr.s_addr = inet_addr(ip);

		//连接服务器
		if( connect(connect_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1 ) {
				perror("my connect " );
				exit(errno);
		}
		printf("==========聊天开始===========\n退出聊天：quit (不分大小写).\n");
		Chat(connect_socket);

		return 0;
}

