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
 * This is the source file for the scenes drawing routines
 *
 * Written by Tor Arvid Lund
 *
\*****************************************************************************/ 


#include "draw.h"


/*---------------------------------------------------------------------------*/ 

void drawScene()
{
  int t;
  static int draw_t0 = 0;
  static int frame_t0 = 0;
  static int frames = 0;

  glDisable( GL_DEPTH_TEST );
  glDisable( GL_DITHER );
  glShadeModel( GL_FLAT );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_DITHER );
  glShadeModel( GL_SMOOTH );

  glLoadIdentity();

  gluLookAt(0.0f, 0.0f, 0.0f, 
      xM[2][0], xM[2][1], xM[2][2], 
      xM[1][0], xM[1][1], xM[1][2]);

  glTranslatef(xTrans, yTrans, zTrans);

  glCallList(triangle);

  glCallList(quad);

  glBegin(GL_TRIANGLES);
  glColor3f(0.5f, 1.0f, 0.2f);
  glVertex3f(0.0f, -5.0f, -1.0f);
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex3f(-1.0f, -5.0f, 1.0f);
  glColor3f(1.0f, 0.0f, 0.3f);
  glVertex3f(1.0f, -5.0f, 1.0f);
  glEnd();
  
  glBegin(GL_TRIANGLES);
  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex3f(5.0f, 1.0f, 0.0f);
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(5.0f, -1.0f, -1.0f);
  glColor3f(0.7f, 0.0f, 1.0f);
  glVertex3f(5.0f, -1.0f, 1.0f);
  glEnd();
  
  glBegin(GL_TRIANGLES);
  glColor3f(0.0f, 0.6f, 0.0f);
  glVertex3f(-5.0f, 1.0f, 0.0f);
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(-5.0f, -1.0f, -1.0f);
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(-5.0f, -1.0f, 1.0f);
  glEnd();
  
  glBegin(GL_TRIANGLES);
  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex3f(0.0f, 1.0f, 5.0f);
  glColor3f(0.8f, 1.0f, 0.3f);
  glVertex3f(1.0f, -1.0f, 5.0f);
  glColor3f(0.0f, 0.3f, 0.3f);
  glVertex3f(-1.0f, -1.0f, 5.0f);
  glEnd();
  
  glBegin(GL_TRIANGLES);
  glColor3f(0.0f, 5.0f, 1.0f);
  glVertex3f(0.0f, 5.0f, -1.0f);
  glColor3f(0.0f, 1.0f, 1.0f);
  glVertex3f(-1.0f, 5.0f, 1.0f);
  glColor3f(1.0f, 0.0f, 1.0f);
  glVertex3f(1.0f, 5.0f, 1.0f);
  glEnd();
  
  glBegin(GL_LINES);
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(-1.0f, 1.0f, -6.0f);
  glVertex3f(-1.0f + xM[0][0], 1.0f + xM[0][1], -6.0f + xM[0][2]);
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex3f(-1.0f, 1.0f, -6.0f);
  glVertex3f(-1.0f + xM[1][0], 1.0f + xM[1][1], -6.0f + xM[1][2]);
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(-1.0f, 1.0f, -6.0f);
  glVertex3f(-1.0f + xM[2][0], 1.0f + xM[2][1], -6.0f + xM[2][2]);
  glEnd();

  SDL_GL_SwapBuffers( );

  frames++;
  t = SDL_GetTicks();
  if (t - frame_t0 >= 5000){
    float secs = (t - frame_t0) / 1000.0f;
    printf("%d frames in %g seconds: %g fps\n", 
	   frames, secs, frames / secs);
    frames = 0;
    frame_t0 = t;
  }
  draw_t0 = SDL_GetTicks();
}
