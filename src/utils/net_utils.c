/*
 * net_utils.c
 *
 *  Created on: Sep 16, 2014
 *      Author: lfranchi
 */

#define _GNU_SOURCE     /* To get defns of NI_MAXSERV and NI_MAXHOST */

#include "../include/net_utils.h"

#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_link.h>
#include <syslog.h>

#include "../include/sys_utils.h"

int is_ifa_enabled(char* ifa_name) {

	int allowed_if_names_size = 1;
	char *allowed_if_names[1] = { "ppp" }; //TODO CARREGAR DE ARQUIVO DE CONFIGURACOES

	int ix;
	for (ix = 0; ix < allowed_if_names_size; ix++) {

		if (strstr(ifa_name, allowed_if_names[ix]) != 0)
			return 1;

	}

	return 0;
}

int find_gateway(char* ifname, char** ifgw)
{

	char command[80] = "";

	sprintf(command, "/sbin/route -n | grep %s | grep '^0.0.0.0' | awk '{ print $2 }'", ifname);

	int ret_value = execute_command(command, ifgw);

	if (strcmp(*ifgw, "") == 0) {

		sprintf(command, "/sbin/ifconfig %s | grep P-t-P | awk '{print $3}' | awk -F: '{print $2}'", ifname);

		ret_value = execute_command(command, ifgw);
	}

	return ret_value;
}

int init_rt_tables_file(struct clif clifs[])
{
	char rt_default_file_path[100];
	char rt_name[5];
	FILE *pFile;
	int rt_index = 253;

	realpath(RT_SOURCE_FILE_PATH, rt_default_file_path);
	copy_file(rt_default_file_path, RT_TARGET_FILE_PATH);

	pFile = fopen(RT_TARGET_FILE_PATH, "a");

	if(pFile != NULL) {

		int ix;
		for(ix = 0; ix < CLIF_ARRAY_SIZE; ix++) {

			if(strlen(clifs[ix].name) == 0)
				break;

			sprintf(rt_name, "%d\trt_%s\n", --rt_index, clifs[ix].name);

			fputs(rt_name, pFile);

			sprintf(clifs[ix].rt_name, "rt_%s", clifs[ix].name);

		}

	} else {
		syslog(LOG_ERR, "Error opening file to append: %s\n", RT_TARGET_FILE_PATH);
		exit(EXIT_FAILURE);
	}

	fclose(pFile);

	return 0;
}

int print_clif_info(struct clif clifs[])
{

	printf("%10s %15s %15s %15s \n", "iface", "ip", "gw", "rt");
	printf("------------------------------------------------------------------------------- \n");

	int ix;
	for(ix = 0; ix < CLIF_ARRAY_SIZE; ix++) {

		if(strlen(clifs[ix].name) == 0)
			break;

		printf("%10s %15s %15s %15s \n", clifs[ix].name, clifs[ix].ip, clifs[ix].gw, clifs[ix].rt_name);

	}

	printf("------------------------------------------------------------------------------- \n");

	return 0;

}

int define_rt_tables(struct clif clifs[])
{
	char *buff = malloc(15);
	char command[256];

	int ix;
	for(ix = 0; ix < CLIF_ARRAY_SIZE; ix++) {

		if(strlen(clifs[ix].name) == 0)
			break;

		sprintf(command, "/sbin/ip route del default via %s table %s", clifs[ix].gw, clifs[ix].rt_name);

		execute_command(command, &buff);

		sprintf(command, "/sbin/ip route add default via %s table %s", clifs[ix].gw, clifs[ix].rt_name);

		execute_command(command, &buff);

	}

	free(buff);

	return 0;
}

int define_rt_rules(struct clif clifs[])
{
	char *buff = malloc(15);
	char command[256];

	int ix;
	for(ix = 0; ix < CLIF_ARRAY_SIZE; ix++) {

		if(strlen(clifs[ix].name) == 0)
			break;

		sprintf(command, "/sbin/ip rule del from %s table %s", clifs[ix].ip, clifs[ix].rt_name);

		execute_command(command, &buff);

		sprintf(command, "/sbin/ip rule add from %s table %s", clifs[ix].ip, clifs[ix].rt_name);

		execute_command(command, &buff);

	}

	free(buff);

	return 0;
}

int delete_nexthop_route()
{
	char *buff = malloc(15);

	return execute_command("ip route del default scope global", &buff);

}

int balance_links(struct clif clifs[])
{
	char *buff = malloc(15);
	char command[256];
	char nexthop_template[64] = " nexthop via %s dev %s weight 1 ";
	char nexthop_buffer[70];

	strcpy(command, "/sbin/ip route add default scope global ");

	int ix;
	for(ix = 0; ix < CLIF_ARRAY_SIZE; ix++) {

		if(strlen(clifs[ix].name) == 0)
			break;

		sprintf(nexthop_buffer, nexthop_template, clifs[ix].gw, clifs[ix].name);

		strcat(command, nexthop_buffer);

	}

	return execute_command(command, &buff);

}

int route_flush_cache()
{
	char *buff = malloc(15);

	return execute_command("/sbin/ip route flush cache", &buff);
}

int load_clifs(struct clif clifs[])
{
	struct ifaddrs *ifaddr, *ifa;
	int family, s, n;
	char host[NI_MAXHOST];
	char *gw = malloc(15);
	int clif_index = 0;

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

	free(gw);
	freeifaddrs(ifaddr);

	return 0;
}
