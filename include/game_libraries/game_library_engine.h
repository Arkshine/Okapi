
#ifndef __GAME_LIBRARY_ENGINE_H__
#define __GAME_LIBRARY_ENGINE_H__

#include <game_library.h>

class GameLibraryEngine : public GameLibrary
{
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
