all:
	gcc server.c -o server -lpthread
	gcc client1.c -o client1 -lpthread
	gcc client2.c -o client2 -lpthread
