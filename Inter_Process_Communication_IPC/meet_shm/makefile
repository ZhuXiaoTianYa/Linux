.PHONE:all
all:client server
client:client.c comm.c
	gcc -o $@ $^
server:server.c comm.c
	gcc -o $@ $^
.PHONE:clean
clean:
	rm -f server client