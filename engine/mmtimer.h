/*
VERGE 2.5+j (AKA V2k+j) -  A video game creation engine
Copyright (C) 1998-2000  Benjamin Eirich (AKA vecna), et al
Please see authors.txt for a complete list of contributing authors.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
/*
    FILE : TIMER.H
    
    DOS-based Timer Interrupt functions
    Includes defines for TIMER.C and VTIMER.C
*/

#ifndef TIMER_H
#define TIMER_H

#include "mmtypes.h"

#define VT_VRSYNC    1
#define VT_CALLOLD   2

#ifdef __cplusplus
extern "C" {
#endif

// Defined in TIMER.C

void Timer_Exit(void);
void Timer_Init(int rate, void (*handler)(void));
void Timer_SetSpeed(UWORD rate);
void Timer_Start(void);
void Timer_Stop(void);


// Defined in VTIMER.C
void VT_Exit(void);
void VT_Init(void);
SWORD VT_Alloc(void);
void VT_Free(SWORD handle);
void VT_SetHandler(SWORD handle,void (*handler)(void));
void VT_SetSpeed(SWORD handle,SLONG speed);
void VT_SetBPM(SWORD handle,UBYTE bpm);
void VT_Start(SWORD handle);
void VT_Stop(SWORD handle);
void VT_Mode(UBYTE flags);
void VT_V0Handler(void (*handler)(void));
void VT_V1Handler(void (*handler)(void));

#ifdef __cplusplus
}
#endif


extern volatile int framecount;
void framecounter(void);

#endif
