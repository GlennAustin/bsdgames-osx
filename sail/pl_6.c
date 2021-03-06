/*-
 * Copyright (c) 1983, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * @(#)pl_6.c	8.1 (Berkeley) 5/31/93
 * $FreeBSD: src/games/sail/pl_6.c,v 1.5 1999/11/30 03:49:37 billf Exp $
 * $DragonFly: src/games/sail/pl_6.c,v 1.3 2006/09/03 17:33:13 pavalos Exp $
 */

#include "player.h"

static bool turned(void);

void
repair(void)
{
	char c;
	char *repairs;
	struct shipspecs *ptr = mc;
	int count;

#define FIX(x, m) (m - ptr->x > count \
	? (ptr->x += count, count = 0) : (count -= m - ptr->x, ptr->x = m))

	if (repaired || loaded || fired || changed || turned()) {
		Signal("No hands free to repair", NULL);
		return;
	}
	c = sgetch("Repair (hull, guns, rigging)? ", NULL, 1);
	switch (c) {
		case 'h':
			repairs = &mf->RH;
			break;
		case 'g':
			repairs = &mf->RG;
			break;
		case 'r':
			repairs = &mf->RR;
			break;
		default:
			Signal("Avast heaving!", NULL);
			return;
	}
	if (++*repairs >= 3) {
		count = 2;
		switch (c) {
		case 'h': {
			int max = ptr->guns/4;
			if (ptr->hull < max) {
				FIX(hull, max);
				Write(W_HULL, ms, ptr->hull, 0, 0, 0);
			}
			break;
			}
		case 'g':
			if (ptr->gunL < ptr->gunR) {
				int max = ptr->guns/5 - ptr->carL;
				if (ptr->gunL < max) {
					FIX(gunL, max);
					Write(W_GUNL, ms, ptr->gunL,
						ptr->carL, 0, 0);
				}
			} else {
				int max = ptr->guns/5 - ptr->carR;
				if (ptr->gunR < max) {
					FIX(gunR, max);
					Write(W_GUNR, ms, ptr->gunR,
						ptr->carR, 0, 0);
				}
			}
			break;
		case 'r':
#define X 2
			if (ptr->rig4 >= 0 && ptr->rig4 < X) {
				FIX(rig4, X);
				Write(W_RIG4, ms, ptr->rig4, 0, 0, 0);
			}
			if (count && ptr->rig3 < X) {
				FIX(rig3, X);
				Write(W_RIG3, ms, ptr->rig3, 0, 0, 0);
			}
			if (count && ptr->rig2 < X) {
				FIX(rig2, X);
				Write(W_RIG2, ms, ptr->rig2, 0, 0, 0);
			}
			if (count && ptr->rig1 < X) {
				FIX(rig1, X);
				Write(W_RIG1, ms, ptr->rig1, 0, 0, 0);
			}
			break;
		}
		if (count == 2) {
			Signal("Repairs completed.", NULL);
			*repairs = 2;
		} else {
			*repairs = 0;
			blockalarm();
			draw_stat();
			unblockalarm();
		}
	}
	blockalarm();
	draw_slot();
	unblockalarm();
	repaired = 1;
}

static bool
turned(void)
{
	char *p;

	for (p = movebuf; *p; p++)
		if (*p == 'r' || *p == 'l')
			return 1;
	return 0;
}

void
loadplayer(void)
{
	char c;
	int loadL, loadR, ready, load;

	if (!mc->crew3) {
		Signal("Out of crew", NULL);
		return;
	}
	loadL = mf->loadL;
	loadR = mf->loadR;
	if (!loadL && !loadR) {
		c = sgetch("Load which broadside (left or right)? ",
			NULL, 1);
		if (c == 'r')
			loadL = 1;
		else
			loadR = 1;
	}
	if ((!loadL && loadR) || (loadL && !loadR)) {
		c = sgetch("Reload with (round, double, chain, grape)? ",
			NULL, 1);
		switch (c) {
		case 'r':
			load = L_ROUND;
			ready = 0;
			break;
		case 'd':
			load = L_DOUBLE;
			ready = R_DOUBLE;
			break;
		case 'c':
			load = L_CHAIN;
			ready = 0;
			break;
		case 'g':
			load = L_GRAPE;
			ready = 0;
			break;
		default:
			Signal("Broadside not loaded.",
				NULL);
			return;
		}
		if (!loadR) {
			mf->loadR = load;
			mf->readyR = ready|R_LOADING;
		} else {
			mf->loadL = load;
			mf->readyL = ready|R_LOADING;
		}
		loaded = 1;
	}
}
