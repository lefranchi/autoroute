SYS_UTILS_OBJS = src/include/sys_utils.h src/utils/sys_utils.c 
NET_UTILS_OBJS = src/include/net_utils.h src/utils/net_utils.c
STR_UTILS_OBJS = src/include/str_utils.h src/utils/str_utils.c
AUTOROUTE_OBJS = src/autoroute.c 
AUTOPPP_OBJS = src/autoppp.c

all: main


main: 
	gcc -g -Wall -o bin/autoroute $(AUTOROUTE_OBJS) $(SYS_UTILS_OBJS) $(NET_UTILS_OBJS) $(STR_UTILS_OBJS)
	gcc -g -Wall -o bin/autoppp $(AUTOPPP_OBJS) $(SYS_UTILS_OBJS) $(NET_UTILS_OBJS) $(STR_UTILS_OBJS)
	gcc -g -Wall -o bin/atinout src/atinout.c

clean:
	rm -f *.o autoroute autoppp atinout