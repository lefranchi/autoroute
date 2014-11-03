/*
 * autoroute_utils.h
 *
 * 	Description: List allowed interfaces, gets ip and gateway. Creates
 *				route table and execute commands to use these interfaces
 * 				to traffic.
 *
 *
 *  Created on: Sep 12, 2014
 *      Author: lfranchi
 */

#include "include/sys_utils.h"
#include "include/net_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

int main(int argc, char *argv[])
{
	struct clif clifs[CLIF_ARRAY_SIZE_MAX];

	openlog("autoroute", LOG_PID|LOG_CONS, LOG_USER);

	syslog(LOG_INFO, "Initializing ... ");

	load_clifs(clifs);

	load_conn_attr(clifs);

	//init_rt_tables_file(clifs);

	print_clif_info(clifs);

	//define_rt_tables(clifs);

	//define_rt_rules(clifs);

	//delete_nexthop_route();

	//balance_links(clifs);

	//route_flush_cache();

	syslog(LOG_INFO, "Finished.");

	exit(EXIT_SUCCESS);

}

