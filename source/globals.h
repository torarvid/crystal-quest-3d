/*****************************************************************************\ 
 *
 * Crystal Quest 3D
 * Copyright (C) 2002 Tor Arvid Lund
 *
 * $Id: globals.h,v 1.1 2003/02/22 19:48:57 totto Exp $
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
 * This is the header file describing global defines and data types
 *
 * Written by Tor Arvid Lund
 *
\*****************************************************************************/ 

/*--- Defines ---------------------------------------------------------------*/
#ifndef CQ3D_HEADER_GLOBALS
#define CQ3D_HEADER_GLOBALS

#define PI 3.141592656898

#define SOUND_ENABLED /* Comment out this line to disable sound... */

#define SC_WIDTH 800 
#define SC_HEIGHT 600
#define SC_BPP 32

#define CAM_FRAME_DELAY 40
#define FRICTION 0.989

#define PRINT_CENTER -1

#define CAM_DIST -5.0
#define WORLD_OBJECTS 30
#define STONE_TYPES 2
#define MAX_STONES 12
#define NUM_CRYSTALS 30
#define	FPS 75
#define PENALTY 15

#define SIM_SOUNDS 8
#define NUM_SOUNDS 4

#define STATE_LOADING 1
#define STATE_MENU_MAIN 2
#define STATE_MENU_SCORES 4
#define STATE_MENU_OPTIONS 8
#define	STATE_PLAYING 16
#define STATE_CRASH 32
#define STATE_DEAD 64
#define STATE_FINISHED 128

#define TICK_INTERVAL (1000/FPS)

/*--- Global Includes -------------------------------------------------------*/

#ifdef WIN32
#include <windows.h>
#endif

#ifdef SOUND_ENABLED
#include "SDL_mixer.h"
#endif

#include "SDL.h"
#include <gl.h>
#include <glu.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "pak_util.h"

/*--- Constants -------------------------------------------------------------*/
const float shipScale = 0.01f;
const float levelScale = 0.1f;
const float stoneScale = 0.5f;
const float crystalScale = 0.09f;
const char pakName[] = "pak0.pak";
const int vMajor = 0;
const int vMinor = 6;
const int vBuild = 0;

/*--- Types -----------------------------------------------------------------*/
typedef struct
{
  float x, y, z;
}vec3f;

typedef struct
{
  float pos[3][3];
}posf;

typedef struct
hiscore{
  char Name[64];
  unsigned int mins;
  unsigned int secs;
  unsigned int dsec;
  hiscore *next;
}hiscore;

typedef hiscore *hs;

class CModel3ds;
class CParticleSystem;

/*--- Global variables ------------------------------------------------------*/
extern CModel3ds *realShip;
extern CModel3ds *collShip;
extern CModel3ds *realWorld[WORLD_OBJECTS];
extern CModel3ds *collWorld[WORLD_OBJECTS];
extern CModel3ds *realStones[MAX_STONES];
extern CModel3ds *collStones[MAX_STONES];
extern CModel3ds *realCrystals[NUM_CRYSTALS];
extern CModel3ds *john;
extern CModel3ds *theEnd;
extern CParticleSystem *explosion;
extern float xSpeed;
extern float ySpeed;
extern float zSpeed;
extern int lives;
extern bool gameStarted;
extern int state;
extern int prevState;
extern GLfloat ex, ey, ez, rx, ry, rz, ux, uy, uz;
extern float upDownAngle;
extern float viewAngle;
extern float rotAngle;
extern int triangle, quad;
extern float xTrans, yTrans, zTrans;
extern float viewMatrix[3][3];
extern float shipMatrix[3][3];
extern FILE *logfile;
extern bool debug;
extern posf *camBase;
extern posf *camRead;
extern posf *camWrt;
extern bool startCam;
extern bool printTempMatrix;
extern bool printFPS;
extern bool drawReal;
extern Uint32 next_time;
extern int numCrystals;
extern bool crash;
extern bool done;
extern int lives;
extern SDL_Surface *surface;
extern GLuint tex;
extern bool finished;
extern bool gameover;
extern int cur_time;
extern int max_bpp;
extern int vidFlags;
extern int fkey, bkey, lkey, rkey;
extern int mouseX, mouseY;
extern int buttons[5];
extern int keys[256];
extern bool isActive;
extern float mouseSense;
extern float xAxis[3], yAxis[3], zAxis[3];
extern float speed;
extern float rotSpeed;
extern vec3f *shipWrt;
extern vec3f *shipRead;
extern vec3f *shipBase;
extern bool startShip;
extern bool drawReal;
extern int sector;
extern CModel3ds *goodies;
extern CModel3ds *stones;
extern FILE *theFiles;
extern bool unpacked;
extern Uint32 startTime;
extern hiscore scores[10];
extern char playerName[16];
extern char *homePath;
extern char hsFName[256];
extern FILE *hiScores;
#ifdef SOUND_ENABLED
extern Mix_Chunk *Wav[NUM_SOUNDS];
#endif

/*--- Global functions ------------------------------------------------------*/
extern void quitProgram(int retValue);
extern void drawScene();
extern void drawMenu();
extern GLvoid glPrint(GLint x, GLint y, int set, const char *fmt, ...);
extern void glPrLine(int xPos, bool rightAdjust, int line, bool botmAdjust,
    int set, const char *fmt, ...);
extern float radToDeg(float rad);
extern void quitProgram(int returnValue);
extern void CheckCollision();
extern void loadIdentity(float Matrix[3][3]);
extern void rotVector(float vector[3], float Matrix[3][3], float angle);
extern void rotX(float Matrix[3][3], float retMatrix[3][3], float angle);
extern void rotY(float Matrix[3][3], float retMatrix[3][3], float angle);
extern void resizeWindow(int width, int height);
extern float degToRad(float deg);
extern void loadStuff();
extern int LoadGLTextures();
extern GLvoid BuildFont();
extern void eventPoll();
extern void init();
extern void buildLists();
extern void drawScene();
extern PAK_file *pak_fopen(const char *fname, FILE *PAK_archive);
extern void pak_fclose(PAK_file *stream);
extern size_t pak_fread(void *ptr, size_t size, size_t nmemb, PAK_file *stream);
extern int pak_fseek(PAK_file *stream, long offset, int whence);
extern long pak_ftell(PAK_file *stream);
extern void usage();
extern void doStateMachine();
extern void stateChange(int newstate);

#endif
