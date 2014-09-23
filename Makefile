LIBS_OBJS = src/include/sys_utils.h src/utils/sys_utils.c src/include/net_utils.h src/utils/net_utils.c
AUTOROUTE_OBJS = src/autoroute.c 
AUTOPPP_OBJS = src/autoppp.c

all: main


main: 
	gcc -g -Wall -o bin/autoroute $(AUTOROUTE_OBJS) $(LIBS_OBJS) 
	gcc -g -Wall -o bin/autoppp $(AUTOPPP_OBJS) $(LIBS_OBJS)  
	gcc -g -Wall -o bin/atinout src/atinout.c

clean:
	rm -f *.o autoroute autoppp atinout