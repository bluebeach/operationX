#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define PORT 4567
#define BUFLEN 1000

int main(int argc, char *argv[]) {
		int connect_socket;
		char ip[16];
		struct sockaddr_in server_addr;
		char buf[BUFLEN];
		socklen_t len;

		printf("Please input the server address: ");
		fgets(ip, 16, stdin);

		//创建socket
		connect_socket = socket(AF_INET, SOCK_STREAM, 0);
		if( connect_socket == -1 ) {
				perror(" connect_socket ");
				exit(errno);
		}
		printf("after create socket.\n");

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
		printf("connect success ! \n start ...\n");
		while(1) {
				memset(buf, 0, BUFLEN);
				len = recv(connect_socket, buf, BUFLEN, 0);
				if(len > 0)
						printf("服务器：%s,共有字节数是: %d\n",buf,len);
				else{
						if(len < 0 )
								printf("接受消息失败！\n");
						else
								printf("服务器退出了，聊天终止！\n");
						break;    
				}

				memset(buf, 0, BUFLEN);
				printf("请输入：");
				/*fgets函数：从流中读取BUFLEN-1个字符*/
				fgets(buf,BUFLEN,stdin);
				/*打印发送的消息*/
				//fputs(buf,stdout);
				if(!strncasecmp(buf,"quit",4)){
						printf("client quit!\n");
						break;
				}
				/*如果输入的字符串只有"\n"，即回车，那么请重新输入*/
				if(!strncmp(buf,"\n",1)){
						printf("请重新输入。\n");
						continue;
				}
				/*如果buf中含有'\n'，那么要用strlen(buf)-1，去掉'\n'*/    
				if(strchr(buf,'\n'))
						len = send(connect_socket,buf,strlen(buf)-1,0);
				/*如果buf中没有'\n'，则用buf的真正长度strlen(buf)*/    
				else
						len = send(connect_socket,buf,strlen(buf),0);
				if(len > 0)
						printf("消息发送成功，本次共发送的字节数是：%d\n",len);            
				else{
						printf("消息发送失败!\n");
						break;            
				}
		}
		while(1){

		}
		return 0;
}

