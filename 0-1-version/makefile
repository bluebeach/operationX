all:server client
.PHONY:all
server:server.o
	gcc -o server server.o -lpthread
server.o:server.c
	gcc -c server.c
client:client.o
	gcc -o client client.o -lpthread
client.o:client.c
	gcc -c client.c
clean:
	rm server.o
	rm client.o
	rm server
	rm client
