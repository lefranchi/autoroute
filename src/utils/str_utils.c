/*
 * str_utils.c
 *
 *  Created on: Sep 23, 2014
 *      Author: lfranchi
 */

#include "../include/str_utils.h"

void substring(int start, int stop, const char *text, char *dst)
{
   sprintf(dst, "%.*s", stop - start, &text[start]);
}
