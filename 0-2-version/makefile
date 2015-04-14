all:server2 client2
.PHONY:all
server2:server2.o
	gcc -o server2 server2.o -lpthread
server2.o:server2.c
	gcc -c server2.c
client2:client2.o
	gcc -o client2 client2.o -lpthread
client2.o:client2.c
	gcc -c client2.c
clean:
	rm server2.o
	rm client2.o
	rm server2
	rm client2
