/*
 * autoroute_utils.h
 *
 *  Created on: Sep 12, 2014
 *      Author: lfranchi
 */

#define _GNU_SOURCE     /* To get defns of NI_MAXSERV and NI_MAXHOST */
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_link.h>

#include "include/sys_utils.h"
#include "include/net_utils.h"

int main(int argc, char *argv[]) {

	printf("Starting autoroute...\n\n");

	struct ifaddrs *ifaddr, *ifa;
	int family, s, n;
	char host[NI_MAXHOST];
	char *gw = malloc(15);

	int clif_index = 0;
	struct clif clifs[CLIF_ARRAY_SIZE];

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		exit(EXIT_FAILURE);
	}

	for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {

		if (ifa->ifa_addr == NULL)
			continue;

		family = ifa->ifa_addr->sa_family;

		if (family == AF_INET) {

			if (is_ifa_enabled(ifa->ifa_name) == 0)
				continue;

			s = getnameinfo(ifa->ifa_addr,
					(family == AF_INET) ?
							sizeof(struct sockaddr_in) :
							sizeof(struct sockaddr_in6), host, NI_MAXHOST,
					NULL, 0, NI_NUMERICHOST);

			if (s != 0) {
				printf("getnameinfo() failed: %s\n", gai_strerror(s));
				continue;
			}

			find_gateway(ifa->ifa_name, &gw);

			strcpy(clifs[clif_index].name, ifa->ifa_name);
			strcpy(clifs[clif_index].ip, host);
			strcpy(clifs[clif_index++].gw, gw);

		}

	}

	init_rt_tables_file(clifs);


	int ix;
	for(ix = 0; ix < CLIF_ARRAY_SIZE; ix++) {

		if(strlen(clifs[ix].name) == 0)
			break;

		printf("If: %s - IP: %s - GW: %s - RT: %s \n", clifs[ix].name, clifs[ix].ip, clifs[ix].gw, clifs[ix].rt_name);

	}

	freeifaddrs(ifaddr);

	printf("\nFinished!\n");

	exit(EXIT_SUCCESS);

}

