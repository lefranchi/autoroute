OBJS = src/autoroute.c


all: main


main: 
	gcc -g -Wall -o autoroute $(OBJS) 


clean:
	$(RM) main $(OBJS)  