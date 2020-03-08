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

#ifndef MTYPES_H
#define MTYPES_H

/*
	MikMod atomic types:
	====================
*/


#ifdef __OS2__

typedef signed char     SBYTE;          /* has to be 1 byte signed */
typedef unsigned char   UBYTE;          /* has to be 1 byte unsigned */
typedef short           SWORD;          /* has to be 2 bytes signed */
typedef unsigned short  UWORD;          /* has to be 2 bytes unsigned */
typedef long            SLONG;          /* has to be 4 bytes signed */
/* ULONG and BOOL are already defined in OS2.H */

#elif defined(__alpha)

typedef char            SBYTE;          /* has to be 1 byte signed */
typedef unsigned char   UBYTE;          /* has to be 1 byte unsigned */
typedef short           SWORD;          /* has to be 2 bytes signed */
typedef unsigned short  UWORD;          /* has to be 2 bytes unsigned */
/* long is 8 bytes on dec alpha - RCA */
typedef int             SLONG;          /* has to be 4 bytes signed */
typedef unsigned int    ULONG;          /* has to be 4 bytes unsigned */
typedef int             BOOL;           /* doesn't matter.. 0=FALSE, <>0 true */

#else

typedef char            SBYTE;          /* has to be 1 byte signed */
typedef unsigned char   UBYTE;          /* has to be 1 byte unsigned */
typedef short           SWORD;          /* has to be 2 bytes signed */
typedef unsigned short  UWORD;          /* has to be 2 bytes unsigned */
typedef long            SLONG;          /* has to be 4 bytes signed */
typedef unsigned long   ULONG;          /* has to be 4 bytes unsigned */
typedef int             BOOL;           /* doesn't matter.. 0=FALSE, <>0 true */

#endif


#ifdef __OS2__
#define INCL_DOS
#define INCL_MCIOS2
#define INCL_MMIOOS2
#include <os2.h>
#include <os2me.h>
#include <mmio.h>
#endif


#ifdef __WATCOMC__
#define inportb(x) inp(x)
#define outportb(x,y) outp(x,y)
#define inport(x) inpw(x)
#define outport(x,y) outpw(x,y)
#define disable() _disable()
#define enable() _enable()
#endif


#ifdef __DJGPP__
#include <dpmi.h>
#include <go32.h>
#include <pc.h>
#define inp inportw
#define outport outportw
#define inport inportw
#define interrupt 
#endif

#endif
