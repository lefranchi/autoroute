/*
 * autoroute_utils.h
 *
 *  Created on: Sep 12, 2014
 *      Author: lfranchi
 */

#include "include/sys_utils.h"
#include "include/net_utils.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

	printf("Starting autoroute...\n\n");

	struct clif clifs[CLIF_ARRAY_SIZE];

	load_clifs(clifs);

	init_rt_tables_file(clifs);

	print_clif_info(clifs);

	define_rt_tables(clifs);

	define_rt_rules(clifs);

	delete_nexthop_route();

	balance_links(clifs);

	route_flush_cache();

	printf("\nFinished!\n");

	exit(EXIT_SUCCESS);

}

