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

void substring(int start, int stop, const char *text, char *dst)
{
   sprintf(dst, "%.*s", stop - start, &text[start]);
}

int main(int argc, char *argv[])
{

	openlog("autoppp", LOG_PID|LOG_CONS, LOG_USER);

	syslog(LOG_INFO, "Initializing ... ");

	if(argc != 4) {
		syslog(LOG_ALERT, "Finishing because no suficient arguments.");
		exit(EXIT_FAILURE);
	}

	char action[5];
	char device_name[15];
	char device_path[150];

	strcpy(action, argv[1]);
	strcpy(device_name, argv[2]);
	strcpy(device_path, argv[3]);

	syslog(LOG_INFO, "Running for %s %s %s\n", action, device_name, device_path);


	// Verify por == 0 to execute commands.
	char * str1 = rindex(rindex(device_path, ':')+1, '.')+1;
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
