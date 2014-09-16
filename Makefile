OBJS = src/autoroute.c src/include/sys_utils.h src/utils/sys_utils.c src/include/net_utils.h src/utils/net_utils.c


all: main


main: 
	gcc -g -Wall -o autoroute $(OBJS) 


clean:
	rm -f *.o  