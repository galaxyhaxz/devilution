//HEADER_GOES_HERE
#ifndef __PORTAL_H__
#define __PORTAL_H__

extern PortalStruct portal[MAXPORTAL];
extern int portalindex;
// int END_portalstruct; // weak

void InitPortals();
void SetPortalStats(int i, int o, int x, int y, int lvl, int lvltype);
void AddWarpMissile(int i, int x, int y);
void SyncPortals();
void AddInTownPortal(int i);
void ActivatePortal(int i, int x, int y, int lvl, int lvltype, int sp);
void DeactivatePortal(int i);
_bool PortalOnLevel(int i);
void RemovePortalMissile(int id);
void SetCurrentPortal(int p);
void GetPortalLevel();
void GetPortalLvlPos();
_bool PosOkPortal(int lvl, int x, int y);

/* rdata */
extern int WarpDropX[MAXPORTAL];
extern int WarpDropY[MAXPORTAL];

#endif /* __PORTAL_H__ */
