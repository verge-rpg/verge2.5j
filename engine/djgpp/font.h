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
#ifndef FONT_H
#define FONT_H

struct fontstruct
{
  byte width, height;                  // Font character dimensions
  byte *data;                          // Font bitmap data
};

extern struct fontstruct font[10];      // Font stuff;
extern int numfonts;                    // number of loaded fonts.
extern int curx, cury;

int LoadFont(char *fname);
void TextColor(int c);
void NumColor(int c);
void GotoXY(int x, int y);
void printstring(int fnt, char *str);
void dec_to_asciiz(int num, char *buf);

#endif
