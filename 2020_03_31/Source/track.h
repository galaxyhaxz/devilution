//HEADER_GOES_HERE
#ifndef __TRACK_H__
#define __TRACK_H__

extern BOOL sgbIsScrolling; // weak
extern int sgdwLastWalk; // weak
extern _bool sgbIsWalking; // weak

void track_process();
void track_repeat_walk(BOOL rep);
BOOL track_isscrolling();

#endif /* __TRACK_H__ */
