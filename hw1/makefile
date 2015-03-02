hw1: prog.o encode.o decode.o hexdump.c
	gcc -c -g -Wall encode.c
	gcc -c -g -Wall decode.c
	gcc -c -g -Wall hexdump.c
	gcc -c -g -Wall prog.c
	gcc -g -Wall *.o -o hw1
clean:
	rm -rf *.o
	rm -rf *.gch
	rm -rf *.exe
	rm -rf f*.*
