/*****************************************************************************\ 
 *
 * Crystal Quest 3D
 * Copyright (C) 2002 Tor Arvid Lund
 *
 * $Id: main.cpp,v 1.1 2003/02/22 19:48:57 totto Exp $
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
 * This is the source file for the main function
 *
 * Written by Tor Arvid Lund
 *
\*****************************************************************************/ 


#include "main.h"

/*---------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
  int i;
  const SDL_VideoInfo *vidInfo;

  fprintf(stdout, "\
This is Crystal Quest 3D\n\
\n\
This program is free software, licensed under the GNU General Public\n\
License. Read more about it in the file GPL that should be distributed\n\
along with this program.\n\
\n\
Copyright (C) 2002 - Tor Arvid Lund\n");
  
  for(i=1;i<argc;i++)
  {
    if ((strcmp(argv[i],"-d") == 0) || (strcmp(argv[i],"--debug")) == 0)
    {
      debug = true;
      fprintf(stderr, "Debug mode on\n");
    }
    else if ((strcmp(argv[i], "-f") == 0) || 
	(strcmp(argv[i], "--full-screen")) == 0)
      vidFlags |= SDL_FULLSCREEN;
    else if ((strcmp(argv[i], "-v") == 0) || 
	(strcmp(argv[i], "--version")) == 0)
    {
      fprintf(stdout, "\nThis is version %d.%d.%d of Crystal Quest 3D\n", 
	  vMajor, vMinor, vBuild);
      exit(0);
    }
    else if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0))
      usage();
    else
    {
      fprintf(stderr, "\nError! Unknown option: %s\n\n", argv[i]);
      usage();
    }
  }
  
  if(debug)
    logfile = stderr;
  else
    logfile = fopen(".log_cq3d", "w");

  /* Struct to hold some video information*/

  camBase = (posf *)malloc(CAM_FRAME_DELAY * sizeof(posf));
  if (camBase == NULL)
  {
    fprintf(stderr, "Cannot allocate memory. Quitting...\n");
    quitProgram(1);
  }
  camRead = camBase;
  camWrt = camBase;

  shipBase = (vec3f *)malloc(CAM_FRAME_DELAY * sizeof(posf));
  if (shipBase == NULL)
  {
    fprintf(stderr, "Cannot allocate memory. Quitting...\n");
    quitProgram(1);
  }
  shipRead = shipBase;
  shipWrt = shipBase;

  /* Initialize the viewing matrix */
  loadIdentity(viewMatrix);
  viewMatrix[2][2] = -1.0f;


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
  vidFlags |= SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE | SDL_RESIZABLE;

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

  /* Sets up OpenGL double buffering */
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  /* Finding the maximum color depth */
  fprintf(logfile, "Checking video mode: %dx%dx%d... ",
      SC_WIDTH, SC_HEIGHT, SC_BPP);
  max_bpp = SDL_VideoModeOK(SC_WIDTH, SC_HEIGHT, SC_BPP, vidFlags);
  fprintf(logfile, "OK\n");

  /* Setting video mode with maximum color depth */
  fprintf(logfile, "Setting video mode: %dx%dx%d... ", 
      SC_WIDTH, SC_HEIGHT, max_bpp);
  surface = SDL_SetVideoMode(SC_WIDTH, SC_HEIGHT, max_bpp, vidFlags);
  SDL_WM_SetCaption("Crystal Quest 3D", NULL);
  fprintf(logfile, "OK\n");

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

  init();
  buildLists();

  /* Request initial window resize */
  resizeWindow(SC_WIDTH, SC_HEIGHT);
  
  fprintf(logfile, "Window set up\n");

  /* Infinite loop which polls for events and draws the scene */
  while(1)
  {
    eventPoll();
    doStateMachine();
  }


  /* Should never get here...*/
  quitProgram(0);

  /* And certainly not here... */
  return 0;
}
