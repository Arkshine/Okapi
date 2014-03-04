
#ifndef __WORK_H__
#define __WORK_H__

#include <my_util.h>
#include <stdio.h>
#include "amxxmodule.h"

void clean_debug_file();
void print(char* format,...);
void patch_printf();

extern "C" void __cxa_pure_virtual();

#endif