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

#include <s_library.h>

#if defined __linux__

	#include <dlfcn.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <unistd.h>
	#include <string.h>
	#include <stdio.h>
	#include <stdlib.h>

#else

	#define WIN32_LEAN_AND_MEAN

	#define NOWINRES
	#define NOSERVICE
	#define NOMCX
	#define NOIME
	#define NOSOUND
	#define NOCOMM
	#define NOKANJI
	#define NORPC
	#define NOPROXYSTUB
	#define NOIMAGE
	#define NOTAPE

	#define PSAPI_VERSION 1

	#pragma comment( lib, "Psapi.lib" ) 
	#pragma comment( lib, "Kernel32.lib" ) 

	#include <windows.h>
	#include <string.h>
	#include <climits>
	#include <psapi.h>

#endif

#if defined __linux__

	long get_length(void *baseAddress)
	{
			pid_t pid = getpid();
			char file[255];
			char buffer[2048];
			snprintf(file, sizeof(file)-1, "/proc/%d/maps", pid);
			FILE *fp = fopen(file, "rt");
			if (fp)
			{
				long length = 0;

				void *start=NULL;
				void *end=NULL;

				while (!feof(fp))
				{
					fgets(buffer, sizeof(buffer)-1, fp);           
					#if defined AMD64
						sscanf(buffer, "%Lx-%Lx", &start, &end);
					#else
						sscanf(buffer, "%lx-%lx", &start, &end);
					#endif
					if(start == baseAddress)
					{
						length = (unsigned long)end  - (unsigned long)start;

						char ignore[100];
						int value;

						while(!feof(fp))
						{
							fgets(buffer, sizeof(buffer)-1, fp);
							#if defined AMD64
								sscanf(buffer, "%Lx-%Lx %s %s %s %d", &start, &end, ignore, ignore, ignore, &value);
							#else
								sscanf(buffer, "%lx-%lx %s %s %s %d", &start, &end, ignore, ignore ,ignore, &value);
							#endif

							if(!value)
							{       
								break;
							}
							else
							{
								length += (unsigned long)end  - (unsigned long)start;
							}
						}
					   
						break;
					}
				}

				fclose(fp);

			return length;
		}

		return 0;
	}

	s_library* create_library(void* address)
	{
		s_library* library = NULL;
		Dl_info info;

		if(dladdr(address,&info))
		{
			library = new s_library;

			library->address = (void*) info.dli_fbase;
			library->length = get_length((void*) info.dli_fbase);
			library->handle = dlopen(info.dli_fname, RTLD_NOW);
		   
			return library;
		}

		return NULL;
	}

	char* get_address_symbol(void* address)
	{
		Dl_info info;

		if(dladdr(address,&info))
		{
			return (char*)info.dli_sname;
		}

		return "";
	}

#else

	s_library* create_library(void* address)
	{
		HMODULE module;

		s_library* library = NULL;

		if(GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,(LPCSTR)address,&module))
		{
			HANDLE process =  GetCurrentProcess();
			_MODULEINFO moduleInfo;

			if(GetModuleInformation(process,module,&moduleInfo,sizeof moduleInfo))
			{
				CloseHandle(process);

				library = new s_library;

				library->address = (void*) moduleInfo.lpBaseOfDll;
				library->length = moduleInfo.SizeOfImage;
				library->handle = module;


				return library;
			}
		}

		return library;
	}

	
	char* get_address_symbol(void* address)
	{
		return "";
	}

#endif
	
void* find_function(s_library* library,char* functionName)
{
	#if defined __linux__
		return dlsym(library->handle,functionName);
	#else

		return GetProcAddress((HMODULE)library->handle,functionName);
	#endif
}
