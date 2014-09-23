/*
 * autoppp.c
 *
 *  Created on: Sep 18, 2014
 *      Author: lfranchi
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <getopt.h>

#include "include/str_utils.h"

int main(int argc, char *argv[])
{

	openlog("autoppp", LOG_PID|LOG_CONS, LOG_USER);

	syslog(LOG_INFO, "Initializing ... ");

	if(argc != 4) {
		syslog(LOG_ALERT, "Finishing because no suficient arguments.");
		exit(EXIT_FAILURE);
	}

#define	ACTION 		argv[optind + 0]
#define	DEVICE_NAME argv[optind + 1]
#define DEVICE_PATH	argv[optind + 2]

	syslog(LOG_INFO, "Running for %s %s %s\n", ACTION, DEVICE_NAME, DEVICE_PATH);


	// Verify por == 0 to execute commands.
	char * str1 = rindex(rindex(DEVICE_PATH, ':')+1, '.')+1;
	char * str2 = malloc(12);
	substring(0, 1, str1, str2);

	if (strcmp(str2, "0") != 0) {
		syslog(LOG_INFO, "Finished. Port is not 0. [%s]", str2);
		exit(EXIT_SUCCESS);
	}

	syslog(LOG_INFO, "Found port 0 to communicate.");


	syslog(LOG_INFO, "Finished.");

	closelog();

	return 0;
}
