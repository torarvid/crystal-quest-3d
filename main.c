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
 * This is the source file for the main function, and other routines
 * that... well... didn't belong anywhere else :)
 *
 * Written by Tor Arvid Lund
 *
\*****************************************************************************/ 


#include "main.h"


/*---------------------------------------------------------------------------*/ 

/* A pointer to the drawingboard */
SDL_Surface *surface;

/* Variable to hold the maximum color depth */
int max_bpp;

/* Variable to hold flags to be passed to SDL_SetVideoMode */
int vidFlags;

/* Variable to tell us if our window is active or not */
bool isActive;

/* Array to hold key press information */
int keys[256];

/* Forward key */
const int fkey = SDLK_f;

/* Backward key */
const int bkey = SDLK_d;

/* Strafe left key */
const int lkey = SDLK_a;

/* Strafe right key */
const int rkey = SDLK_s;

/* Angle for looking up/down */
float upDownAngle = 0.0f;

/* Viewing angle */
float viewAngle = 0.0f;

/* Translation variables for x, y and z direction */
float xTrans = 0.0f;
float yTrans = 0.0f;
float zTrans = 0.0f;

float vM[3][3];
float xM[3][3];

/* Speed constant */
const float speed = 0.2f;

/* Mouse Sensitivity */
float mouseSense = 40.0f;


/*---------------------------------------------------------------------------*/ 

float degToRad(float deg)
{
  return deg * PI / 180.0f;
}


/*---------------------------------------------------------------------------*/ 

void resizeWindow(int width, int height)
{
  /* Prevent division by zero errors */
  if (height == 0)
    height = 1;

  /* Set up GL viewport */
  glViewport(0, 0, width, height);

  /* Apply our viewing volume to the projection matrix */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);
  /* glOrtho(0.0f, width, height, 0.0f, -1.0f, 100.0f);*/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


/*---------------------------------------------------------------------------*/ 

void quitProgram(int returnValue)
{
  SDL_Quit();
  exit(returnValue);
}


/*---------------------------------------------------------------------------*/ 

void rotVector(float vector[3], 
	       float Matrix[3][3],
	       float retMatrix[3][3],
	       float angle
    )
{
  float rotMatrix[3][3];
  float x, y, z, s, c, t, sx, sy, sz, tyz, txz, txy;

  x = vector[0];
  y = vector[1];
  z = vector[2];
  s = sin(angle);
  c = cos(angle);
  t = 1.0f - c;

  printf("x:%g\ny:%g\nz:%g\n\n", x, y, z);
  sx = s * x;
  sy = s * y;
  sz = s * z;

  tyz = t * y * z;
  txz = t * x * z;
  txy = t * x * y;

  rotMatrix[0][0] = t * x * x + c;
  rotMatrix[1][1] = t * y * y + c;
  rotMatrix[2][2] = t * z * z + c;
  
  rotMatrix[0][1] = txy + sz;
  rotMatrix[1][0] = txy - sz;

  rotMatrix[0][2] = txz - sy;
  rotMatrix[2][0] = txz + sy;

  rotMatrix[1][2] = tyz + sx;
  rotMatrix[2][1] = tyz - sx;
  
  retMatrix[0][0] = Matrix[0][0] * rotMatrix[0][0] +
    		    Matrix[1][0] * rotMatrix[0][1] + 
		    Matrix[2][0] * rotMatrix[0][2];
  retMatrix[0][1] = Matrix[0][1] * rotMatrix[0][0] +
    		    Matrix[1][1] * rotMatrix[0][1] + 
		    Matrix[2][1] * rotMatrix[0][2];
  retMatrix[0][2] = Matrix[0][2] * rotMatrix[0][0] +
    		    Matrix[1][2] * rotMatrix[0][1] + 
		    Matrix[2][2] * rotMatrix[0][2];
  retMatrix[1][0] = Matrix[0][0] * rotMatrix[1][0] +
    		    Matrix[1][0] * rotMatrix[1][1] + 
		    Matrix[2][0] * rotMatrix[1][2];
  retMatrix[1][1] = Matrix[0][1] * rotMatrix[1][0] +
    		    Matrix[1][1] * rotMatrix[1][1] + 
		    Matrix[2][1] * rotMatrix[1][2];
  retMatrix[1][2] = Matrix[0][2] * rotMatrix[1][0] +
    		    Matrix[1][2] * rotMatrix[1][1] + 
		    Matrix[2][2] * rotMatrix[1][2];
  retMatrix[2][0] = Matrix[0][0] * rotMatrix[2][0] +
    		    Matrix[1][0] * rotMatrix[2][1] + 
		    Matrix[2][0] * rotMatrix[2][2];
  retMatrix[2][1] = Matrix[0][1] * rotMatrix[2][0] +
    		    Matrix[1][1] * rotMatrix[2][1] + 
		    Matrix[2][1] * rotMatrix[2][2];
  retMatrix[2][2] = Matrix[0][2] * rotMatrix[2][0] +
    		    Matrix[1][2] * rotMatrix[2][1] + 
		    Matrix[2][2] * rotMatrix[2][2];
}


/*---------------------------------------------------------------------------*/ 

void rotX(float Matrix[3][3], float retMatrix[3][3], float angle)
{
  retMatrix[0][0] = Matrix[0][0] * 1 +
    		    Matrix[1][0] * 0 + 
		    Matrix[2][0] * 0;
  retMatrix[0][1] = Matrix[0][1] * 1 +
    		    Matrix[1][1] * 0 + 
		    Matrix[2][1] * 0;
  retMatrix[0][2] = Matrix[0][2] * 1 +
    		    Matrix[1][2] * 0 + 
		    Matrix[2][2] * 0;
  retMatrix[1][0] = Matrix[0][0] * 0 +
    		    Matrix[1][0] * cos(angle) + 
		    Matrix[2][0] * sin(angle);
  retMatrix[1][1] = Matrix[0][1] * 0 +
    		    Matrix[1][1] * cos(angle) + 
		    Matrix[2][1] * sin(angle);
  retMatrix[1][2] = Matrix[0][2] * 0 +
    		    Matrix[1][2] * cos(angle) + 
		    Matrix[2][2] * sin(angle);
  retMatrix[2][0] = Matrix[0][0] * 0 +
    		    Matrix[1][0] * -sin(angle) + 
		    Matrix[2][0] * cos(angle);
  retMatrix[2][1] = Matrix[0][1] * 0 +
    		    Matrix[1][1] * -sin(angle) + 
		    Matrix[2][1] * cos(angle);
  retMatrix[2][2] = Matrix[0][2] * 0 +
    		    Matrix[1][2] * -sin(angle) + 
		    Matrix[2][2] * cos(angle);
}


/*---------------------------------------------------------------------------*/ 

void rotY(float Matrix[3][3], float retMatrix[3][3], float angle)
{
  retMatrix[0][0] = Matrix[0][0] * cos(angle) +
    		    Matrix[1][0] * 0 + 
		    Matrix[2][0] * -sin(angle);
  retMatrix[0][1] = Matrix[0][1] * cos(angle) +
    		    Matrix[1][1] * 0 + 
		    Matrix[2][1] * -sin(angle);
  retMatrix[0][2] = Matrix[0][2] * cos(angle) +
    		    Matrix[1][2] * 0 + 
		    Matrix[2][2] * -sin(angle);
  retMatrix[1][0] = Matrix[0][0] * 0 +
    		    Matrix[1][0] * 1 + 
		    Matrix[2][0] * 0;
  retMatrix[1][1] = Matrix[0][1] * 0 +
    		    Matrix[1][1] * 1 + 
		    Matrix[2][1] * 0;
  retMatrix[1][2] = Matrix[0][2] * 0 +
    		    Matrix[1][2] * 1 + 
		    Matrix[2][2] * 0;
  retMatrix[2][0] = Matrix[0][0] * sin(angle) +
    		    Matrix[1][0] * 0 + 
		    Matrix[2][0] * cos(angle);
  retMatrix[2][1] = Matrix[0][1] * sin(angle) +
    		    Matrix[1][1] * 0 + 
		    Matrix[2][1] * cos(angle);
  retMatrix[2][2] = Matrix[0][2] * sin(angle) +
    		    Matrix[1][2] * 0 + 
		    Matrix[2][2] * cos(angle);
}


/*---------------------------------------------------------------------------*/ 

void loadIdentity(float Matrix[3][3])
{
  int i, j;

  for(i=0;i<3;i++)
    for(j=0;j<3;j++)
      Matrix[i][j] = (i == j) ? 1.0f : 0.0f;
  Matrix[2][2] = -1.0f;
}


/*---------------------------------------------------------------------------*/ 

int main(int argc, char **argv)
{
  /* Struct to hold some video information*/
  const SDL_VideoInfo *vidInfo;

  vM[0][0] = 1.0f;
  vM[0][1] = 0.0f;
  vM[0][2] = 0.0f;

  vM[1][0] = 0.0f;
  vM[1][1] = 1.0f; 
  vM[1][2] = 0.0f;

  vM[2][0] = 0.0f;
  vM[2][1] = 0.0f;
  vM[2][2] = 1.0f;  

  /* Initialization of SDL video system */
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "SDL Video initialization failed: %s\n", SDL_GetError());
    quitProgram(1);
  }

  isActive = true;

  vidInfo = SDL_GetVideoInfo();
  if (!vidInfo)
  {
    fprintf(stderr, "SDL Video query failed: %s\n", SDL_GetError());
    quitProgram(1);
  }

  /* Enable OpenGL, double buffering, storing of palette in hardware, and
     resizing of the window */
  vidFlags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE | SDL_RESIZABLE;
  
  /* Enable hardware graphics buffer if available, else enable software
     graphics buffer */
  if (vidInfo->hw_available)
  {
    vidFlags |= SDL_HWSURFACE;
  }
  else
  {
    vidFlags |= SDL_SWSURFACE;
  }

  /* Enable hardware blitting if available */
  if (vidInfo->blit_hw)
  {
    vidFlags |= SDL_HWACCEL;
  }

  /* Get the mouse cursor to the center of out window */
  SDL_ShowCursor(SDL_DISABLE);
  SDL_WarpMouse(SC_WIDTH / 2, SC_HEIGHT / 2);

  /* Sets up OpenGL double buffering */
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  /* Finding the maximum color depth */
  fprintf(stdout, "Checking video mode: %dx%dx%d\n",
	  SC_WIDTH, SC_HEIGHT, SC_BPP);
  max_bpp = SDL_VideoModeOK(SC_WIDTH, SC_HEIGHT, SC_BPP, vidFlags);

  /* Setting video mode with maximum color depth */
  fprintf(stdout, "Setting video mode: %dx%dx%d\n", 
	  SC_WIDTH, SC_HEIGHT, max_bpp);
  surface = SDL_SetVideoMode(SC_WIDTH, SC_HEIGHT, max_bpp, vidFlags);
  
  /* Verify that the creation of a surface went OK */
  if (!surface)
  {
    fprintf(stderr, "Couldn't create surface at resolution %dx%dx%d: %s\n", 
	    SC_WIDTH, SC_HEIGHT, max_bpp, SDL_GetError());
    quitProgram(1);
  }
  
  /* Enable repeating of keys 890510*/
  if (SDL_EnableKeyRepeat(100, 80))
  {
    fprintf(stderr, "Failed to enable key repeating %s\n", SDL_GetError());
    quitProgram(1);
  }

  /* Initialize OpenGL */
  initGL();

  buildLists();

  /* Request initial window resize */
  resizeWindow(SC_WIDTH, SC_HEIGHT);

  /* Infinite loop which polls for events and draws the scene */
  while(1)
  {
    SDL_WarpMouse(SC_WIDTH / 2, SC_HEIGHT / 2);
    eventPoll();
    if (isActive)
      drawScene();
  }


  /* Should never get here...*/
  quitProgram(0);

  /* And certainly not here... */
  return 0;
}
