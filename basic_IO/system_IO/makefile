system_IO:system_IO_W system_IO_R system_IO_display
system_IO_W:system_IO_W.c
	gcc system_IO_W.c -o system_IO_W
system_IO_R:system_IO_R.c
	gcc system_IO_R.c -o system_IO_R
system_IO_display:system_IO_display.c
	gcc system_IO_display.c -o system_IO_display
.PHONE:clean
clean:
	rm -f system_IO_W system_IO_R system_IO_display