/*
 * autoppp.c
 *
 *  Created on: Sep 18, 2014
 *      Author: lfranchi
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <getopt.h>
#include <limits.h>

#include "include/str_utils.h"
#include "include/sys_utils.h"
#include "include/cel_utils.h"

int main(int argc, char *argv[])
{
	char * buff = malloc(15);
	char command[256];
	char imsi_code[256];
	char operator_name[6];

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

	// CHECK IF IS THE COMMAND PORT (==0).
	//------------------------------------------------------------------------
	char * str1 = rindex(rindex(DEVICE_PATH, ':')+1, '.')+1;
	char * str2 = malloc(12);
	substring(0, 1, str1, str2);

	if (strcmp(str2, "0") != 0) {
		syslog(LOG_INFO, "Finished. Port is not 0. [%s]", str2);
		exit(EXIT_SUCCESS);
	}

	syslog(LOG_INFO, "Found port 0 to communicate.");
	//------------------------------------------------------------------------

	syslog(LOG_INFO, "Waiting for 5 seconds...");

	sleep(5);

	// EXECUTE CHMOD 666 ON DEVICE.
	//------------------------------------------------------------------------

	sprintf(command, "chmod 666 %s", DEVICE_NAME);

	if (execute_command(command, &buff) != 0) {
		exit(EXIT_FAILURE);
	}

	//------------------------------------------------------------------------

	// EXECUTE AT COMAND TO CHECK MODEM STATUS.
	//------------------------------------------------------------------------

	strcpy(command, "");
	strcpy(buff, "");

	sprintf(command, "echo AT | atinout - %s - | grep OK", DEVICE_NAME);

	if (execute_command(command, &buff) != 0) {
		exit(EXIT_FAILURE);
	}

	if(strcmp(buff, "") == 0) {
		syslog(LOG_ERR, "Not found atinout ou Modem not OK");
		exit(EXIT_FAILURE);
	}

	syslog(LOG_INFO, "--- %s", buff);

	//------------------------------------------------------------------------

	// EXECUTE AT+CIMI COMAND TO CHECK OPERATOR CODE.
	//------------------------------------------------------------------------

	strcpy(command, "");
	strcpy(buff, "");

	sprintf(command, "echo AT+CIMI | atinout - %s - | awk 'NR==2'", DEVICE_NAME);

	if (execute_command(command, &buff) != 0) {
		exit(EXIT_FAILURE);
	}

	syslog(LOG_INFO, "--- %s", buff);

	strcpy(imsi_code, buff);

	syslog(LOG_INFO, "Extracted %s IMSI Code.", imsi_code);

	//------------------------------------------------------------------------

	// EXECUTE AT+CIMI COMAND TO CHECK OPERATOR CODE.
	//------------------------------------------------------------------------

	get_operator_name(imsi_code, operator_name);

	syslog(LOG_INFO, "Extracted %s Operator.", operator_name);

	//------------------------------------------------------------------------

	// SAVE PPP FILES - chatscripts
	//------------------------------------------------------------------------

	char connection_name[128];
	strcpy(connection_name, operator_name);
	strcat(connection_name, "_");
	strcat(connection_name, rindex(DEVICE_NAME, '/')+1);

	char chatscripts_file_path[128] = "./ppp/carrierdb/carrier/";
	char chatscripts_file_full_path[256];

	char chatscripts_file_dest[128] = "/etc/chatscripts/";
	strcat(chatscripts_file_dest, connection_name);

	strcat(chatscripts_file_path, operator_name);
	strcat(chatscripts_file_path, "/chatscripts");

	realpath(chatscripts_file_path, chatscripts_file_full_path);

	copy_file(chatscripts_file_full_path, chatscripts_file_dest);

	syslog(LOG_INFO, "Saved %s to %s", chatscripts_file_full_path, chatscripts_file_dest);

	//------------------------------------------------------------------------

	// SAVE PPP FILES - peers
	//------------------------------------------------------------------------

	char peers_file_path[128] = "./ppp/carrierdb/carrier/";
	char peers_file_full_path[256];

	strcat(peers_file_path, operator_name);
	strcat(peers_file_path, "/peers");

	realpath(peers_file_path, peers_file_full_path);

	char peers_file_dest[128] = "/etc/ppp/peers/";
	strcat(peers_file_dest, connection_name);

	char peers_change_copy_command[256];

	char * dev_name_scaped;
	dev_name_scaped = str_replace(DEVICE_NAME, "/", "\\/");

	sprintf(peers_change_copy_command, "/bin/cat %s | sed 's/${connectionName}/%s/g' | sed 's/${deviceName}/%s/g' > %s", peers_file_full_path, connection_name, dev_name_scaped, peers_file_dest);

	execute_command(peers_change_copy_command, &buff);

	syslog(LOG_INFO, "Saved %s to %s", peers_file_path, peers_file_dest);

	//------------------------------------------------------------------------

	syslog(LOG_INFO, "Finished.");

	closelog();

	return 0;
}
