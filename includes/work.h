//
// AMX Mod X, based on AMX Mod by Aleksander Naszko ("OLO").
// Copyright (C) The AMX Mod X Development Team.
//
// This software is licensed under the GNU General Public License, version 3 or higher.
// Additional exceptions apply. For full license details, see LICENSE.txt or visit:
//     https://alliedmods.net/amxmodx-license

//
// Okapi Module
//

#ifndef __WORK_H__
#define __WORK_H__

#include <my_util.h>
#include <stdio.h>
#include "amxxmodule.h"

void clean_debug_file();
void print(char* format, ...);
void patch_printf();

extern "C" void __cxa_pure_virtual();

#endif // __WORK_H__
