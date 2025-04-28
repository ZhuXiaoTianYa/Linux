.PHONE:all
all:ClientPipe ServerPipe
ServerPipe:ServerPipe.c
	gcc -o $@ $^
ClientPipe:ClientPipe.c
	gcc -o $@ $^
.PHONE:clean
clean:
	rm -f ClientPipe ServerPipe