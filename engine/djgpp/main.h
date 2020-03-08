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
#ifndef MAIN_H
#define MAIN_H

extern void *valloc(int amount, char *desc, int owner);
//void *valloc(int amount);
void err(char *message, ...);
int sgn(int x);
void ver();
void CheckMessages();
void Log(char *message, ...);
void Logp(char *message, ...);
void LogDone();
void InitMouse(int, int);
void ReadMouse();
void SetMouse(int, int);

extern char startmap[80], nocdaudio;
extern int mx, my, mb;

#define vfree(x) if (x) { free(x); x=0; }

#ifndef DJGPP
#define strncasecmp strnicmp
#define strcasecmp strcmpi
#endif

#endif
