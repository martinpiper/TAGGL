/***************************************************************************
						  syscall.c  -  description
							 -------------------
	begin				: Wed Sep 26 2001
	copyright			: (C) 2001 CEA and Université Paris XI Orsay
	author			   : Gilles Mouchard
	email				: gilles.mouchard@lri.fr, gilles.mouchard@cea.fr
 ***************************************************************************/

/***************************************************************************
 *																		 *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or	 *
 *   (at your option) any later version.								   *
 *																		 *
 ***************************************************************************/

/* The file is modified by Wei Qin for arm target.
 * Added mmap syscall interpretation.
 */
/*
 * Modified by Chris Han for better Linux compatibility
 */

#ifdef linux
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#define EINVAL 22
#include "misc.h"
#include "syscall.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "armemul.h"
#include "armmmu.h"


static char *arm_get_syscall_name(int num)
{
	return "?";
}

#define SYSCALL_NUM ((inst<<8)>>8)

void do_syscall(armulator *emu, arm_inst_t inst)
{
	int syscall_num;
	bool ret = false;
	
	syscall_num	= SYSCALL_NUM & 0xff;
	if (emu->is_verbose())
		fprintf(stderr, "got a system call (number : %u, name : %s)\n", syscall_num, arm_get_syscall_name(syscall_num));
		
	fflush(stderr);

	
	if(!ret)
	{
		if (emu->is_verbose())
			fprintf(stderr, "Warning : system call returns an error (number : %u, name : %s)\n", syscall_num, arm_get_syscall_name(syscall_num));
	}
	else
	{
	}
}

