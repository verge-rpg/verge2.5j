-----------------------------------------------------------------------------
          VERGE v2.5+j -- the version that the Speed Bump worked on
-----------------------------------------------------------------------------

First off, I did my utmost to keep everything all nice n' compatible and
whatnot, so you should be able to just drop the EXE in instead of v2k b6.1's.

Unfortunately, one thing did end up changing.  Entities with a speed of 0 are
stopped now.  A little more intuitive, no?  If this is a problem, just stick
this in the AutoExec event in your VC:

for (i=0; i<numents; i++)
 if (entity.speed[i]=0) entity.speed[i]=4;

Change the "i"'s to any temp variable declared in system.vc.

Okay, now for all the goodies that I added.

New functions/variables:

bitdepth - returns the number of bits per pixel 8, 15 or 16.  (actually, aen
           added this one to the compiler, I simply put the code in VERGE.EXE)

SetLucent- this works the same as always, but has a few more tricks up it's
           sleave.
           0 - Opaque :P
           1 - normal lucency
           2 - variable lucency (set the level with CalcLucent)
           3 - colour addition
           4 - colour subtraction
           5 - variable colour addition (set with CalcLucent)
           6 - variable colour subtraction (also set with CalcLucent)

CalcLucent-this precalculates the lucency table for SetLucent.  0 is in-
           visible, while 255 is opaque. ie CalcLucent(58);

sqrt   -   Returns the square root of the value passed.
           ie a=sqrt((x*x)+(y*y));

um.. that's it. heh.  I'll add more stuff later, honest!

Now, more than that, v2.5 also supports some new file formats, namely
hicolour CHRs and VSPs.  New conversion utils are included. (but I readily
admit that they're subpar) WinMapEd will be fully capable of editing both
the old and new file formats.

BMP2VSP This works just like PCX2VSP, except it accepts a 24 bit BMP, and outputs a hicolour VSP.

CHRMAK16 This is a POS, and I know it. ^_^; It works somewhat like CHRMAK, except the .MAK file format is different,  it accepts a truecolour BMP as a source file, and it outputs a hicolour CHR.