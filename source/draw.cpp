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
  int i,j;
  static int printMat = 19;
  float tempMatrix[16];

  for(i=0;i<2;i++)
    for(j=0;j<3;j++)
    {
      camWrt->pos[i][j] = viewMatrix[2-i][j] * ((i == 0) ? CAM_DIST : 1);
    }

  tempMatrix[0] = viewMatrix[0][0];
  tempMatrix[1] = viewMatrix[1][0];
  tempMatrix[2] = viewMatrix[2][0];
  tempMatrix[3] = 0.0f;
  tempMatrix[4] = viewMatrix[0][1];
  tempMatrix[5] = viewMatrix[1][1];
  tempMatrix[6] = viewMatrix[2][1];
  tempMatrix[7] = 0.0f;
  tempMatrix[8] = viewMatrix[0][2];
  tempMatrix[9] = viewMatrix[1][2];
  tempMatrix[10] = viewMatrix[2][2];
  tempMatrix[11] = 0.0f;
  tempMatrix[12] = 0.0f;
  tempMatrix[13] = 0.0f;
  tempMatrix[14] = 0.0f;
  tempMatrix[15] = 1.0f;

  camWrt = ((int)++camWrt % ((int)(CAM_FRAME_DELAY + camBase))) 
    ? camWrt : camBase;
  
  /*if (camWrt == camBase)
  quitProgram(0);*/

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

  /*if ((++printMat % 20) == 3)
    printf("r: %f w: %f\n", camRead->pos[1][2], camWrt->pos[1][2]);*/
  /* Rotate camera */

  /*  gluLookAt(viewMatrix[2][0] * -5.0f, 
      viewMatrix[2][1] * -5.0f,
      viewMatrix[2][2] * -5.0f,
      0.0f, 0.0f, 0.0f,
      viewMatrix[1][0], viewMatrix[1][1], viewMatrix[1][2]);*/

  
    
  /*gluLookAt(camWrt->pos[0][0],
      camWrt->pos[0][1],
      camWrt->pos[0][2],
      0.0f, 0.0f, 0.0f,
      camWrt->pos[1][0],
      camWrt->pos[1][1],
      camWrt->pos[1][2]);*/
  
  /* Draw the ship */
  glLoadMatrixf(tempMatrix);
  glTranslatef(0.0f, 0.0f, -5.0f);
  glScalef(shipScale, shipScale, shipScale);
  ourShip->Draw();
  glScalef(1 / shipScale, 1 / shipScale, 1 / shipScale);

  /* Load idendity ModelView matrix */
  glLoadIdentity();

  gluLookAt(camRead->pos[0][0],
      camRead->pos[0][1],
      camRead->pos[0][2],
      0.0f, 0.0f, 0.0f,
      camRead->pos[1][0],
      camRead->pos[1][1],
      camRead->pos[1][2]);

  if (printTempMatrix)
  {
    printf("| %.1f %.1f %.1f %.1f |\n",
	tempMatrix[0], tempMatrix[1], tempMatrix[2], tempMatrix[3]);
    printf("| %.1f %.1f %.1f %.1f |\n",
	tempMatrix[4], tempMatrix[5], tempMatrix[6], tempMatrix[7]);
    printf("| %.1f %.1f %.1f %.1f |\n",
	tempMatrix[8], tempMatrix[9], tempMatrix[10], tempMatrix[11]);
    printf("| %.1f %.1f %.1f %.1f |\n\n",
	tempMatrix[12], tempMatrix[13], tempMatrix[14], tempMatrix[15]);
  }
  if (startCam)
  {
    camRead = ((int)++camRead % ((int)(CAM_FRAME_DELAY + camBase)))
      ? camRead : camBase;
  }

  /* Push ModelView matrix to stack */
  glPushMatrix();

  /* Pop the ModelView matrix from stack */
  glPopMatrix();
  
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
