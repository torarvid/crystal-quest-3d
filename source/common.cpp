/*****************************************************************************\ 
 *
 * Crystal Quest 3D
 * Copyright (C) 2002 Tor Arvid Lund
 *
 * $Id: common.cpp,v 1.1 2003/02/22 19:48:57 totto Exp $
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
 * This is the source file describing common functions (obviously...)
 *
 * Written by Tor Arvid Lund
 *
\*****************************************************************************/ 


#include <time.h>
#include "common.h"

/*---------------------------------------------------------------------------*/

void stateChange(const int newstate)
{
  prevState = state;
  static Uint32 tempTime;
  
  switch(newstate)
  {
    case(STATE_LOADING):
      break;
    case(STATE_MENU_MAIN):
      SDL_ShowCursor(SDL_ENABLE);
      if(gameStarted)
	tempTime = SDL_GetTicks();
      break;
    case(STATE_MENU_SCORES):
      break;
    case(STATE_MENU_OPTIONS):
      break;
    case(STATE_PLAYING):
      if(!gameStarted)
	startTime = SDL_GetTicks();
      else
	startTime += SDL_GetTicks() - tempTime;
      gameStarted = true;
      playerName[0] = 0;
      SDL_ShowCursor(SDL_DISABLE);
      /*realShip->Load();*/
      break;
    case(STATE_CRASH):
      break;
    case(STATE_DEAD):
      break;
    case(STATE_FINISHED):
      ;
  }

  state = newstate;
}

/*---------------------------------------------------------------------------*/

void doStateMachine()
{
  switch(state)
  {
    case(STATE_LOADING):
      loadStuff();
      break;
    case(STATE_MENU_MAIN):
      if(isActive)
	drawMenu();
      break;
    case(STATE_MENU_SCORES):
      if(isActive)
	drawMenu();
      break;
    case(STATE_MENU_OPTIONS):
      break;
    case(STATE_PLAYING):
      if(isActive)
	drawScene();
      break;
    case(STATE_CRASH):
      break;
    case(STATE_DEAD):
      break;
    case(STATE_FINISHED):
      ;
  }
}

/*---------------------------------------------------------------------------*/

void usage()
{
  fprintf(stdout, "\
Usage:\n\
	cq3d [OPTIONS]\n\
\n\
OPTIONS:\n\
	-f, --full-screen	Start the game in 'full-screen' mode\n\
	-d, --debug		Print status information to stdout\n\
				instead of logfile\n\
	-v, --version		Print version information\n\
	-h, --help		Print this help message\n\
\n\
In-game hotkeys:\n\
	f	forward\n\
	d	backward (cheating, really...)\n\
	s	roll right\n\
	a	roll left\n\
	mouse	change viewing direction\n\
\n\
	x	show fps\n\
	r	reload ship model if it starts out garbled... (BUG)\n\
	ESC	quit\n\
");

  exit(0);
}

/*---------------------------------------------------------------------------*/

int LoadGLTextures()
{
  /* Status indicator */
  int Status = false;

  /* Create storage space for the texture */
  SDL_Surface *TextureImage[1];

  /* Load The Bitmap, Check For Errors, If Bitmap's Not Found -> Quit */
  if ((TextureImage[0] = SDL_LoadBMP("gfx/font.bmp")))
  {
    /* Set the status to true */
    Status = true;

    /* Create The Texture */
    glGenTextures(1, &fontTexture);

    glBindTexture( GL_TEXTURE_2D, fontTexture);
    glTexImage2D( GL_TEXTURE_2D, 0, 3,
	TextureImage[0]->w,
	TextureImage[0]->h, 0, GL_RGB,
	GL_UNSIGNED_BYTE,
	TextureImage[0]->pixels );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
	GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
	GL_LINEAR);

    /* Loop through 2 local Textures */
    if ( TextureImage[0] )
      SDL_FreeSurface( TextureImage[0] );
  }

  return Status;
}


/*---------------------------------------------------------------------------*/

/* function to recover memory form our list of characters */
GLvoid KillFont(void)
{
  glDeleteLists(base, 96);

  return;
}


/*---------------------------------------------------------------------------*/

/* function to build our font list */
GLvoid BuildFont(void)
{
  float cx;    /* Holds Our X Character Coord */
  float cy;    /* Holds Our Y Character Coord */

  /* Creating 256 Display List */
  base = glGenLists( 256 );
  /* Select Our Font Texture */
  glBindTexture( GL_TEXTURE_2D, fontTexture );

  /* Loop Through All 256 Lists */
  for ( int loop1 = 0; loop1 < 256; loop1++ )
  {
    /* NOTE:
     *  BMPs are stored with the top-leftmost pixel being the
     * last byte and the bottom-rightmost pixel being the first
     * byte. So an image that is displayed as
     *    1 0
     *    0 0
     * is represented data-wise like
     *    0 0
     *    0 1
     * And because SDL_LoadBMP loads the raw data without
     * translating to how it is thought of when viewed we need
     * to start at the bottom-right corner of the data and work
     * backwards to get everything properly. So the below code has
     * been modified to reflect this. Examine how this is done and
     * how the original tutorial is done to grasp the differences.
     *
     * As a side note BMPs are also stored as BGR instead of RGB
     * and that is why we load the texture using GL_BGR. It's
     * bass-ackwards I know but whattaya gonna do?
     */

    /* X Position Of Current Character */
    cx = 1 - ( float )( loop1 % 16 ) / 16.0f;
    /* Y Position Of Current Character */
    cy = 1 - ( float )( loop1 / 16 ) / 16.0f;

    /* Start Building A List */
    glNewList( base + ( 255 - loop1 ), GL_COMPILE );
    /* Use A Quad For Each Character */
    glBegin( GL_QUADS );
    /* Texture Coord (Bottom Left) */
    glTexCoord2f( cx - 0.0625, cy );
    /* Vertex Coord (Bottom Left) */
    glVertex2i( 0, 16 );

    /* Texture Coord (Bottom Right) */
    glTexCoord2f( cx, cy );
    /* Vertex Coord (Bottom Right) */
    glVertex2i( 16, 16 );

    /* Texture Coord (Top Right) */
    glTexCoord2f( cx, cy - 0.0625f );
    /* Vertex Coord (Top Right) */
    glVertex2i( 16, 0 );

    /* Texture Coord (Top Left) */
    glTexCoord2f( cx - 0.0625f, cy - 0.0625f);
    /* Vertex Coord (Top Left) */
    glVertex2i( 0, 0 );
    glEnd( );

    /* Move To The Left Of The Character */
    glTranslated( 15, 0, 0 );
    glEndList( );
  }
}


/*---------------------------------------------------------------------------*/

/* function to print a string */
GLvoid glPrint(GLint x, GLint y, int set, const char *fmt, ...)
{
  char text[256]; /* Holds Our String */
  va_list ap;     /* Pointer To List Of Arguments */

  /* If There's No Text */
  if ( fmt == NULL )
    return;

  glPushAttrib(GL_CURRENT_BIT);
  glDisable(GL_LIGHTING);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);


  /* Parses The String For Variables */
  va_start( ap, fmt );
  /* Converts Symbols To Actual Numbers */
  vsprintf( text, fmt, ap );
  va_end( ap );

  /* Did User Choose An Invalid Character Set? */
  if ( set > 1 )
    set = 1;

  /* Select our texture */
  glBindTexture( GL_TEXTURE_2D, fontTexture );
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  /* Disable depth testing */
  glDisable( GL_DEPTH_TEST );

  /* Select The Projection Matrix */
  glMatrixMode( GL_PROJECTION );
  /* Store The Projection Matrix */
  glPushMatrix( );

  /* Reset The Projection Matrix */
  glLoadIdentity( );
  /* Set Up An Ortho Screen */
  glOrtho(0, SC_WIDTH, 0, SC_HEIGHT, 0, 1);

  /* Select The Modelview Matrix */
  glMatrixMode( GL_MODELVIEW );
  /* Stor the Modelview Matrix */
  glPushMatrix( );


  /* Reset The Modelview Matrix */
  glLoadIdentity( );
  /* Position The Text (0,0 - Bottom Left) */
  glTranslated( x, y, 0 );
  glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
  glRotatef(180.0f, 0.0f, 0.0f, 1.0f);

  /* Choose The Font Set (0 or 1) */
  glListBase( base - 32 + ( 128 * set ) );

  /* If Set 0 Is Being Used Enlarge Font */
  if ( set == 0 )
    /* Enlarge Font Width And Height */
    glScalef( 1.2f, 1.5f, 1.0f );
  else
    glScalef(2.0f, 2.0f, 1.0f);

  /* Write The Text To The Screen */
  glCallLists( strlen( text ), GL_BYTE, text );
  /* Disable Texture Mapping */
  glMatrixMode( GL_PROJECTION );
  /* Restore The Old Projection Matrix */
  glPopMatrix( );

  /* Select the Modelview Matrix */
  glMatrixMode( GL_MODELVIEW );
  /* Restore the Old Projection Matrix */
  glPopMatrix( );

  glPopAttrib();


  /* Re-enable Depth Testing */
  glEnable( GL_DEPTH_TEST );
  glDisable(GL_BLEND);

  return;
}

/*---------------------------------------------------------------------------*/

void glPrLine(int xChar, bool rightAdjust, int line, bool botmAdjust,
    int set, const char *fmt, ...)
{
  char text[256]; /* Holds Our String */
  va_list ap;     /* Pointer To List Of Arguments */
  int xPos, yPos;
  int lineDist = 30;
  int letterDist = 18 + set * 12;

  /* If There's No Text */
  if ( fmt == NULL )
    return;

  va_start( ap, fmt );
  vsprintf( text, fmt, ap );
  va_end( ap );

  if (xChar == PRINT_CENTER)
  {
    int len = strlen(text) * letterDist;
    xPos = (int)(SC_WIDTH / 2 - len / 2);
  }
  else if (rightAdjust)
    xPos = SC_WIDTH - (strlen(text) + xChar) * letterDist;
  else
    xPos = xChar * letterDist;
  if (botmAdjust)
    yPos = line * lineDist;
  else
    yPos = SC_HEIGHT - line * lineDist;
  glPrint(xPos, yPos, set, text);
}


/*---------------------------------------------------------------------------*/

void loadStuff()
{
  char realLevelFName[40];
  char collLevelFName[40];
  char realStoneFName[STONE_TYPES][40];
  char collStoneFName[STONE_TYPES][40];
  int i;
  /*int total = WORLD_OBJECTS + MAX_STONES + NUM_CRYSTALS + 3;*/

  SDL_WarpMouse(SC_WIDTH / 2, SC_HEIGHT / 2);

  drawMenu();

  realShip = new CModel3ds("model_real/ship.3DS", 0);
  realShip->m_Scale = shipScale;
  realShip->Load();

  for(i=0;i<WORLD_OBJECTS;i++)
  {
    sprintf(realLevelFName, "model_real/brett%02d.3DS", i);
    sprintf(collLevelFName, "model_coll/kollisjon%02d.3DS", i);
    realWorld[i] = new CModel3ds(realLevelFName, 0);
    realWorld[i]->m_Scale = levelScale;
    realWorld[i]->Load();
    collWorld[i] = new CModel3ds(collLevelFName, 0);
    collWorld[i]->m_Scale = levelScale;
    collWorld[i]->Load();
    drawMenu();
  }

  for(i=0;i<STONE_TYPES;i++)
  {
    sprintf(realStoneFName[i], "model_real/stein%d.3DS", i+2);
    sprintf(collStoneFName[i], "model_coll/kstein%d.3DS", i+2);
    drawMenu();
  }

  for(i=0;i<5;i++)
  {
    realStones[i] = new CModel3ds(realStoneFName[1], 0, STONE_BREAKABLE);
    realStones[i]->m_Scale = stoneScale;
    realStones[i]->Load();
    collStones[i] = new CModel3ds(collStoneFName[1], 0, STONE_BREAKABLE);
    collStones[i]->m_Scale = stoneScale;
    collStones[i]->Load();
    drawMenu();
  }

  for(i=5;i<13;i++)
  {
    realStones[i] = new CModel3ds(realStoneFName[0], 0, STONE_BREAKABLE);
    realStones[i]->m_Scale = stoneScale;
    realStones[i]->Load();
    collStones[i] = new CModel3ds(collStoneFName[0], 0, STONE_BREAKABLE);
    collStones[i]->m_Scale = stoneScale;
    collStones[i]->Load();
    drawMenu();
  }

  realStones[0]->m_Origin.x = 300.0f;
  realStones[0]->m_Origin.y = 500.0f;
  realStones[0]->m_Origin.z = 0.0f;
  realStones[0]->stoneFactor = 30.0f;
  realStones[0]->stoneTilt = 0.0f;
  realStones[0]->stoneOnZAxis = false;
  realStones[0]->collRef = collStones[0];
  realStones[0]->collRef->m_Origin = realStones[0]->m_Origin;

  realStones[1]->m_Origin.x = realWorld[6]->m_Center.x * realWorld[6]->m_Scale
    + 10.0f;
  realStones[1]->m_Origin.y = realWorld[6]->m_Center.y * realWorld[6]->m_Scale
    - 40.0f;
  realStones[1]->m_Origin.z = 50.0f;
  realStones[1]->stoneFactor = 40.0f;
  realStones[1]->stoneTilt = 45.0f;
  realStones[1]->stoneOnZAxis = true;
  realStones[1]->collRef = collStones[1];
  realStones[1]->collRef->m_Origin = realStones[1]->m_Origin;

  realStones[2]->m_Origin.x = 2935.0f;
  realStones[2]->m_Origin.y = 535.0f;
  realStones[2]->m_Origin.z = -140.0f;
  realStones[2]->stoneFactor = 30.0f;
  realStones[2]->stoneTilt = 40.0f;
  realStones[2]->stoneOnZAxis = false;
  realStones[2]->collRef = collStones[2];
  realStones[2]->collRef->m_Origin = realStones[2]->m_Origin;

  realStones[3]->m_Origin.x = 1275.0f;
  realStones[3]->m_Origin.y = 395.0f;
  realStones[3]->m_Origin.z = 280.0f;
  realStones[3]->stoneFactor = 30.0f;
  realStones[3]->stoneTilt = 65.0f;
  realStones[3]->stoneOnZAxis = false;
  realStones[3]->collRef = collStones[3];
  realStones[3]->collRef->m_Origin = realStones[3]->m_Origin;

  realStones[4]->m_Origin.x = 1510.0f;
  realStones[4]->m_Origin.y = 565.0f;
  realStones[4]->m_Origin.z = 280.0f;
  realStones[4]->stoneFactor = 30.0f;
  realStones[4]->stoneTilt = -20.0f;
  realStones[4]->stoneOnZAxis = true;
  realStones[4]->collRef = collStones[4];
  realStones[4]->collRef->m_Origin = realStones[4]->m_Origin;

  realStones[5]->m_Origin.x = 1300.0f;
  realStones[5]->m_Origin.y = 535.0f;
  realStones[5]->m_Origin.z = 415.0f;
  realStones[5]->stoneFactor = 30.0f;
  realStones[5]->stoneTilt = 90.0f;
  realStones[5]->stoneOnZAxis = false;
  realStones[5]->collRef = collStones[5];
  realStones[5]->collRef->m_Origin = realStones[5]->m_Origin;

  realStones[6]->m_Origin.x = 1465.0f;
  realStones[6]->m_Origin.y = 355.0f;
  realStones[6]->m_Origin.z = 390.0f;
  realStones[6]->stoneFactor = 30.0f;
  realStones[6]->stoneTilt = 50.0f;
  realStones[6]->stoneOnZAxis = true;
  realStones[6]->collRef = collStones[6];
  realStones[6]->collRef->m_Origin = realStones[6]->m_Origin;

  realStones[7]->m_Origin.x = 2560.0f;
  realStones[7]->m_Origin.y = 590.0f;
  realStones[7]->m_Origin.z = -115.0f;
  realStones[7]->stoneFactor = 30.0f;
  realStones[7]->stoneTilt = -30.0f;
  realStones[7]->stoneOnZAxis = false;
  realStones[7]->collRef = collStones[7];
  realStones[7]->collRef->m_Origin = realStones[7]->m_Origin;

  realStones[8]->m_Origin.x = 2755.0f;
  realStones[8]->m_Origin.y = 590.0f;
  realStones[8]->m_Origin.z = -125.0f;
  realStones[8]->stoneFactor = 30.0f;
  realStones[8]->stoneTilt = 0.0f;
  realStones[8]->stoneOnZAxis = true;
  realStones[8]->collRef = collStones[8];
  realStones[8]->collRef->m_Origin = realStones[8]->m_Origin;

  realStones[9]->m_Origin.x = 2725.0f;
  realStones[9]->m_Origin.y = 455.0f;
  realStones[9]->m_Origin.z = -165.0f;
  realStones[9]->stoneFactor = 30.0f;
  realStones[9]->stoneTilt = -75.0f;
  realStones[9]->stoneOnZAxis = false;
  realStones[9]->collRef = collStones[9];
  realStones[9]->collRef->m_Origin = realStones[9]->m_Origin;

  realStones[10]->m_Origin.x = 2675.0f;
  realStones[10]->m_Origin.y = 530.0f;
  realStones[10]->m_Origin.z = -20.0f;
  realStones[10]->stoneFactor = 30.0f;
  realStones[10]->stoneTilt = 20.0f;
  realStones[10]->stoneOnZAxis = true;
  realStones[10]->collRef = collStones[10];
  realStones[10]->collRef->m_Origin = realStones[10]->m_Origin;

  realStones[11]->m_Origin.x = 2805.0f;
  realStones[11]->m_Origin.y = 495.0f;
  realStones[11]->m_Origin.z = -315.0f;
  realStones[11]->stoneFactor = 30.0f;
  realStones[11]->stoneTilt = -20.0f;
  realStones[11]->stoneOnZAxis = false;
  realStones[11]->collRef = collStones[11];
  realStones[11]->collRef->m_Origin = realStones[11]->m_Origin;


  for(i=0;i<NUM_CRYSTALS;i++)
  {
    realCrystals[i] = new CModel3ds("model_real/crystal.3DS", 0, CRYSTAL);
    realCrystals[i]->m_Scale = crystalScale;
    realCrystals[i]->Load();
    drawMenu();
  }

  realCrystals[0]->m_Origin.x = 250.0f;
  realCrystals[0]->m_Origin.y = 400.0f;
  realCrystals[0]->m_Origin.z = 0.0f;
  realCrystals[0]->m_Pos = realCrystals[0]->m_Origin;
  realCrystals[0]->stoneFactor = 0.0f;
  realCrystals[0]->stoneTilt = 0.0f;
  realCrystals[0]->stoneOnZAxis = false;

  realCrystals[1]->m_Origin.x = 0.0f;
  realCrystals[1]->m_Origin.y = 290.0f;
  realCrystals[1]->m_Origin.z = 0.0f;
  realCrystals[1]->m_Pos = realCrystals[1]->m_Origin;
  realCrystals[1]->stoneFactor = 0.0f;
  realCrystals[1]->stoneTilt = 0.0f;
  realCrystals[1]->stoneOnZAxis = false;

  realCrystals[2]->m_Origin.x = 175.0f;
  realCrystals[2]->m_Origin.y = 405.0f;
  realCrystals[2]->m_Origin.z = -20.0f;
  realCrystals[2]->m_Pos = realCrystals[2]->m_Origin;
  realCrystals[2]->stoneFactor = 0.0f;
  realCrystals[2]->stoneTilt = 0.0f;
  realCrystals[2]->stoneOnZAxis = false;

  realCrystals[3]->m_Origin.x = 255.0f;
  realCrystals[3]->m_Origin.y = 425.0f;
  realCrystals[3]->m_Origin.z = -10.0f;
  realCrystals[3]->m_Pos = realCrystals[3]->m_Origin;
  realCrystals[3]->stoneFactor = 0.0f;
  realCrystals[3]->stoneTilt = 0.0f;
  realCrystals[3]->stoneOnZAxis = false;

  realCrystals[4]->m_Origin.x = 335.0f;
  realCrystals[4]->m_Origin.y = 390.0f;
  realCrystals[4]->m_Origin.z = -30.0f;
  realCrystals[4]->m_Pos = realCrystals[4]->m_Origin;
  realCrystals[4]->stoneFactor = 0.0f;
  realCrystals[4]->stoneTilt = 0.0f;
  realCrystals[4]->stoneOnZAxis = false;

  realCrystals[5]->m_Origin.x = 745.0f;
  realCrystals[5]->m_Origin.y = 435.0f;
  realCrystals[5]->m_Origin.z = 70.0f;
  realCrystals[5]->m_Pos = realCrystals[5]->m_Origin;
  realCrystals[5]->stoneFactor = 0.0f;
  realCrystals[5]->stoneTilt = 0.0f;
  realCrystals[5]->stoneOnZAxis = false;

  realCrystals[6]->m_Origin.x = 1290.0f;
  realCrystals[6]->m_Origin.y = 305.0f;
  realCrystals[6]->m_Origin.z = 255.0f;
  realCrystals[6]->m_Pos = realCrystals[6]->m_Origin;
  realCrystals[6]->stoneFactor = 0.0f;
  realCrystals[6]->stoneTilt = 0.0f;
  realCrystals[6]->stoneOnZAxis = false;

  realCrystals[7]->m_Origin.x = 1460.0f;
  realCrystals[7]->m_Origin.y = 420.0f;
  realCrystals[7]->m_Origin.z = 275.0f;
  realCrystals[7]->m_Pos = realCrystals[7]->m_Origin;
  realCrystals[7]->stoneFactor = 0.0f;
  realCrystals[7]->stoneTilt = 0.0f;
  realCrystals[7]->stoneOnZAxis = false;

  realCrystals[8]->m_Origin.x = 1395.0f;
  realCrystals[8]->m_Origin.y = 480.0f;
  realCrystals[8]->m_Origin.z = 365.0f;
  realCrystals[8]->m_Pos = realCrystals[8]->m_Origin;
  realCrystals[8]->stoneFactor = 0.0f;
  realCrystals[8]->stoneTilt = 0.0f;
  realCrystals[8]->stoneOnZAxis = false;

  realCrystals[9]->m_Origin.x = 1705.0f;
  realCrystals[9]->m_Origin.y = 435.0f;
  realCrystals[9]->m_Origin.z = 85.0f;
  realCrystals[9]->m_Pos = realCrystals[9]->m_Origin;
  realCrystals[9]->stoneFactor = 0.0f;
  realCrystals[9]->stoneTilt = 0.0f;
  realCrystals[9]->stoneOnZAxis = false;

  realCrystals[10]->m_Origin.x = 1750.0f;
  realCrystals[10]->m_Origin.y = 440.0f;
  realCrystals[10]->m_Origin.z = -35.0f;
  realCrystals[10]->m_Pos = realCrystals[10]->m_Origin;
  realCrystals[10]->stoneFactor = 0.0f;
  realCrystals[10]->stoneTilt = 0.0f;
  realCrystals[10]->stoneOnZAxis = false;

  realCrystals[11]->m_Origin.x = 1880.0f;
  realCrystals[11]->m_Origin.y = 440.0f;
  realCrystals[11]->m_Origin.z = -85.0f;
  realCrystals[11]->m_Pos = realCrystals[11]->m_Origin;
  realCrystals[11]->stoneFactor = 0.0f;
  realCrystals[11]->stoneTilt = 0.0f;
  realCrystals[11]->stoneOnZAxis = false;

  realCrystals[12]->m_Origin.x = 1975.0f;
  realCrystals[12]->m_Origin.y = 440.0f;
  realCrystals[12]->m_Origin.z = -90.0f;
  realCrystals[12]->m_Pos = realCrystals[12]->m_Origin;
  realCrystals[12]->stoneFactor = 0.0f;
  realCrystals[12]->stoneTilt = 0.0f;
  realCrystals[12]->stoneOnZAxis = false;

  realCrystals[13]->m_Origin.x = 2070.0f;
  realCrystals[13]->m_Origin.y = 440.0f;
  realCrystals[13]->m_Origin.z = -80.0f;
  realCrystals[13]->m_Pos = realCrystals[13]->m_Origin;
  realCrystals[13]->stoneFactor = 0.0f;
  realCrystals[13]->stoneTilt = 0.0f;
  realCrystals[13]->stoneOnZAxis = false;

  realCrystals[14]->m_Origin.x = 2110.0f;
  realCrystals[14]->m_Origin.y = 375.0f;
  realCrystals[14]->m_Origin.z = -150.0f;
  realCrystals[14]->m_Pos = realCrystals[14]->m_Origin;
  realCrystals[14]->stoneFactor = 0.0f;
  realCrystals[14]->stoneTilt = 0.0f;
  realCrystals[14]->stoneOnZAxis = false;

  realCrystals[15]->m_Origin.x = 2185.0f;
  realCrystals[15]->m_Origin.y = 355.0f;
  realCrystals[15]->m_Origin.z = -160.0f;
  realCrystals[15]->m_Pos = realCrystals[15]->m_Origin;
  realCrystals[15]->stoneFactor = 0.0f;
  realCrystals[15]->stoneTilt = 0.0f;
  realCrystals[15]->stoneOnZAxis = false;

  realCrystals[16]->m_Origin.x = 2225.0f;
  realCrystals[16]->m_Origin.y = 425.0f;
  realCrystals[16]->m_Origin.z = -155.0f;
  realCrystals[16]->m_Pos = realCrystals[16]->m_Origin;
  realCrystals[16]->stoneFactor = 0.0f;
  realCrystals[16]->stoneTilt = 0.0f;
  realCrystals[16]->stoneOnZAxis = false;

  realCrystals[17]->m_Origin.x = 2315.0f;
  realCrystals[17]->m_Origin.y = 470.0f;
  realCrystals[17]->m_Origin.z = -165.0f;
  realCrystals[17]->m_Pos = realCrystals[17]->m_Origin;
  realCrystals[17]->stoneFactor = 0.0f;
  realCrystals[17]->stoneTilt = 0.0f;
  realCrystals[17]->stoneOnZAxis = false;

  realCrystals[18]->m_Origin.x = 2350.0f;
  realCrystals[18]->m_Origin.y = 550.0f;
  realCrystals[18]->m_Origin.z = -165.0f;
  realCrystals[18]->m_Pos = realCrystals[18]->m_Origin;
  realCrystals[18]->stoneFactor = 0.0f;
  realCrystals[18]->stoneTilt = 0.0f;
  realCrystals[18]->stoneOnZAxis = false;

  realCrystals[19]->m_Origin.x = 2495.0f;
  realCrystals[19]->m_Origin.y = 570.0f;
  realCrystals[19]->m_Origin.z = -165.0f;
  realCrystals[19]->m_Pos = realCrystals[19]->m_Origin;
  realCrystals[19]->stoneFactor = 0.0f;
  realCrystals[19]->stoneTilt = 0.0f;
  realCrystals[19]->stoneOnZAxis = false;

  realCrystals[20]->m_Origin.x = 2650.0f;
  realCrystals[20]->m_Origin.y = 445.0f;
  realCrystals[20]->m_Origin.z = -200.0f;
  realCrystals[20]->m_Pos = realCrystals[20]->m_Origin;
  realCrystals[20]->stoneFactor = 0.0f;
  realCrystals[20]->stoneTilt = 0.0f;
  realCrystals[20]->stoneOnZAxis = false;

  realCrystals[21]->m_Origin.x = 2600.0f;
  realCrystals[21]->m_Origin.y = 635.0f;
  realCrystals[21]->m_Origin.z = -245.0f;
  realCrystals[21]->m_Pos = realCrystals[21]->m_Origin;
  realCrystals[21]->stoneFactor = 0.0f;
  realCrystals[21]->stoneTilt = 0.0f;
  realCrystals[21]->stoneOnZAxis = false;

  realCrystals[22]->m_Origin.x = 2755.0f;
  realCrystals[22]->m_Origin.y = 650.0f;
  realCrystals[22]->m_Origin.z = -240.0f;
  realCrystals[22]->m_Pos = realCrystals[22]->m_Origin;
  realCrystals[22]->stoneFactor = 0.0f;
  realCrystals[22]->stoneTilt = 0.0f;
  realCrystals[22]->stoneOnZAxis = false;

  realCrystals[23]->m_Origin.x = 2860.0f;
  realCrystals[23]->m_Origin.y = 595.0f;
  realCrystals[23]->m_Origin.z = -165.0f;
  realCrystals[23]->m_Pos = realCrystals[23]->m_Origin;
  realCrystals[23]->stoneFactor = 0.0f;
  realCrystals[23]->stoneTilt = 0.0f;
  realCrystals[23]->stoneOnZAxis = false;

  realCrystals[24]->m_Origin.x = 515.0f;
  realCrystals[24]->m_Origin.y = 435.0f;
  realCrystals[24]->m_Origin.z = 15.0f;
  realCrystals[24]->m_Pos = realCrystals[24]->m_Origin;
  realCrystals[24]->stoneFactor = 0.0f;
  realCrystals[24]->stoneTilt = 0.0f;
  realCrystals[24]->stoneOnZAxis = false;

  realCrystals[25]->m_Origin.x = 550.0f;
  realCrystals[25]->m_Origin.y = 435.0f;
  realCrystals[25]->m_Origin.z = 95.0f;
  realCrystals[25]->m_Pos = realCrystals[25]->m_Origin;
  realCrystals[25]->stoneFactor = 0.0f;
  realCrystals[25]->stoneTilt = 0.0f;
  realCrystals[25]->stoneOnZAxis = false;

  realCrystals[26]->m_Origin.x = 810.0f;
  realCrystals[26]->m_Origin.y = 435.0f;
  realCrystals[26]->m_Origin.z = 80.0f;
  realCrystals[26]->m_Pos = realCrystals[26]->m_Origin;
  realCrystals[26]->stoneFactor = 0.0f;
  realCrystals[26]->stoneTilt = 0.0f;
  realCrystals[26]->stoneOnZAxis = false;

  realCrystals[27]->m_Origin.x = 905.0f;
  realCrystals[27]->m_Origin.y = 435.0f;
  realCrystals[27]->m_Origin.z = 250.0f;
  realCrystals[27]->m_Pos = realCrystals[27]->m_Origin;
  realCrystals[27]->stoneFactor = 0.0f;
  realCrystals[27]->stoneTilt = 0.0f;
  realCrystals[27]->stoneOnZAxis = false;

  realCrystals[28]->m_Origin.x = 1210.0f;
  realCrystals[28]->m_Origin.y = 335.0f;
  realCrystals[28]->m_Origin.z = 415.0f;
  realCrystals[28]->m_Pos = realCrystals[28]->m_Origin;
  realCrystals[28]->stoneFactor = 0.0f;
  realCrystals[28]->stoneTilt = 0.0f;
  realCrystals[28]->stoneOnZAxis = false;

  realCrystals[29]->m_Origin.x = 1375.0f;
  realCrystals[29]->m_Origin.y = 315.0f;
  realCrystals[29]->m_Origin.z = 345.0f;
  realCrystals[29]->m_Pos = realCrystals[29]->m_Origin;
  realCrystals[29]->stoneFactor = 0.0f;
  realCrystals[29]->stoneTilt = 0.0f;
  realCrystals[29]->stoneOnZAxis = false;

  john = new CModel3ds("model_real/john.3DS", 0);
  john->m_Scale = levelScale;
  john->Load();
  drawMenu();

  collShip = new CModel3ds("model_coll/kship.3DS", 0);
  collShip->m_Scale = shipScale;
  collShip->Load();
  drawMenu();

  theEnd = new CModel3ds("model_real/john.3DS", 0);
  theEnd->m_Scale = levelScale;
  theEnd->Load();
  drawMenu();

  Vector3f pos ;
  pos.x =0.0f;
  pos.y =0.0f;
  pos.z =0.0f;

  explosion  = new CParticleSystem(250,pos,"gfx/particle.bmp");
  logString("Explosion created...\n");
  fflush(stdout);
  stateChange(STATE_MENU_MAIN);
}


/*---------------------------------------------------------------------------*/ 

float degToRad(float deg)
{
  return deg * PI / 180.0f;
}


/*---------------------------------------------------------------------------*/ 

float radToDeg(float rad)
{
  return rad / PI * 180.0f;
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
  gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 750.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


/*---------------------------------------------------------------------------*/ 

void quitProgram(int returnValue)
{
#ifdef SOUND_ENABLED
  Mix_FadeOutMusic(700);
#endif
  if(unpacked)
  {
    logString("Quitting\n");
    remove("textures/CRYSTAL.BMP");
    remove("textures/FLY.BMP");
    remove("textures/JOHN.BMP");
    remove("textures/MARS1.BMP");
    remove("textures/MARS2.BMP");
    remove("textures/METAL.BMP");
    remove("textures/MOON1.BMP");
    remove("textures/MOON2.BMP");
    remove("textures/SAND.BMP");
    remove("textures/STEIN1.BMP");
    remove("textures/STEIN.BMP");
    remove("textures/STONE53.BMP");
    remove("textures/STONE54.BMP");
    remove("sounds/bg.ogg");
    remove("sounds/boom.wav");
    remove("sounds/crystal.wav");
    remove("sounds/fanfare.wav");
    remove("gfx/font.bmp");
    remove("gfx/particle.bmp");
    remove("gfx/loading.bmp");
    remove("textures");
    remove("sounds");
    remove("gfx");
  }
  
  hiScores = fopen(hsFName, "w");
  if (!hiScores)
    fprintf(stderr, "Couldn't write Hall of Fame\n");
  else
  {
    int i;
    for(i=0; i<10; i++)
    {
      fprintf(hiScores, "%s = %d %d %d\n",
	  scores[i].Name, scores[i].mins, scores[i].secs, scores[i].dsec);
    }
    fclose(hiScores);
  }
  
  free(camBase);
  free(shipBase);
#ifdef SOUND_ENABLED
  while(Mix_FadingMusic() == MIX_FADING_OUT){};
#endif
  SDL_Quit();
  if(!debug && unpacked)
    fclose(logfile);
  exit(returnValue);
}


/*---------------------------------------------------------------------------*/ 

void rotVector(float vector[3], float inMatrix[3][3], float angle)
{
  int i, j;
  float Matrix[3][3];
  float rotMatrix[3][3];
  float x, y, z, s, c, t, sx, sy, sz, tyz, txz, txy;
  float length[3];

  for(i=0;i<3;i++)
    for(j=0;j<3;j++)
      Matrix[i][j] = inMatrix[i][j];

  x = vector[0];
  y = vector[1];
  z = vector[2];
  s = sin(angle);
  c = cos(angle);
  t = 1.0f - c;

  /*printf("x:%g\ny:%g\nz:%g\n\n", x, y, z);*/
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

  inMatrix[0][0] = Matrix[0][0] * rotMatrix[0][0] +
    Matrix[1][0] * rotMatrix[0][1] + 
    Matrix[2][0] * rotMatrix[0][2];
  inMatrix[0][1] = Matrix[0][1] * rotMatrix[0][0] +
    Matrix[1][1] * rotMatrix[0][1] + 
    Matrix[2][1] * rotMatrix[0][2];
  inMatrix[0][2] = Matrix[0][2] * rotMatrix[0][0] +
    Matrix[1][2] * rotMatrix[0][1] + 
    Matrix[2][2] * rotMatrix[0][2];
  inMatrix[1][0] = Matrix[0][0] * rotMatrix[1][0] +
    Matrix[1][0] * rotMatrix[1][1] + 
    Matrix[2][0] * rotMatrix[1][2];
  inMatrix[1][1] = Matrix[0][1] * rotMatrix[1][0] +
    Matrix[1][1] * rotMatrix[1][1] + 
    Matrix[2][1] * rotMatrix[1][2];
  inMatrix[1][2] = Matrix[0][2] * rotMatrix[1][0] +
    Matrix[1][2] * rotMatrix[1][1] + 
    Matrix[2][2] * rotMatrix[1][2];
  inMatrix[2][0] = Matrix[0][0] * rotMatrix[2][0] +
    Matrix[1][0] * rotMatrix[2][1] + 
    Matrix[2][0] * rotMatrix[2][2];
  inMatrix[2][1] = Matrix[0][1] * rotMatrix[2][0] +
    Matrix[1][1] * rotMatrix[2][1] + 
    Matrix[2][1] * rotMatrix[2][2];
  inMatrix[2][2] = Matrix[0][2] * rotMatrix[2][0] +
    Matrix[1][2] * rotMatrix[2][1] + 
    Matrix[2][2] * rotMatrix[2][2];
  for(i=0;i<2;i++)
    length[i] = sqrt(inMatrix[i][0] * inMatrix[i][0] + 
	inMatrix[i][1] * inMatrix[i][1] +
	inMatrix[i][2] * inMatrix[i][2]);
  for(i=0;i<2;i++)
    for(j=0;j<2;j++)
      inMatrix[i][j] /= length[i];
}


/*---------------------------------------------------------------------------*/ 

void loadIdentity(float Matrix[3][3])
{
  int i, j;

  for(i=0;i<3;i++)
    for(j=0;j<3;j++)
      Matrix[i][j] = (i == j) ? 1.0f : 0.0f;
}


/*---------------------------------------------------------------------------*/

void logString(const char *str, ...)
{
  struct tm *t;
  time_t timep = time(NULL);
  t = localtime(&timep);
  const size_t BUFSIZE = 512;
  char time_buf[BUFSIZE];
  strftime(time_buf, BUFSIZE, "%c", t);
  va_list marker;
  fprintf(logfile, "\n%s: ", time_buf);

  va_start(marker, str);
  vfprintf(logfile, str, marker);
  va_end(marker);
  fflush(logfile);
}
