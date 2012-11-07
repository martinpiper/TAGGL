/*************************************************************************
    Copyright (C) 2002 - 2007 Wei Qin
    See file COPYING for more information.

    This program is free software; you can redistribute it and/or modify    
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*************************************************************************/

#include <fcntl.h>
#include <cstdlib>
#include <cstdio>
#include "bittypes.h"

uint64_t ato_uint64(char *input)
{
	char *p = input;
	uint64_t val = 0;
	while (*p!=0) {
		val = val*10+(*p-'0');
		p++;
	}
	return val;
}

void dump_int64(uint64_t value, FILE *stream)
{
	char buffer[32];
	char *p = buffer + sizeof(buffer) - 1;
	
	if (value==0) {
		fputs("0", stream);
	}
	else {
		*p = 0;
		while (value > 0)
		{
			*--p = '0' + (value % 10);
			value /= 10;
		}
		fputs(p, stream);
	}
}

void dump_int64_smart(uint64_t value, FILE *stream)
{
	uint64_t val = value;
	char *c = "";

	if(value > 1000000000)
	{
		val = value / 1000000000;
		c = "G";
	}
	else if(value > 1000000)
	{
		val = value / 1000000;
		c = "M";
	}
	else if(value > 1000)
	{
		val = value / 1000;
		c = "K";
	}

	dump_int64(val, stream);
	fputs(c, stream);
}

void term_exit(void)
{
}

void term_init(void)
{
}

