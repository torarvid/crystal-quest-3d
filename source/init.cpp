/*****************************************************************************\ 
 *
 * Crystal Quest 3D
 * Copyright (C) 2002 Tor Arvid Lund
 *
 * $Id: init.cpp,v 1.1 2003/02/22 19:48:57 totto Exp $
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
 * This is the source file for the initialization routines
 *
 * Written by Tor Arvid Lund
 *
\*****************************************************************************/ 


#include "init.h"

/*---------------------------------------------------------------------------*/ 

int initAudio()
{
#ifdef SOUND_ENABLED
  if(-1 == SDL_Init(SDL_INIT_AUDIO)) 
    fprintf(stderr, "%s",SDL_GetError()); 
  /* 44 kHz ,16 bit ,stereo */
  if(Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512)) 
  {
    fprintf(stderr, "Mix_OpenAudio() returned an error.\n");
    return 1 ; 
  }
  Mix_ReserveChannels(SIM_SOUNDS); 
  music = Mix_LoadMUS("sounds/bg.ogg");
  for(int i=0; i<NUM_SOUNDS ; i++)
    Wav[i] = Mix_LoadWAV(Sounds[i]); 
  Mix_PlayMusic(music, 50);
  Mix_Volume(0,40); 
  Mix_Volume(1,100); 
  Mix_Volume(2,100); 
  Mix_Volume(3,100); 
  Mix_Volume(4,100);
#endif
  return 0;
}


/*---------------------------------------------------------------------------*/

void initGL()
{
  float ambientLight[] = {1.0f,1.0f,1.0f,1.0f} ;
  float diffuseLight[] = {1.0f,1.0f,1.0f,1.0f} ;
  float LightPosition[] = {0.0f,0.0f,1.0f, 1.0f} ;
  float fogCol[] = {0.3f, 0.3f, 0.3f, 1.0f};

  next_time = SDL_GetTicks() + TICK_INTERVAL;
  /* Enable Gouraud shading */
  glShadeModel(GL_SMOOTH);

  /* Set the background color */
  glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

  /* Enable drawing of polygon backsides */
  glEnable(GL_CULL_FACE);

  /* Depth buffer clear */
  glClearDepth(1.0f);

  /* Enables Depth Testing */
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_FOG);
  glFogi(GL_FOG_MODE, GL_EXP);
  glFogfv(GL_FOG_COLOR, fogCol);
  glFogf(GL_FOG_DENSITY, 0.005f);
  glHint(GL_FOG_HINT, GL_NICEST);
  glFogf(GL_FOG_START, 9.0f);
  glFogf(GL_FOG_END, 15.0f);

  /* The Type Of Depth Test To Do */
  glDepthFunc(GL_LEQUAL);

  /* Nice Perspective Calculations */
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

  glEnable(GL_DITHER);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_POLYGON_SMOOTH);

  glEnable(GL_TEXTURE_2D);

  glEnable(GL_LIGHTING);

  glMaterialfv(GL_FRONT,GL_AMBIENT,ambientLight);
  glMaterialfv(GL_FRONT,GL_AMBIENT,diffuseLight);
  glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
  glLightfv(GL_LIGHT0,GL_POSITION,LightPosition);
  glEnable(GL_LIGHT0);

  SDL_Surface* bitmap = LoadBMP("gfx/loading.bmp");
  glGenTextures(1,&tex);	
  glBindTexture( GL_TEXTURE_2D,tex);
  glTexParameteri( GL_TEXTURE_2D, 
      GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, 
      GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bitmap->w, bitmap->h, 
      GL_RGB, GL_UNSIGNED_BYTE, bitmap->pixels);
  if ( bitmap )
    SDL_FreeSurface( bitmap);
}


/*---------------------------------------------------------------------------*/ 

/* We don't use these... just nice to have around for debugging... */
void buildLists()
{
  triangle = glGenLists(1);

  glNewList(triangle, GL_COMPILE);

  glBegin( GL_TRIANGLES );            /* Drawing Using Triangles */
  glColor3f(0.0f, 1.0f, 0.0);
  glVertex3f(  -1.5f,  1.0f, -6.0f ); /* Top */
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f( -2.5f, -1.0f, -5.0f ); /* Bottom Left */
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(  -0.5f, -1.0f, -5.0f ); /* Bottom Right */

  glColor3f(0.0f, 1.0f, 0.0);
  glVertex3f(  -1.5f,  1.0f, -6.0f ); /* Top */
  glColor3f(1.0f, 0.0f, 1.0f);
  glVertex3f( -1.5f, -1.0f, -7.0f ); /* Bottom Left */
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(  -2.5f, -1.0f, -5.0f ); /* Bottom Right */

  glColor3f(0.0f, 1.0f, 0.0);
  glVertex3f(  -1.5f,  1.0f, -6.0f ); /* Top */
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f( -0.5f, -1.0f, -5.0f ); /* Bottom Left */
  glColor3f(1.0f, 0.0f, 1.0f);
  glVertex3f(  -1.5f, -1.0f, -7.0f ); /* Bottom Right */

  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(-2.5f, -1.0f, -5.0f ); /* Top */
  glColor3f(1.0f, 0.0f, 1.0);
  glVertex3f( -1.5f, -1.0f, -7.0f ); /* Bottom Left */
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(  -0.5f, -1.0f, -5.0f ); /* Bottom Right */
  glEnd( );                           /* Finished Drawing The Triangle */

  glEndList();

  quad = glGenLists(1);

  glNewList(quad, GL_COMPILE);

  glBegin( GL_QUADS );                /* Draw A Quad */
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f( 0.5f,  1.0f, -6.0f ); /* Top Left */
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex3f(  2.5f,  1.0f, -6.0f ); /* Top Right */
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(  2.5f, -1.0f, -6.0f ); /* Bottom Right */
  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex3f( 0.5f, -1.0f, -6.0f ); /* Bottom Left */
  glEnd( );                           /* Done Drawing The Quad */

  glEndList();
}

/*---------------------------------------------------------------------------*/

void writeFile(const char *fName)
{
  char *contents;
  PAK_file *curFile;
  FILE *outFile;

  curFile = pak_fopen(fName, theFiles);
  contents = (char *)malloc(curFile->size);
  pak_fread(contents, curFile->size, 1, curFile);
  outFile = fopen(fName, "w");
  fwrite(contents, curFile->size, 1, outFile);
  fclose(outFile);
  pak_fclose(curFile);
  fprintf(logfile, "%s successfully unpacked\n", fName);
}

/*---------------------------------------------------------------------------*/

void unpak_files()
{
  mkdir("textures", 0755);
  mkdir("sounds", 0755);
  mkdir("gfx", 0755);
  theFiles = fopen("pak0.pak", "r");
  writeFile("textures/CRYSTAL.BMP");
  writeFile("textures/FLY.BMP");
  writeFile("textures/JOHN.BMP");
  writeFile("textures/MARS1.BMP");
  writeFile("textures/MARS2.BMP");
  writeFile("textures/METAL.BMP");
  writeFile("textures/MOON1.BMP");
  writeFile("textures/MOON2.BMP");
  writeFile("textures/SAND.BMP");
  writeFile("textures/STEIN1.BMP");
  writeFile("textures/STEIN.BMP");
  writeFile("textures/STONE53.BMP");
  writeFile("textures/STONE54.BMP");
  writeFile("sounds/bg.ogg");
  writeFile("sounds/boom.wav");
  writeFile("sounds/crystal.wav");
  writeFile("sounds/fanfare.wav");
  writeFile("gfx/font.bmp");
  writeFile("gfx/particle.bmp");
  writeFile("gfx/loading.bmp");
  unpacked = true;
}

/*---------------------------------------------------------------------------*/

void init()
{
  int i;
  fprintf(logfile, "Unpacking data files...");
  unpak_files();
  fprintf(logfile, "OK\n");

  fprintf(logfile, "Loading sound...");
  i = initAudio();
  fprintf(logfile, "OK\n");

  fprintf(logfile, "Initialising OpenGL...");
  initGL();
  fprintf(logfile, "OK\n\n");

  for(i=0;i<256;i++)
    keys[i] = 0;

  LoadGLTextures();
  BuildFont();

  homePath = getenv("HOME");
  sprintf(hsFName, "%s/.cq3drc", homePath);
  hiScores = fopen(hsFName, "r");
  if(!hiScores)
    fprintf(logfile, "Could not open hiScores file %s\n", hsFName);
  else
  {
    fprintf(logfile, "hiScores file %s opened\n", hsFName);
    for(i=0;i<10;i++)
    {
      fscanf(hiScores, "%s = %d %d %d", scores[i].Name, &scores[i].mins,
	  &scores[i].secs, &scores[i].dsec);
    }
    fclose(hiScores);
  }
}
