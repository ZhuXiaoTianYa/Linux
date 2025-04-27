lib:libmymath.a main
main:libmymath.a main.c
	gcc main.c -o main -L. -lmymath
libmymath.a:add.o sub.o
	ar -rc libmymath.a add.o sub.o
add.o:add.c
	gcc -c add.c -o add.o
sub.o:sub.c
	gcc -c sub.c -o sub.o

.PHONE:clean
clean:
	rm -f libmymath.a main sub.o add.o