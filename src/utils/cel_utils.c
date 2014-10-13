/*
 * cel_utils.c
 *
 *  Created on: Oct 13, 2014
 *      Author: lfranchi
 */

#include "../include/cel_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/str_utils.h"

int get_operator_name(char* imsi_code, char* operator_name) {

	char imsi_digits[5];
	char operator_imsi_file[128];

	substring(0, 5, imsi_code, imsi_digits);

	realpath(OPERATOR_IMSI_FILE_PATH, operator_imsi_file);

	FILE *file = fopen(operator_imsi_file, "r");

	if (file != NULL) {
		char line[128];
		while (fgets(line, sizeof line, file) != NULL) {

			if(strstr(line, imsi_digits) != NULL) {
				sscanf(line, "%32[^=]=%s", imsi_digits, operator_name);
			}

		}
		fclose(file);
	} else {
		perror(operator_imsi_file);
		return 1;
	}

	return 0;
}
