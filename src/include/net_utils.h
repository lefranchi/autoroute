/*
 * net_utils.h
 *
 *  Created on: Sep 16, 2014
 *      Author: lfranchi
 */

#ifndef NET_UTILS_H_
#define NET_UTILS_H_

#define RT_SOURCE_FILE_PATH "./conf/rt_tables"
#define RT_TARGET_FILE_PATH "/etc/iproute2/rt_tables"

#define _GNU_SOURCE     /* To get defns of NI_MAXSERV and NI_MAXHOST */

#define CLIF_ARRAY_SIZE_MAX 12

#include <netdb.h>


/*
 * CL - Network Interface.
 */
struct clif {
	char name[6];
	char ip[NI_MAXHOST];
	char gw[NI_MAXHOST];
	char rt_name[15];
	float conn_avg;
};

int is_ifa_enabled(char*);

int find_gateway(char*, char*);

int print_clif_info(struct clif[]);

int init_rt_tables_file(struct clif[]);

int define_rt_tables(struct clif[]);

int define_rt_rules(struct clif[]);

int delete_nexthop_route();

int balance_links(struct clif[]);

int route_flush_cache();

int load_clifs(struct clif[]);

int load_conn_attr(struct clif[]);

int load_conn_avg(char*, int, char*, float*);

#endif /* NET_UTILS_H_ */
