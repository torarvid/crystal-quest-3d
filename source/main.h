/*****************************************************************************\ 
 *
 * Crystal Quest 3D
 * Copyright (C) 2002 Tor Arvid Lund
 *
 * $Id: main.h,v 1.1 2003/02/22 19:48:57 totto Exp $
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
 * This is the header file for the main function and other global stuff
 *
 * Written by Tor Arvid Lund
 *
\*****************************************************************************/ 


#include "globals.h"

/*---------------------------------------------------------------------------*/ 

/* A pointer to the drawingboard */
SDL_Surface *surface;
bool drawReal = true;

/* Variable to hold the maximum color depth */
int max_bpp;

/* Variable to hold flags to be passed to SDL_SetVideoMode */
int vidFlags;

/* Variable to tell us if our window is active or not */
bool isActive;

/* Mouse cursor position */
int mouseX = 0, mouseY = 0;

/* Array to hold key press information */
int keys[256];

/* Array to hold mouse press information */
int buttons[5];

/* Forward key */
int fkey = SDLK_f;

/* Backward key */
int bkey = SDLK_d;

/* Strafe left key */
int lkey = SDLK_a;

/* Strafe right key */
int rkey = SDLK_s;

/* Angle for looking up/down */
float upDownAngle = 0.0f;

/* Viewing angle */
float viewAngle = 0.0f;

/* Rotation angle */
float rotAngle = 0.0f;

/* Translation variables for x, y and z direction */
float xTrans = 0.0f;
float yTrans = 0.0f;
float zTrans = 0.0f;

/* Matrix holding viewing vectors */
float viewMatrix[3][3];

/* Speed delta constant */
float speed = 0.003f;

/* Rotation speed */
float rotSpeed = 0.5f;

/* Directional speed */
float xSpeed = 0.0f;
float ySpeed = 0.0f;
float zSpeed = 0.0f;
  
/* Vectors holding the generic axii */
float xAxis[] = {1.0f, 0.0f, 0.0f};
float yAxis[] = {0.0f, 1.0f, 0.0f};
float zAxis[] = {0.0f, 0.0f, 1.0f};

/* Mouse Sensitivity */
float mouseSense = 150.0f;

/* Camera position buffer */
posf *camBase;
posf *camWrt;
posf *camRead;
bool startCam = false;

/* Ship angles buffer */
vec3f *shipBase;
vec3f *shipWrt;
vec3f *shipRead;
bool startShip = false;

/* Ship rotation matrix */
float shipMatrix[3][3];

/* Debug mode */
bool debug = false;

/* Log file */
FILE *logfile;

char playerName[16];

/*---------------------------------------------------------------------------*/

int main(int argc, char **argv);
