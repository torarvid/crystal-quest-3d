/*****************************************************************************\ 
 *
 * Crystal Quest 3D
 * Copyright (C) 2002 Tor Arvid Lund
 *
 * $Id: event.h,v 1.1 2003/02/22 19:48:57 totto Exp $
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
 * This is the header file for the event polling routines
 *
 * Written by Tor Arvid Lund
 *
\*****************************************************************************/ 


#include "globals.h"
#include "Model3ds.h"
#include "particle.h"

/*---------------------------------------------------------------------------*/ 

/* Up-Down movement */
static float udm = -90.0f;

/* Side-Side movement */
static float ssm = 0.0f;

static int prevTime = 0;
int cur_time = 0;

/* Rotation movement */
static float rm = 0.0f;

/* TEMPORARY - register mouse x and y movement... */
static bool regX = true;
static bool regY = false;

bool printTempMatrix = false;
bool printFPS = false;

/*---------------------------------------------------------------------------*/

void eventPoll();
void processKeys();
