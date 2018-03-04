/*****************************************************************************\ 
 *
 * Crystal Quest 3D
 * Copyright (C) 2002 Tor Arvid Lund
 *
 * $Id: event.cpp,v 1.1 2003/02/22 19:48:57 totto Exp $
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

void processKeys()
{
  static int counter = 0;
  vec3f *ship;
  int i,a;
  int loopLimit;
  int test;

  /* Rotate the viewing matrix */
  rotVector(xAxis, viewMatrix, degToRad(udm));
  rotVector(yAxis, viewMatrix, degToRad(ssm));
  rotVector(zAxis, viewMatrix, degToRad(rm));

  /* Save angles to ship rotation matrix */
  shipWrt->x = udm;
  shipWrt->y = ssm;
  shipWrt->z = -rm;

  if (shipWrt == shipBase + CAM_FRAME_DELAY - 1)
    startShip = true;

  shipWrt = (++shipWrt < (CAM_FRAME_DELAY + shipBase)) ? shipWrt : shipBase;

  loadIdentity(shipMatrix);
  if (!startShip)
    loopLimit = (int)(shipWrt - shipRead);
  else
    loopLimit = CAM_FRAME_DELAY;
  for(i=0;i<loopLimit;i++)
  {
    a = CAM_FRAME_DELAY - (int)(shipRead - shipBase);
    ship = shipRead + i;
    if (ship >= shipBase + CAM_FRAME_DELAY - 1)
    {
      ship = shipBase + i - a;
      test = 1;
    }
    else
      test = 0;
    rotVector(xAxis, shipMatrix, degToRad(ship->x));
    rotVector(yAxis, shipMatrix, degToRad(ship->y));
    rotVector(zAxis, shipMatrix, degToRad(ship->z));
    if (startShip)
    {
      if(counter++ < 1500)
        ;/*printf("%f %f %f\n", ship->x, ship->y, ship->z);*/
    }

  }
  if (startShip)
    shipRead = (++shipRead < (CAM_FRAME_DELAY + shipBase)) ? shipRead : shipBase;

  /* Reset angles */
  udm = 0.0f;
  ssm = 0.0f;
  rm = 0.0f;

  /* Perform keyboard-activated actions */
  if (keys[SDLK_ESCAPE])
  {
    stateChange(STATE_MENU_MAIN);
    keys[SDLK_ESCAPE] = false;
  }
  if (keys[SDLK_i])
  {
    keys[SDLK_i] = false;
    logString("Fullscreen toggle pressed\n");
    /*SDL_WM_ToggleFullScreen(surface);*/
  }
  if (keys[SDLK_q])
    quitProgram(0);
  if (keys[SDLK_o])
  {
    printf("sx:%g\nsy:%g\nsz:%g\n", 
        viewMatrix[0][0], viewMatrix[0][1], viewMatrix[0][2]);
    printf("ux:%g\nuy:%g\nuz:%g\n", 
        viewMatrix[1][0], viewMatrix[1][1], viewMatrix[1][2]);
    printf("vx:%g\nvy:%g\nvz:%g\n\n", 
        viewMatrix[2][0], viewMatrix[2][1], viewMatrix[2][2]);
  }
  if (keys[SDLK_l])
    printTempMatrix = true;
  else
    printTempMatrix = false;
  if (keys[SDLK_g])
    regX = false;
  else
    regX = true;
  if (keys[SDLK_h])
    regY = false;
  else
    regY = true;
  if (keys[SDLK_c])
    drawReal = false;
  else
    drawReal = true;
  if (keys[SDLK_m])
    done = true;
  if (keys[SDLK_r])
    realShip->Load();
  if (keys[SDLK_e])
    explosion->Reset();
  if (keys[SDLK_p])
  {
    printf("%f %f %f\n", realShip->m_Pos.x, realShip->m_Pos.y,
        realShip->m_Pos.z);
  }
  if (keys[SDLK_SPACE])
  {
    if (crash)
    {
      int a = (sector == 0) ? 0 : sector - 1;
      xTrans = realWorld[a]->m_Center.x * realWorld[a]->m_Scale;
      yTrans = realWorld[a]->m_Center.y * realWorld[a]->m_Scale;
      zTrans = realWorld[a]->m_Center.z * realWorld[a]->m_Scale;
      xSpeed = ySpeed = zSpeed = 0.0f;
      crash = false;
      if (gameover)
      {
        lives = 10;
        xTrans = yTrans = zTrans = 0.0f;
        for(int abc=0;abc<NUM_CRYSTALS;abc++)
          realCrystals[abc]->isActive = true;
        numCrystals = 0;
        gameover = false;
        john->Load();
      }
    }
  }
  if (keys[SDLK_b])
  {
    xSpeed = 0.0f;
    ySpeed = 0.0f;
    zSpeed = 0.0f;
  }
  if (keys[fkey] && (state == STATE_PLAYING))
  {
    xSpeed += speed * viewMatrix[2][0] * cur_time;
    ySpeed += speed * viewMatrix[2][1] * cur_time;
    zSpeed += speed * viewMatrix[2][2] * cur_time;
  }
  if (keys[bkey] && (state == STATE_PLAYING))
  {
    xSpeed -= speed * viewMatrix[2][0] * cur_time;
    ySpeed -= speed * viewMatrix[2][1] * cur_time;
    zSpeed -= speed * viewMatrix[2][2] * cur_time;
  }
  if (keys[lkey] && (state == STATE_PLAYING))
  {
    rotAngle += rotSpeed;
    rm += rotSpeed;
  }
  if (keys[rkey] && (state == STATE_PLAYING))
  {
    rotAngle -= rotSpeed;
    rm -= rotSpeed;
  }
  if(keys[SDLK_x])
  {
    printFPS = !printFPS;
    keys[SDLK_x] = false;
  }
  xSpeed *= FRICTION;
  ySpeed *= FRICTION;
  zSpeed *= FRICTION;
  if (crash)
    xSpeed = ySpeed = zSpeed = 0.0f;
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

  cur_time = SDL_GetTicks() - prevTime;
  prevTime += cur_time;

  /*printf("Time: %i\n", cur_time);*/

  /* Poll for events */
  while(SDL_PollEvent(&event)){
    switch(event.type){
      case SDL_MOUSEBUTTONDOWN:
        buttons[event.button.button] = true;
        break;
      case SDL_MOUSEBUTTONUP:
        buttons[event.button.button] = false;
        break;
      case SDL_MOUSEMOTION:
        if (regX && (state == STATE_PLAYING))
        {
          viewAngle -= (SC_WIDTH / 2 - event.motion.x) * cur_time / mouseSense;
          ssm -= (SC_WIDTH / 2 - event.motion.x) * cur_time / mouseSense;
        }
        if (regY && (state == STATE_PLAYING))
        {
          upDownAngle -= (SC_HEIGHT / 2 - event.motion.y) * cur_time /mouseSense;
          udm -= (SC_HEIGHT / 2 - event.motion.y) * cur_time / mouseSense;
        }
        mouseX = event.motion.x;
        mouseY = event.motion.y;
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
