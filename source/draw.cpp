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
  /* Variables to calculate fps */
  int t;
  static int frame_t0 = 0;
  static int frames = 0;
  float shipScale = 0.01f;
  float shipXAngle, shipYAngle, shipZAngle;

  if (viewMatrix[2][2] == 0)
    shipXAngle = 90.0f;
  else
    shipXAngle = radToDeg(atan(viewMatrix[2][1] / viewMatrix[2][2]));
  
  /* Try to clear the screen as fast as possible */
  glDisable( GL_DEPTH_TEST );
  glDisable( GL_DITHER );
  glShadeModel( GL_FLAT );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_DITHER );
  glShadeModel( GL_SMOOTH );

  /* Load idendity matrix */
  glLoadIdentity();

  /* Rotate camera */
  gluLookAt(viewMatrix[2][0] * -5.0f, 
      viewMatrix[2][1] * -5.0f,
      viewMatrix[2][2] * -5.0f,
      0.0f, 0.0f, 0.0f,
      viewMatrix[1][0], viewMatrix[1][1], viewMatrix[1][2]);

  glRotatef(-upDownAngle, viewMatrix[0][0], viewMatrix[0][1], viewMatrix[0][2]);
  glRotatef(-viewAngle, viewMatrix[1][0], viewMatrix[1][1], viewMatrix[1][2]);
  //glRotatef(-rotAngle, viewMatrix[2][0], viewMatrix[2][1], viewMatrix[2][2]);
  glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
  glScalef(shipScale, shipScale, shipScale);
  ourShip->Draw();
  glScalef(1 / shipScale, 1 / shipScale, 1 / shipScale);
  glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);
  //glRotatef(rotAngle, viewMatrix[2][0], viewMatrix[2][1], viewMatrix[2][2]);
  glRotatef(viewAngle, viewMatrix[1][0], viewMatrix[1][1], viewMatrix[1][2]);
  glRotatef(upDownAngle, viewMatrix[0][0], viewMatrix[0][1], viewMatrix[0][2]);
  
  /* Position ourselves */
  glTranslatef(xTrans, yTrans, zTrans);

  /* Draw stuff */
  glCallList(triangle);
  theLevel->Draw();

  glCallList(quad);

  /*glBegin(GL_TRIANGLES);
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
  glEnd();*/
  
  glBegin(GL_LINES);
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(-1.0f, 1.0f, -6.0f);
  glVertex3f(-1.0f + viewMatrix[0][0], 
      1.0f + viewMatrix[0][1], 
      -6.0f + viewMatrix[0][2]);
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex3f(-1.0f, 1.0f, -6.0f);
  glVertex3f(-1.0f + viewMatrix[1][0], 
      1.0f + viewMatrix[1][1], 
      -6.0f + viewMatrix[1][2]);
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(-1.0f, 1.0f, -6.0f);
  glVertex3f(-1.0f + viewMatrix[2][0], 
      1.0f + viewMatrix[2][1], 
      -6.0f + viewMatrix[2][2]);
  glEnd();

  /* Swap the drawing buffers (draw the back buffer to the screen) */
  SDL_GL_SwapBuffers( );

  /* Calculate fps */
  frames++;
  t = SDL_GetTicks();
  if (t - frame_t0 >= 5000){
    float secs = (t - frame_t0) / 1000.0f;
    printf("%d frames in %g seconds: %g fps\n", 
	   frames, secs, frames / secs);
    frames = 0;
    frame_t0 = t;
  }
}
