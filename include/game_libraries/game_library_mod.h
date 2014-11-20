
#ifndef __GAME_LIBRARY_MOD_H__
#define __GAME_LIBRARY_MOD_H__

#include <game_library.h>

class GameLibraryMod : public GameLibrary
{
	public:

		GameLibraryMod(s_library s) : GameLibrary(s)
		{
			this->init();
		}

		String get_name()
		{
			return String("mod");
		}
};

#endif
