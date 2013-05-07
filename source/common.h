/*****************************************************************************\ 
 *
 * Crystal Quest 3D
 * Copyright (C) 2002 Tor Arvid Lund
 *
 * $Id: common.h,v 1.1 2003/02/22 19:48:57 totto Exp $
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version. 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details. 
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * If you do not find the GNU General public license in the file 'GPL'
 * which should be distributed along with the rest of this program,
 * you can normally find it at http://www.gnu.org/licenses/gpl.html
 *
 *
 * If you have comments, suggestions or otherwise wish to contact
 * me, write an email to: 
 *
 * totto@boredom.nu
 *
\*****************************************************************************/ 


/*****************************************************************************\ 
 *
 * This is the header file describing common functions (obviously...)
 *
 * Written by Tor Arvid Lund
 *
\*****************************************************************************/ 

#ifndef CQ3D_HEADER_COMMON
#define CQ3D_HEADER_COMMON

#include "globals.h"
#include "Model3ds.h"
#include "particle.h"

/*---------------------------------------------------------------------------*/

CModel3ds *realWorld[WORLD_OBJECTS];
CModel3ds *collWorld[WORLD_OBJECTS];
CModel3ds *realStones[MAX_STONES];
CModel3ds *collStones[MAX_STONES];
CModel3ds *realShip;
CModel3ds *collShip;
CModel3ds *realCrystals[NUM_CRYSTALS];
CModel3ds *john;
CModel3ds *theEnd;
CParticleSystem* explosion;
GLuint base; /* Base Display List For The Font Set */
GLuint fontTexture;

/*---------------------------------------------------------------------------*/

int LoadGLTextures();
GLvoid KillFont(void);
GLvoid BuildFont(void);
GLvoid glPrint(GLint x, GLint y, int set, const char *fmt, ...);
void loadStuff();
float degToRad(float deg);
float radToDeg(float rad);
void resizeWindow(int width, int height);
void quitProgram(int returnValue);
void rotVector(float vector[3], float inMatrix[3][3], float angle);
void loadIdentity(float Matrix[3][3]);
void usage();
void doStateMachine();
void stateChange(int newstate);
void logString(const char *str, ...);

#endif
