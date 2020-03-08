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
// Wee. A generic #include. I feel so warm and fuzzy inside. :)

// #include "mss.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// CHANGELOG:
// <aen, may 15>
// + added a_memory.h
// <aen, may 5>
// + added VERSION tag and made all text occurences of version use it
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#define VERSION "2.01b"

#ifdef _WIN32
  #include <windows.h>
  #define delay Sleep
#endif

#ifndef _WIN32
  #include <dos.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define byte unsigned char
#define word unsigned short int
#define quad unsigned int

#define TRUE 1
#define FALSE 0

#ifdef __cplusplus
extern "C" {
#endif

#include "inter.h"

#include "a_memory.h"

#include "vfile.h"
#include "conlib.h"
#include "console.h"
#include "controls.h"
#include "engine.h"
#include "entity.h"
#include "fli.h"
#include "font.h"
#include "image.h"
#include "keyboard.h"
#include "main.h"
#include "message.h"
#include "mikmod.h"
#include "render.h"
#include "sound.h"
#include "timer.h"
#include "vc.h"
#include "vdriver.h"

extern char *strbuf;

void CD_Init();
void CD_Deinit();
void CD_Open_Door();
void CD_Close_Door();
void CD_Stop();
void CD_Play(int track);

#ifdef __cplusplus
}
#endif
