a.out : myshell.o
	gcc myshell.o
myshell.o : myshell.c
	gcc -c myshell.c
clean:
	-@rm  *.o
