/*
 * net_utils.c
 *
 *  Created on: Sep 16, 2014
 *      Author: lfranchi
 */

#include "../include/net_utils.h"
#include "../include/sys_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_ifa_enabled(char* ifa_name) {

	int allowed_if_names_size = 3;
	char *allowed_if_names[3] = { "eth", "wlan", "ppp" };

	int ix;
	for (ix = 0; ix < allowed_if_names_size; ix++) {

		if (strstr(ifa_name, allowed_if_names[ix]) != 0)
			return 1;

	}

	return 0;
}

int find_gateway(char* ifname, char** ifgw) {
	char command[80] = "";

	strcat(command, "/sbin/route -n | grep ");
	strcat(command, ifname);
	strcat(command, " | grep '^0.0.0.0' | awk '{ print $2 }'");

	int ret_value = execute_command(command, ifgw);

	if (strcmp(*ifgw, "") == 0) {

		strcpy(command, "");

		strcat(command, "/sbin/ifconfig ");
		strcat(command, ifname);
		strcat(command,
				" | grep P-t-P | awk '{print $3}' | awk -F: '{print $2}'");

		ret_value = execute_command(command, ifgw);
	}

	return ret_value;
}

int init_rt_tables_file()
{
	char rt_default_file_path[100];
	realpath(RT_SOURCE_FILE_PATH, rt_default_file_path);
	copy_file(rt_default_file_path, RT_TARGET_FILE_PATH);

	return 0;
}
