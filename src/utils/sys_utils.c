/*
 * sys_utils.h
 *
 *  Created on: Sep 12, 2014
 *      Author: lfranchi
 */

#include "../include/sys_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int execute_command(char* command, char** ret_val)
{
	FILE *fp;
	char buff[2048];

	fp = popen(command, "r");
	if (fp == NULL) {
		printf("Failed to run command %s: \n", command);
		return 0;
	} else {
		printf("Executing: %s \n", command);
	}

	if (fgets(buff, sizeof(buff) - 1, fp) != NULL) {
		strncpy(*ret_val, buff, strlen(buff) - 1);
	} else {
		strcpy(*ret_val, "");
	}

	pclose(fp);

	return 0;
}

int copy_file(char *source_file, char *target_file)
{
	char ch;
	FILE * source, *target;

	source = fopen(source_file, "r");
	target = fopen(target_file, "w");

	if (source == NULL || target == NULL) {
		printf("Source or Target file is null...\n");

		if (target == NULL)
			printf("Execute sudo chmod 777 %s.\n", target_file);

		exit(EXIT_FAILURE);
	}

	while ((ch = fgetc(source)) != EOF)
		fputc(ch, target);

	fclose(source);
	fclose(target);

	return 0;
}

