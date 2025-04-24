example:_exit_example exit_example
_exit_example:_exit_example.c
	gcc -o _exit_example _exit_example.c
exit_example:exit_example.c
	gcc -o exit_example exit_example.c

.PHONE:clean
clean:
	rm -f _exit_example exit_example