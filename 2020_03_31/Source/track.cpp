#include "diablo.h"
#include "../3rdParty/Storm/Source/storm.h"

BOOL sgbIsScrolling; // weak
int sgdwLastWalk; // weak
_bool sgbIsWalking; // weak

void track_process()
{
	int v0; // eax
	DWORD v1; // eax

	if ( sgbIsWalking )
	{
		if ( cursmx >= 0 && cursmx < 111 && cursmy >= 0 && cursmy < 111 )
		{
			v0 = myplr;
			if ( (plr[myplr]._pVar8 > 6 || plr[v0]._pmode == PM_STAND)
			  && (cursmx != plr[v0]._ptargx || cursmy != plr[v0]._ptargy) )
			{
				v1 = GetTickCount();
				if ( v1 - sgdwLastWalk >= 300 )
				{
					sgdwLastWalk = v1;
					NetSendCmdLoc(1u, CMD_WALKXY, cursmx, cursmy);
					if ( !sgbIsScrolling )
						sgbIsScrolling = 1;
				}
			}
		}
	}
}

void track_repeat_walk(BOOL rep)
{
	if ( sgbIsWalking != rep )
	{
		sgbIsWalking = rep;
		if ( rep )
		{
			sgbIsScrolling = 0;
			sgdwLastWalk = GetTickCount() - 50;
			NetSendCmdLoc(1u, CMD_WALKXY, cursmx, cursmy);
		}
		else if ( sgbIsScrolling )
		{
			sgbIsScrolling = 0;
		}
	}
}

BOOL track_isscrolling()
{
	return sgbIsScrolling;
}
