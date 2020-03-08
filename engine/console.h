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

#ifndef CONSOLE_INC
#define CONSOLE_INC

#include "main.h"

#define ETC 0
#define RENDER 1
#define PFLIP 2

extern byte cpu_watch;
extern byte cpubyte;

extern void Con_Key(int key);
extern void V2SE_Key(int key);

extern int Con_IsConsoleAllowed();
extern void Con_AllowConsole(int allow);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERFACE //////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

extern void Console_Init();
extern void Console_Printf(string_t str);
extern void Console_SendCommand(string_t cmd);

extern void Console_Activate();

#endif // CONSOLE_INC
