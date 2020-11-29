#include "diablo.h"

// preallocated nodes, search is terminated after 300 nodes are visited
PATHNODE path_nodes[300];
// size of the pnode_tblptr stack
int gdwCurPathStep;
// the number of in-use nodes in path_nodes
int gdwCurNodes;
/* for reconstructing the path after the A* search is done. The longest
 * possible path is actually 24 steps, even though we can fit 25
 */
int pnode_vals[25];
// a linked list of all visited nodes
PATHNODE *pnode_ptr;
// a stack for recursively searching nodes
PATHNODE *pnode_tblptr[300];
// a linked list of the A* frontier, sorted by distance
PATHNODE *path_2_nodes;
PATHNODE path_unusednodes[300];

// for iterating over the 8 possible movement directions
const char pathxdir[8] = { -1, -1, 1, 1, -1, 0, 1, 0 };
const char pathydir[8] = { -1, 1, -1, 1, 0, -1, 0, 1 };

/* data */

/* each step direction is assigned a number like this:
 *       dx
 *     -1 0 1
 *     +-----
 *   -1|5 1 6
 * dy 0|2 0 3
 *    1|8 4 7
 */
char path_directions[9] = { 5, 1, 6, 2, 0, 3, 8, 4, 7 };

/* find the shortest path from (sx,sy) to (dx,dy), using PosOk(PosOkArg,x,y) to
 * check that each step is a valid position. Store the step directions (see
 * path_directions) in path, which must have room for 24 steps
 */
int FindPath(BOOL (*PosOk)(int, int, int), int PosOkArg, int sx, int sy, int dx, int dy, char *path)
{
	int i, steps;
	PATHNODE *pNext, *pPath;

	gdwCurNodes = 0;
	path_2_nodes = path_new_step();
	pnode_ptr = path_new_step();
	gdwCurPathStep = 0;

	pNext = path_new_step();
	pNext->g = 0;
	pNext->h = path_get_h_cost(sx, sy, dx, dy);
	pNext->f = pNext->h + pNext->g;
	pNext->x = sx;
	pNext->y = sy;
	path_2_nodes->NextNode = pNext;

	while(pPath = GetNextPath()) {
		if(pPath->x == dx && pPath->y == dy) {
			pNext = pPath;
			steps = 0;
			while(pNext->Parent != NULL && steps < 25) {
				pnode_vals[steps] = path_directions[3 * (pNext->y - pNext->Parent->y) + 3 - pNext->Parent->x + 1 + pNext->x];
				steps++;
				pNext = pNext->Parent;
			}
			if(steps == 25) {
				return 0;
			}
			for(i = 0; i < steps; i++) {
				path[i] = pnode_vals[steps - i - 1];
			}
			return i;
		}
		if(!path_get_path(PosOk, PosOkArg, pPath, dx, dy)) {
			return 0;
		}
	}

	return 0;
}

/* heuristic, estimated cost from (sx,sy) to (dx,dy) */
int path_get_h_cost(int x1, int y1, int x2, int y2)
{
	int dx, dy, minc, maxc;

	dx = abs(x1 - x2);
	dy = abs(y1 - y2);

	minc = dx < dy ? dx : dy;
	maxc = dx > dy ? dx : dy;

	return 2 * (minc + maxc);
}

/* return 2 if pPath is horizontally/vertically aligned with (dx,dy), else 3
 *
 * This approximates that diagonal movement on a square grid should have a cost
 * of sqrt(2). That's approximately 1.5, so they multiply all step costs by 2,
 * except diagonal steps which are times 3
 */
int path_check_equal(PATHNODE *pPath, int dx, int dy)
{
	if(pPath->x == dx || pPath->y == dy) {
		return 2;
	}

	return 3;
}

/* get the next node on the A* frontier to explore (estimated to be closest to
 * the goal), mark it as visited, and return it
 */
PATHNODE *GetNextPath()
{
	PATHNODE *pNext;

	if(path_2_nodes->NextNode == NULL) {
		return NULL;
	}

	pNext = path_2_nodes->NextNode;
	path_2_nodes->NextNode = pNext->NextNode;
	pNext->NextNode = pnode_ptr->NextNode;
	pnode_ptr->NextNode = pNext;
	return pNext;
}

/* check if stepping from pPath to (dx,dy) cuts a corner. If you step from A to
 * B, both Xs need to be clear:
 *
 *  AX
 *  XB
 *
 *  return true if step is allowed
 */
BOOL path_solid_pieces(PATHNODE *pPath, int dx, int dy)
{
	BOOL rv;

	rv = TRUE;

	switch(path_directions[3 * (dy - pPath->y) + 3 - pPath->x + 1 + dx]) {
	case 5:
		rv = !nSolidTable[dPiece[dx][dy + 1]] && !nSolidTable[dPiece[dx + 1][dy]];
		break;
	case 6:
		rv = !nSolidTable[dPiece[dx][dy + 1]] && !nSolidTable[dPiece[dx - 1][dy]];
		break;
	case 7:
		rv = !nSolidTable[dPiece[dx][dy - 1]] && !nSolidTable[dPiece[dx - 1][dy]];
		break;
	case 8:
		rv = !nSolidTable[dPiece[dx + 1][dy]] && !nSolidTable[dPiece[dx][dy - 1]];
		break;
	}

	return rv;
}

/* perform a single step of A* bread-first search by trying to step in every
 * possible direction from pPath with goal (x,y). Check each step with PosOk
 *
 * return 0 if we ran out of preallocated nodes to use, else 1
 */
BOOL path_get_path(BOOL (*PosOk)(int, int, int), int PosOkArg, PATHNODE *pPath, int x, int y)
{
	int i, dx, dy;
	BOOL ok;

	for(i = 0; i < 8; i++) {
		dx = pPath->x + pathxdir[i];
		dy = pPath->y + pathydir[i];
		ok = PosOk(PosOkArg, dx, dy);
		if(ok && path_solid_pieces(pPath, dx, dy) || !ok && dx == x && dy == y) {
			if(!path_parent_path(pPath, dx, dy, x, y)) {
				return FALSE;
			}
		}
	}

	return TRUE;
}

/* add a step from pPath to (dx,dy), return 1 if successful, and update the
 * frontier/visited nodes accordingly
 *
 * return 1 if step successfully added, 0 if we ran out of nodes to use
 */
BOOL path_parent_path(PATHNODE *pPath, int dx, int dy, int sx, int sy)
{
	int i, steps;
	PATHNODE *pNext, *pNew;

	steps = pPath->g + path_check_equal(pPath, dx, dy);

	if(pNext = path_get_node1(dx, dy)) {
		for(i = 0; i < 8; i++) {
			if(pPath->Child[i] == NULL) {
				break;
			}
		}
		pPath->Child[i] = pNext;
		if(steps < pNext->g && path_solid_pieces(pPath, dx, dy)) {
			pNext->Parent = pPath;
			pNext->g = steps;
			pNext->f = steps + pNext->h;
		}
	} else if(pNext = path_get_node2(dx, dy)) {
		for(i = 0; i < 8; i++) {
			if(pPath->Child[i] == NULL) {
				break;
			}
		}
		pPath->Child[i] = pNext;
		if(steps < pNext->g && path_solid_pieces(pPath, dx, dy)) {
			pNext->Parent = pPath;
			pNext->g = steps;
			pNext->f = steps + pNext->h;
			path_set_coords(pNext);
		}
	} else {
		pNew = path_new_step();
		if(pNew == NULL) {
			return FALSE;
		}
		pNew->Parent = pPath;
		pNew->g = steps;
		pNew->h = path_get_h_cost(dx, dy, sx, sy);
		pNew->f = steps + pNew->h;
		pNew->x = dx;
		pNew->y = dy;
		path_next_node(pNew);
		for(i = 0; i < 8; i++) {
			if(pPath->Child[i] == NULL) {
				break;
			}
		}
		pPath->Child[i] = pNew;
	}

	return TRUE;
}

/* return a node for (dx,dy) on the frontier, or NULL if not found */
PATHNODE *path_get_node1(int dx, int dy)
{
	PATHNODE *pPath;

	pPath = path_2_nodes->NextNode;
	while(pPath != NULL) {
		if(pPath->x == dx && pPath->y == dy) {
			return pPath;
		}
		pPath = pPath->NextNode;
	}

	return NULL;
}

/* return a node for (dx,dy) if it was visited, or NULL if not found */
PATHNODE *path_get_node2(int dx, int dy)
{
	PATHNODE *pPath;

	pPath = pnode_ptr->NextNode;
	while(pPath != NULL) {
		if(pPath->x == dx && pPath->y == dy) {
			return pPath;
		}
		pPath = pPath->NextNode;
	}

	return NULL;
}

/* insert pPath into the frontier (keeping the frontier sorted by total
 * distance) */
void path_next_node(PATHNODE *pPath)
{
	int f;
	PATHNODE *pOld, *pNext;

	if(path_2_nodes->NextNode == NULL) {
		path_2_nodes->NextNode = pPath;
		return;
	}

	f = pPath->f;
	pOld = path_2_nodes;
	pNext = path_2_nodes->NextNode;

	while(pNext != NULL && pNext->f < f) {
		pOld = pNext;
		pNext = pNext->NextNode;
	}

	pPath->NextNode = pNext;
	pOld->NextNode = pPath;
}

/* update all path costs using depth-first search starting at pPath */
void path_set_coords(PATHNODE *pPath)
{
	int i;
	PATHNODE *pOld, *pCur;

	path_push_active_step(pPath);

	while(gdwCurPathStep != 0) {
		pOld = path_pop_active_step();
		for(i = 0; i < 8; i++) {
			pCur = pOld->Child[i];
			if(pCur == NULL) {
				break;
			}
			if(pOld->g + path_check_equal(pOld, pCur->x, pCur->y) < pCur->g) {
				if(path_solid_pieces(pOld, pCur->x, pCur->y)) {
					pCur->Parent = pOld;
					pCur->g = pOld->g + path_check_equal(pOld, pCur->x, pCur->y);
					pCur->f = pCur->h + pCur->g;
					path_push_active_step(pCur);
				}
			}
		}
	}
}

/* push pPath onto the pnode_tblptr stack */
void path_push_active_step(PATHNODE *pPath)
{
	pnode_tblptr[gdwCurPathStep] = pPath;
	gdwCurPathStep++;
}

/* pop and return a node from the pnode_tblptr stack */
PATHNODE *path_pop_active_step()
{
	gdwCurPathStep--;
	return pnode_tblptr[gdwCurPathStep];
}

/* zero one of the preallocated nodes and return a pointer to it, or NULL if
 * none are available */
PATHNODE *path_new_step()
{
	PATHNODE *pPath;

	if(gdwCurNodes == 300) {
		return NULL;
	}

	pPath = &path_nodes[gdwCurNodes];
	gdwCurNodes++;
	memset(pPath, 0, sizeof(*pPath));
	return pPath;
}
