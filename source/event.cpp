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
 * This is the source file for the event polling routines
 *
 * Written by Tor Arvid Lund
 *
\*****************************************************************************/ 


#include "event.h"


/*---------------------------------------------------------------------------*/ 

/* Up-Down movement */
static float udm = 0.0f;

/* Side-Side movement */
static float ssm = 0.0f;

/* Rotation movement */
static float rm = 0.0f;

/* TEMPORARY - register mouse x and y movement... */
bool regX = true;
bool regY = false;

void processKeys()
{
  /* Rotate the viewing matrix */
  rotVector(xAxis, viewMatrix, degToRad(udm));
  rotVector(yAxis, viewMatrix, degToRad(ssm));
  rotVector(zAxis, viewMatrix, degToRad(rm));

  /* Reset angles */
  udm = 0.0f;
  ssm = 0.0f;
  rm = 0.0f;
  
  /* Perform keyboard-activated actions */
  if (keys[SDLK_ESCAPE])
    quitProgram(0);
  if (keys[SDLK_F1])
    SDL_WM_ToggleFullScreen(surface);
  if (keys[SDLK_w])
  {
    printf("sx:%g\nsy:%g\nsz:%g\n", 
	viewMatrix[0][0], viewMatrix[0][1], viewMatrix[0][2]);
    printf("ux:%g\nuy:%g\nuz:%g\n", 
	viewMatrix[1][0], viewMatrix[1][1], viewMatrix[1][2]);
    printf("vx:%g\nvy:%g\nvz:%g\n\n", 
	viewMatrix[2][0], viewMatrix[2][1], viewMatrix[2][2]);
  }
  if (keys[SDLK_g])
    regX = false;
  else
    regX = true;
  if (keys[SDLK_h])
    regY = false;
  else
    regY = true;
  if (keys[SDLK_b])
  {
    xSpeed = 0.0f;
    ySpeed = 0.0f;
    zSpeed = 0.0f;
  }
  if (keys[fkey])
  {
    xSpeed -= speed * viewMatrix[2][0];
    ySpeed -= speed * viewMatrix[2][1];
    zSpeed -= speed * viewMatrix[2][2];
  }
  if (keys[bkey])
  {
    xSpeed += speed * viewMatrix[2][0];
    ySpeed += speed * viewMatrix[2][1];
    zSpeed += speed * viewMatrix[2][2];
  }
  if (keys[lkey])
  {
    rotAngle += rotSpeed;
    rm += rotSpeed;
  }
  if (keys[rkey])
  {
    rotAngle -= rotSpeed;
    rm -= rotSpeed;
  }
  xTrans += xSpeed;
  yTrans += ySpeed;
  zTrans += zSpeed;
}


/*---------------------------------------------------------------------------*/ 

void eventPoll()
{
  /* Variable to hold event information */
  SDL_Event event;
  int key;

  /* Poll for events */
  while(SDL_PollEvent(&event)){
    switch(event.type){
    case SDL_MOUSEMOTION:
      if (regX)
      {
	viewAngle -= (SC_WIDTH / 2 - event.motion.x) / mouseSense;
	ssm -= (SC_WIDTH / 2 - event.motion.x) / mouseSense;
      }
      if (regY)
      {
	upDownAngle -= (SC_HEIGHT / 2 - event.motion.y) /mouseSense;
	udm -= (SC_HEIGHT / 2 - event.motion.y) / mouseSense; 
      }
      break;
    case SDL_ACTIVEEVENT: /* Check window focus */
      if (event.active.gain == 0)
	isActive = false;
      else
	isActive = true;
      break;
    case SDL_VIDEORESIZE: /* Resize the window */
      /* Set video mode for new resolution */
      surface = SDL_SetVideoMode(event.resize.w, 
				 event.resize.h, 
				 max_bpp, 
				 vidFlags);
      /* If mode is not available, quit... */
      if (!surface){
	fprintf(stderr, "Couldn't create surface at resolution %dx%dx%d: %s\n",
		event.resize.w, event.resize.h, max_bpp, SDL_GetError());
	quitProgram(1);
      }
      resizeWindow(event.resize.w, event.resize.h);
      break;
    case SDL_KEYDOWN:
      key = event.key.keysym.sym;
      keys[key] = true;
      if ((key == fkey) && (keys[fkey]))
	keys[bkey] = false;
      else if ((key == bkey) && (keys[bkey]))
	keys[fkey] = false;
      else if ((key == lkey) && (keys[lkey]))
	keys[rkey] = false;
      else if ((key == rkey) && (keys[rkey]))
	keys[lkey] = false;
      break;
    case SDL_KEYUP:
      keys[event.key.keysym.sym] = false;
      break;
    case SDL_QUIT:
      quitProgram(1);
      break;
    }
  }
  processKeys();
}
