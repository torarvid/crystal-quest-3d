/*****************************************************************************\ 
 *
 * Crystal Quest 3D
 * Copyright (C) 2002 Tor Arvid Lund
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
 * This is the header file for the event polling routines
 *
 * Written by Tor Arvid Lund
 *
\*****************************************************************************/ 


#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include "globals.h"
#include <math.h>

extern SDL_Surface *surface;
extern int max_bpp;
extern int vidFlags;
extern const int fkey, bkey, lkey, rkey;
extern int keys[256];
extern bool isActive;
extern GLfloat ex, ey, ez, rx, ry, rz, ux, uy, uz;
extern float upDownAngle;
extern float viewAngle;
extern float mouseSense;
extern float xTrans, yTrans, zTrans;
extern float speed;
extern float vM[3][3];
extern float xM[3][3];

extern void loadIdentity(float Matrix[3][3]);
extern void rotVector(float vector[3], 
    		      float Matrix[3][3], 
		      float retMatrix[3][3], 
		      float angle
		      );
extern void rotX(float Matrix[3][3], float retMatrix[3][3], float angle);
extern void rotY(float Matrix[3][3], float retMatrix[3][3], float angle);
extern void quitProgram(int returnValue);
extern void resizeWindow(int width, int height);
extern float degToRad(float deg);

void eventPoll();
