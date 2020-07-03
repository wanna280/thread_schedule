main:main.c thread.c thread.h schedule.c func.h switch.s
	gcc -g main.c thread.c schedule.c func.h switch.s -o main
run:
	./main
