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

static float udm = 0.0f;
static float ssm = 0.0f;
static float rm = 0.0f;
bool regX = true;
bool regY = false;

void processKeys()
{
  float xAxis[] = {1.0f, 0.0f, 0.0f};
  float yAxis[] = {0.0f, 1.0f, 0.0f};
  float zAxis[] = {0.0f, 0.0f, 1.0f};

  float rotSpeed = 0.4f;

  static float xSpeed = 0.0f;
  static float ySpeed = 0.0f;
  static float zSpeed = 0.0f;
  
  rotVector(xAxis, vM, xM, degToRad(udm));
  rotVector(yAxis, xM, vM, degToRad(ssm));
  rotVector(zAxis, vM, xM, degToRad(rm));

  /* dummy... (matrix copy) */
  rotVector(xAxis, xM, vM, 0.0f);

  udm = 0.0f;
  ssm = 0.0f;
  rm = 0.0f;
  
  if (keys[SDLK_ESCAPE])
    quitProgram(0);
  if (keys[SDLK_F1])
    SDL_WM_ToggleFullScreen(surface);
  if (keys[SDLK_w])
  {
    printf("sx:%g\nsy:%g\nsz:%g\n", vM[0][0], vM[0][1], vM[0][2]);
    printf("ux:%g\nuy:%g\nuz:%g\n", vM[1][0], vM[1][1], vM[1][2]);
    printf("vx:%g\nvy:%g\nvz:%g\n\n", vM[2][0], vM[2][1], vM[2][2]);
  }
  if (keys[SDLK_g])
    regX = false;
  else
    regX = true;
  if (keys[SDLK_h])
    regY = false;
  else
    regY = true;
  if (keys[fkey])
  {
    xSpeed -= speed * xM[2][0];
    ySpeed -= speed * xM[2][1];
    zSpeed -= speed * xM[2][2];
  }
  if (keys[bkey])
  {
    xSpeed += speed * xM[2][0];
    ySpeed += speed * xM[2][1];
    zSpeed += speed * xM[2][2];
  }
  if (keys[lkey])
    rm += rotSpeed;
  if (keys[rkey])
    rm -= rotSpeed;
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
