/*
 * autoroute_utils.h
 *
 *  Created on: Sep 12, 2014
 *      Author: lfranchi
 */

#include "../include/autoroute_utils.h"

#include <stdio.h>
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

int find_gateway(char* ifname, char** ifgw)
{
	char command[80] = "";

	strcat(command, "/sbin/route -n | grep ");
	strcat(command, ifname);
	strcat(command, " | grep '^0.0.0.0' | awk '{ print $2 }'");

	int ret_value = execute_command(command, ifgw);

	if (strcmp(*ifgw, "") == 0) {

		strcpy(command, "");

		strcat(command, "/sbin/ifconfig ");
		strcat(command, ifname);
		strcat(command, " | grep P-t-P | awk '{print $3}' | awk -F: '{print $2}'");

		ret_value = execute_command(command, ifgw);
	}

	return ret_value;
}


int execute_command(char* command, char** ret_val) {
	FILE *fp;
	char buff[2048];

	fp = popen(command, "r");
	if (fp == NULL) {
		printf("Failed to run command %s: \n", command);
		return 0;
	}

	if (fgets(buff, sizeof(buff) - 1, fp) != NULL) {
		strncpy(*ret_val, buff, strlen(buff) - 1);
	} else {
		strcpy(*ret_val, "");
	}

	pclose(fp);

	return 1;
}
