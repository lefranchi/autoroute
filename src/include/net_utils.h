/*
 * net_utils.h
 *
 *  Created on: Sep 16, 2014
 *      Author: lfranchi
 */

#ifndef NET_UTILS_H_
#define NET_UTILS_H_

#define RT_SOURCE_FILE_PATH "./conf/rt_tables"
#define RT_TARGET_FILE_PATH "/etc/iproute2/rt_tables"

int is_ifa_enabled(char* ifa_name);

int find_gateway(char*, char**);

int init_rt_tables_file();

#endif /* NET_UTILS_H_ */
