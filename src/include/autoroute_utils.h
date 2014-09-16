/*
 * autoroute_utils.h
 *
 *  Created on: Sep 12, 2014
 *      Author: lfranchi
 */

#ifndef AUTOROUTE_UTILS_H_
#define AUTOROUTE_UTILS_H_

int is_ifa_enabled(char* ifa_name);

int find_gateway(char*, char**);

int execute_command(char*, char**);

#endif /* AUTOROUTE_UTILS_H_ */
