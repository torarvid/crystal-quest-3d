/*****************************************************************************\ 
 *
 * Crystal Quest 3D
 * Copyright (C) 2002 Tor Arvid Lund
 *
 * $Id: draw.cpp,v 1.1 2003/02/22 19:48:57 totto Exp $
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
 * This is the source file for the scenes drawing routines
 *
 * Written by Tor Arvid Lund
 *
\*****************************************************************************/ 


#include "draw.h"

/*---------------------------------------------------------------------------*/ 

Uint32 time_left(void)
{
  Uint32 now = SDL_GetTicks();
  if(next_time <= now)
    return 0;
  else
    return next_time - now;
}


/*---------------------------------------------------------------------------*/

void drawMenu()
{
  int i;
  float dim[] = {0.3f, 0.3f, 0.3f, 1.0f};
  char load[] = "-\\|/";
  static Uint8 loadIndex = 0;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glLightfv(GL_LIGHT1, GL_DIFFUSE, dim);
  glDisable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glDisable(GL_BLEND);
  glPushMatrix( );
  glBindTexture(GL_TEXTURE_2D, tex);
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin( GL_QUADS );
  glNormal3f( 0.0f, 0.0f, 1.0f );
  glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -16.0f, -11.0f, -25.0f );
  glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  16.0f, -11.0f, -25.0f );
  glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  16.0f,  11.0f, -25.0f );
  glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -16.0f,  11.0f, -25.0f );
  glEnd( );
  glPopMatrix( );
  glEnable(GL_BLEND);
  glDisable(GL_LIGHTING);
  switch(state)
  {
    case(STATE_LOADING):
      ++loadIndex;
      loadIndex %= 4;
      glColor3f(1.0f, 0.0f, 0.0f);
      glPrLine(PRINT_CENTER, false, 3, false, 1, 
	  "Loading... %c", load[loadIndex]);
      glColor3f(1.0f, 1.0f, 1.0f);
      break;
    case(STATE_MENU_MAIN):
      glColor3f(1.0f, 1.0f, 0.0f);
      glPrLine(PRINT_CENTER, false, 3, false, 1, "Crystal Quest 3D");
      if((mouseY < (SC_HEIGHT - 7 * 30)) && 
	 (mouseY > (SC_HEIGHT - 8 * 30)) &&
	 (mouseX > (int)((SC_WIDTH - 198) / 2)) && 
	 (mouseX < (int)((SC_WIDTH + 198) / 2)))
      {
	glColor3f(0.0f, 0.0f, 1.0f);
	if(buttons[SDL_BUTTON_LEFT])
	{
	  stateChange(STATE_PLAYING);
	}
      }
      else
	glColor3f(1.0f, 1.0f, 0.0f);
      if(gameStarted)
	glPrLine(PRINT_CENTER, false, 8, true, 0, "Resume Game");
      else
	glPrLine(PRINT_CENTER, false, 8, true, 0, "Start Game!");
      if((mouseY < (SC_HEIGHT - 5 * 30)) && 
	 (mouseY > (SC_HEIGHT - 6 * 30)) &&
	 (mouseX > (int)((SC_WIDTH - 198) / 2)) && 
	 (mouseX < (int)((SC_WIDTH + 198) / 2)))
      {
	glColor3f(0.0f, 0.0f, 1.0f);
	if(buttons[SDL_BUTTON_LEFT])
	{
	  stateChange(STATE_MENU_SCORES);
	}
      }
      else
	glColor3f(1.0f, 1.0f, 0.0f);
      glPrLine(PRINT_CENTER, false, 6, true, 0, "High Scores");
      if((mouseY < (SC_HEIGHT - 3 * 30)) && 
	 (mouseY > (SC_HEIGHT - 4 * 30)) &&
	 (mouseX > (int)((SC_WIDTH - 126) / 2)) && 
	 (mouseX < (int)((SC_WIDTH + 126) / 2)))
	glColor3f(0.0f, 0.0f, 1.0f);
      else
	glColor3f(1.0f, 1.0f, 0.0f);
      glPrLine(PRINT_CENTER, false, 4, true, 0, "Options");
      if((mouseY < (SC_HEIGHT -  1 * 30)) && 
	 (mouseY > (SC_HEIGHT -  2 * 30)) &&
	 (mouseX > (int)((SC_WIDTH - 72) / 2)) && 
	 (mouseX < (int)((SC_WIDTH + 72) / 2)))
      {
	glColor3f(0.0f, 0.0f, 1.0f);
	if(buttons[SDL_BUTTON_LEFT])
	  quitProgram(0);
      }
      else
	glColor3f(1.0f, 1.0f, 0.0f);
      glPrLine(PRINT_CENTER, false, 2, true, 0, "Quit");
      glColor3f(1.0f, 1.0f, 1.0f);
      break;
    case(STATE_MENU_SCORES):
      unsigned int len = 0;
      glColor3f(1.0f, 1.0f, 1.0f);
      glPrLine(PRINT_CENTER, false, 1, false, 1, "Hall of Fame");
      for(i=0; i<10; i++)
      {
	unsigned int l = strlen(scores[i].Name);
	len = (l > len) ? l : len;
      }
      for(i=0; i<10; i++)
      {
	unsigned int l = strlen(scores[i].Name);
	unsigned int j;
	for(j=0;j<(len-l);j++)
	  strcat(scores[i].Name, " ");
	glPrLine(PRINT_CENTER, false, 6+i, false, 0, "%s - %02d:%02d.%d",
	    scores[i].Name, scores[i].mins, scores[i].secs, scores[i].dsec);
      }
      break;
  }
  glEnable(GL_LIGHTING);
  SDL_GL_SwapBuffers( );
}

/*---------------------------------------------------------------------------*/

void drawScene()
{
  /* Variables to calculate fps */
  Uint32 t;
  static int frame_t0 = 0;
  static int frames = 0;
  static int fps = 0;
  int i,j,k;
  float tempMatrix[16];

  if (finished)
  {
    int place = 10;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glPrLine(PRINT_CENTER, false, 5, false, 0, 
	"CONGRATULATIONS!!! You finished the game!");
    for(i=9; i>=0; i--)
    {
      if ((minutes * 600 + seconds * 10 + dsec) <=
	  (scores[i].mins * 600 + scores[i].secs * 10 + scores[i].dsec))
	    place = i;
      else
	break;
    }
    if (place < 10)
    {
      SDL_ShowCursor(SDL_ENABLE);
      glPrLine(PRINT_CENTER, false, 7, false, 0,
	  "Enter your name for the Hall of Fame!");
      glPrLine(PRINT_CENTER, false, 10, true, 0,
	  "ABCDEFGHIJKLMNOPQRSTUVWXYZ Del Done");
      if((mouseY < (SC_HEIGHT - 9 * 30)) && 
	 (mouseY > (SC_HEIGHT - 10 * 30)) &&
	 buttons[SDL_BUTTON_LEFT])
      {
	static int numLetters = 0;
	int alphaStart = (int)((SC_WIDTH - 630) / 2);
	int letterIndex = (int)((mouseX - alphaStart) / 18);
	buttons[SDL_BUTTON_LEFT] = false;
	if(letterIndex >= 0 && letterIndex < 26)
	{
	  if(numLetters < 16)
	  {
	    sprintf(playerName, "%s%c", playerName, 'A' + letterIndex);
	    numLetters++;
	  }
	} 
	else if(letterIndex >= 27 && letterIndex < 30)
	{
	  int index = strlen(playerName) - 1;
	  index = (index > 0) ? index : 0;
	  playerName[index] = 0;
	  numLetters--;
	}
	else if(letterIndex >= 30)
	{
	  for(i=9; i>place; i--)
	  {
	    scores[i] = scores[i-1];
	  }
	  strcpy(scores[place].Name, playerName);
	  scores[place].mins = minutes;
	  scores[place].secs = seconds;
	  scores[place].dsec = dsec;
	  gameStarted = false;
	  finished = false;
	  lives = 10;
	  xTrans = yTrans = zTrans = 0.0f;
	  for(int abc=0;abc<NUM_CRYSTALS;abc++)
	    realCrystals[abc]->isActive = true;
	  numCrystals = 0;
	  SDL_ShowCursor(SDL_DISABLE);
	  stateChange(STATE_MENU_SCORES);
	  john->Load();
	}
      }
      glPrLine(PRINT_CENTER, false, 8, true, 0, "%s", playerName);
    }
    else
    {
      glPrLine(1, false, 1, true, 0, 
	  "Your time was: %02i:%02i.%i", minutes, seconds, dsec);
      if (buttons[SDL_BUTTON_LEFT])
      {
	buttons[SDL_BUTTON_LEFT] = false;
	lives = 10;
	xTrans = yTrans = zTrans = 0.0f;
	for(int abc=0;abc<NUM_CRYSTALS;abc++)
	  realCrystals[abc]->isActive = true;
	numCrystals = 0;
	gameover = false;
	gameStarted = false;
	finished = false;
	stateChange(STATE_MENU_SCORES);
	john->Load();
      }
    }
    SDL_GL_SwapBuffers();
    return;
  }
  SDL_WarpMouse(SC_WIDTH / 2, SC_HEIGHT / 2);
  for(i=0;i<3;i++)
    for(j=0;j<3;j++)
    {
      camWrt->pos[i][j] = viewMatrix[i][j];
    }

  tempMatrix[0] = shipMatrix[0][0];
  tempMatrix[1] = shipMatrix[1][0];
  tempMatrix[2] = shipMatrix[2][0];
  tempMatrix[3] = 0.0f;
  tempMatrix[4] = shipMatrix[0][1];
  tempMatrix[5] = shipMatrix[1][1];
  tempMatrix[6] = shipMatrix[2][1];
  tempMatrix[7] = 0.0f;
  tempMatrix[8] = shipMatrix[0][2];
  tempMatrix[9] = shipMatrix[1][2];
  tempMatrix[10] = shipMatrix[2][2];
  tempMatrix[11] = 0.0f;
  tempMatrix[12] = 0.0f;
  tempMatrix[13] = 0.0f;
  tempMatrix[14] = CAM_DIST;
  tempMatrix[15] = 1.0f;

  if (camWrt == camBase + CAM_FRAME_DELAY - 1)
  {
    startCam = true;
  }

  /* Try to clear the screen as fast as possible */
  glDisable( GL_DEPTH_TEST );
  glDisable( GL_DITHER );
  glShadeModel( GL_FLAT );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_DITHER );
  glShadeModel( GL_SMOOTH );
  glEnable(GL_LIGHT0);
  glDisable(GL_LIGHT1);

  /*glGetFloatv(GL_MODELVIEW_MATRIX, tempMatrix);*/
  if (printTempMatrix)
  {
    printf("| %.1f %.1f %.1f %.1f |\n",
	tempMatrix[0], tempMatrix[4], tempMatrix[8], tempMatrix[12]);
    printf("| %.1f %.1f %.1f %.1f |\n",
	tempMatrix[1], tempMatrix[5], tempMatrix[9], tempMatrix[13]);
    printf("| %.1f %.1f %.1f %.1f |\n",
	tempMatrix[2], tempMatrix[6], tempMatrix[10], tempMatrix[14]);
    printf("| %.1f %.1f %.1f %.1f |\n\n",
	tempMatrix[3], tempMatrix[7], tempMatrix[11], tempMatrix[15]);
  }

  if (startCam)
  {
    camRead = ((int)++camRead % ((int)(CAM_FRAME_DELAY + camBase)))
      ? camRead : camBase;
  }

  /* Draw stuff */

  glLoadIdentity();
  glEnable(GL_LIGHTING);

  gluLookAt(0.0f + camRead->pos[2][0] * CAM_DIST,
      0.0f + camRead->pos[2][1] * CAM_DIST,
      0.0f + camRead->pos[2][2] * CAM_DIST,
      0.0f, 0.0f, 0.0f,
      camRead->pos[1][0],
      camRead->pos[1][1],
      camRead->pos[1][2]);

  glPushMatrix();
  glLoadMatrixf(tempMatrix);
  glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
  if(!crash)
  {
    if (drawReal)
      realShip->Render();
    else
      collShip->Render();
  }
  glPopMatrix();


  glTranslatef(-xTrans, -yTrans, -zTrans);  
  realShip->m_Pos.x = xTrans;
  realShip->m_Pos.y = yTrans;
  realShip->m_Pos.z = zTrans;
  collShip->m_Pos.x = xTrans;
  collShip->m_Pos.y = yTrans;
  collShip->m_Pos.z = zTrans;

  for(k=0;k<WORLD_OBJECTS;k++)
    if (drawReal)
      realWorld[k]->Render();
    else
      collWorld[k]->Render();

  for(i=0;i<MAX_STONES;i++)
    realStones[i]->Render();
  for(i=0;i<NUM_CRYSTALS;i++)
    realCrystals[i]->Render();

  if (done)
  {
    john->m_Pos.z += 1.0f;
  }
  glTranslatef(0.0f, 0.0f, john->m_Pos.z);
  john->Render();
  glTranslatef(0.0f, 0.0f, -john->m_Pos.z);

  /*time = SDL_GetTicks() - prevTime;
    {
    signed int time_left = 1000 / FPS - time;
    if (time_left > 0)
    SDL_Delay(time_left);
    }
    prevTime = SDL_GetTicks();*/

  glTranslatef(xTrans, yTrans, zTrans);

  if (gameover)
  {
    explosion->Render();
    glPrLine(PRINT_CENTER, false, 10, false, 0, 
	"GAME OVER! Press SPACE to start over");
  }
  else if (crash)
  {
    explosion->Render();
    glPrLine(PRINT_CENTER, false, 10, false, 0, "Press SPACE to respawn");
  }
  glPrLine(1, false, 1, false, 0, 
      "Crystals: %i/%i", numCrystals, NUM_CRYSTALS);
  glPrLine(1, true, 1, false, 0, "%i lives left", lives);

  /*SDL_Delay(time_left());*/
  next_time += TICK_INTERVAL;

  /* Calculate fps */
  frames++;
  t = SDL_GetTicks();
  dsec = (t - startTime) / 100;
  minutes = dsec / 600;
  seconds = dsec % 600 / 10;
  dsec = dsec % 600 % 10;
  glPrLine(1, false, 1, true, 0, "%02i:%02i.%i", minutes, seconds, dsec);
  if (t - frame_t0 >= 250)
  {
    float secs = (t - frame_t0) / 1000.0f;
    fps = (int)(frames / secs);
    frames = 0;
    frame_t0 = t;
  }
  if(printFPS)
    glPrLine(1, true, 1, true, 0, "%d fps", fps);

  /* Swap the drawing buffers (draw the back buffer to the screen) */
  SDL_GL_SwapBuffers( );

  camWrt = ((int)++camWrt % ((int)(CAM_FRAME_DELAY + camBase))) 
    ? camWrt : camBase;

  /* Check for collisions */
  /*if (!(frames % 4))*/
  CheckCollision();
}
