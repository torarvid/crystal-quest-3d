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
 * This is the source file for the initialization routines
 *
 * Written by Tor Arvid Lund
 *
\*****************************************************************************/ 


#include "init.h"


/*---------------------------------------------------------------------------*/ 

int triangle;
int quad;


/*---------------------------------------------------------------------------*/ 

void initGL()
{
  /* Enable Gouraud shading */
  glShadeModel(GL_SMOOTH);

  /* Set the background color */
  glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

  /* Depth buffer clear */
  glClearDepth(1.0f);

  /* Enables Depth Testing */
  glEnable(GL_DEPTH_TEST);

  /* The Type Of Depth Test To Do */
  glDepthFunc(GL_LEQUAL);

  /* Nice Perspective Calculations */
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  
  /*glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);*/

  glEnable(GL_DITHER);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_POLYGON_SMOOTH);
}


/*---------------------------------------------------------------------------*/ 

void buildLists()
{
  triangle = glGenLists(2);

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

  quad = triangle + 1;

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
