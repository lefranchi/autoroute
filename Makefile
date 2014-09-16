OBJS = src/autoroute.c src/include/autoroute_utils.h src/utils/autoroute_utils.c


all: main


main: 
	gcc -g -Wall -o autoroute $(OBJS) 


clean:
	rm -f *.o  