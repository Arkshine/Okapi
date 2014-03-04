
#ifndef __GAME_LIBRARY_MOD_H__
#define __GAME_LIBRARY_MOD_H__

#include <game_library.h>

char Exported[][50] = 
{
	"GiveFnptrsToDll",
	"GetEntityAPI",
	"DllEntryPoint",
	"aiscripted_sequence",
	"ammo_buckshot"
	"bmortar",
	"button_target",
	"cine_blood",
	"cycler_prdroid",
	"cycler_sprite",
	"cycler_weapon",
	"cycler_wreckage",
	"env_beam",
	"env_beverage",
	"env_blood",
	"env_bubbles",
	"env_debris",
	"env_funnel",
	"env_global",
	"env_laser",
	"env_lightning",
	"env_message",
	"env_shake",
	"env_smoker",
	"env_sound",
	"env_spark",
	"fireanddie",
	"func_breakable",
	"func_button",
	"func_conveyor",
	"func_door",
	"func_friction",
	"func_guntarget",
	"func_healthcharger",
	"func_illusionary",
	"func_ladder",
	"func_mortar_field",
	"func_pendulum",
	"func_plat",
	"func_pushable",
	"func_recharge",
	"func_rot_button",
	"func_rotating",
	"func_tank",
	"func_trackautochange",
	"func_trackchange",
	"func_tracktrain",
	"func_train",
	"func_traincontrols",
	"func_wall",
	"func_wall_toggle",
	"func_water",
	"grenade",
	"info_null",
	"infodecal",
	"item_airtank",
	"item_antidote",
	"item_battery",
	"item_healthkit",
	"item_longjump",
	"item_security",
	"item_sodacan",
	"item_suit",
	"light",
	"light_environment",
	"light_spot",
	"multisource",
	"path_corner",
	"path_track",
	"player",
	"player_loadsaved",
	"player_weaponstrip",
	"soundent",
	"speaker",
	"target_cdaudio",
	"test_effect",
	"trigger",
	"trigger_auto",
	"trigger_autosave",
	"trigger_camera",
	"trigger_cdaudio",
	"trigger_changelevel",
	"trigger_changetarget",
	"trigger_counter",
	"trigger_gravity",
	"trigger_hurt",
	"trigger_multiple",
	"trigger_once",
	"trigger_push",
	"trigger_relay",
	"trigger_teleport",
	"trigger_transition",
	"world_items",
	"worldspawn",
};

class GameLibraryMod : public GameLibrary
{
	protected :

		CVector<int> get_base_offsets()
		{
			CVector<int> base_offsets;

			for(int i=0;i<sizeof(DLL_FUNCTIONS)/sizeof(void*);i++)
			{
				int offset = (int)(((void**)(gpGamedllFuncs->dllapi_table))[i]) - (int)this->address;

				base_offsets.push_back(offset);
			}

			for(int k=0;k<ARRAYSIZE(Exported);k++)
			{
				int offset = (int)find_function(this,Exported[k]) - (int)this->address;

				base_offsets.push_back(offset);
			}

			return base_offsets;
		}

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
