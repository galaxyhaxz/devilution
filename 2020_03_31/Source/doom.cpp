#include "diablo.h"

int doom_quest_time; // weak
int doom_stars_drawn; // weak
void *pDoomCel;
int doomflag; // weak
int DoomQuestState; // idb

int doom_get_frame_from_time()
{
	int result; // eax

	if ( DoomQuestState == 36001 )
		result = 31;
	else
		result = DoomQuestState / 1200;
	return result;
}

void doom_alloc_cel()
{
	pDoomCel = DiabloAllocPtr(229376);
}

void doom_cleanup()
{
	void *v0; // ecx

	v0 = pDoomCel;
	pDoomCel = 0;
	mem_free_dbg(v0);
}

void doom_load_graphics()
{
	if ( doom_quest_time == 31 )
	{
		strcpy(tempstr, "Items\\Map\\MapZDoom.CEL");
	}
	else if ( doom_quest_time >= 10 )
	{
		sprintf(tempstr, "Items\\Map\\MapZ00%i.CEL", doom_quest_time);
	}
	else
	{
		sprintf(tempstr, "Items\\Map\\MapZ000%i.CEL", doom_quest_time);
	}
	LoadFileWithMem(tempstr, pDoomCel);
}

void doom_init()
{
	int v0; // eax

	doomflag = 1;
	doom_alloc_cel();
	v0 = -(doom_get_frame_from_time() != 31);
	_LOBYTE(v0) = v0 & 0xE1;
	doom_quest_time = v0 + 31;
	doom_load_graphics();
}

void doom_close()
{
	if ( doomflag )
	{
		doomflag = 0;
		doom_cleanup();
	}
}

void doom_draw()
{
	if ( doomflag )
	{
		if ( doom_quest_time != 31 && ++doom_stars_drawn >= 5 )
		{
			doom_stars_drawn = 0;
			if ( ++doom_quest_time > doom_get_frame_from_time() )
				doom_quest_time = 0;
			doom_load_graphics();
		}
		CelDecodeOnly(64, 511, (BYTE *)pDoomCel, 1, 640);
	}
}
