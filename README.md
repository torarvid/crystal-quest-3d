Crystal Quest 3D
================

This was a school project that was written in Spring 2002.

It is a simple game. The objective is to fly a plane through a 3D world and collect crystals as you go. There
is little friction in the game, so steering the plane is meant to be somewhat challenging.

Practical info
--------------

The game was originally written on Linux. Dependencies are OpenGL, SDL and SDL-mixer (for sound). In later commits,
I've tried to make it run on Windows and Mac OS X. It works most of the time, but there are also a heisenbug,
which cause the plane to be facing the wrong way upon load (and being stuck that way).
