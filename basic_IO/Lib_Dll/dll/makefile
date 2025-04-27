dll:libmymath.so main
main:main.c libmymath.so
	gcc main.c -o main -L. -lmymath
libmymath.so:
	gcc -fPIC -c sub.c add.c
	gcc -shared -o libmymath.so *.o

.PHONE:clean
clean:
	rm -f sub.o add.o main libmymath.so