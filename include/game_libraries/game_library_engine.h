
#ifndef __GAME_LIBRARY_ENGINE_H__
#define __GAME_LIBRARY_ENGINE_H__

#include <game_library.h>

char ExportedEng[][50] = 
{
	"F",
	"DllEntryPoint"
};

class GameLibraryEngine : public GameLibrary
{
	protected :

		CVector<int> get_base_offsets()
		{
			CVector<int> base_offsets;

			for(int i=0;i<sizeof(g_engfuncs)/sizeof(void*);i++)
			{
				int offset = (int)(((void**)(&g_engfuncs))[i]) - (int)this->address;
				base_offsets.push_back(offset);
			}

			for(int k=0;k<ARRAYSIZE(ExportedEng);k++)
			{
				int offset_w = (int)find_function(this,ExportedEng[k]) - (int)this->address;
				base_offsets.push_back(offset_w);
			}

			return base_offsets;
		}

	public:
		GameLibraryEngine(s_library s) : GameLibrary(s)
		{
			this->init();
		}

		String get_name()
		{
			return String("engine");
		}

};

#endif
