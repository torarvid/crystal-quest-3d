/*****************************************************************************\ 
 *
 * Crystal Quest 3D
 * Copyright (C) 2002 Tor Arvid Lund
 *
 * $Id: collision.cpp,v 1.1 2003/02/22 19:48:57 totto Exp $
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
 * This is the source file describing collision detection routines
 *
 * Written by Tor Arvid Lund
 *
\*****************************************************************************/ 


#include "collision.h"

/*---------------------------------------------------------------------------*/

void FaceCollision(CModel3ds *model)
{
  unsigned int i, j, k, l;
  float f0, f1, t;
  Vector3f v1, v2, cross;
  float A, B, C, D;
  Vector3f p0, p1, intersect;
  int i0, i1;
  float tx[3], ty[3], lx, ly;
  int axis;
  float tCenterX, tCenterY;
  float slope;
  bool slopeX = true;
  static int time1 = SDL_GetTicks();

  /*printf("%i\n", model->m_nFaces * collShip->m_nFaces);*/
  for(i=0;i<model->m_nFaces;i++)
  {
    v1.x = (model->m_Vertices[model->m_Faces[i].vertIndex[1]].x -
        model->m_Vertices[model->m_Faces[i].vertIndex[0]].x) * model->m_Scale;
    v1.y = (model->m_Vertices[model->m_Faces[i].vertIndex[1]].y -
        model->m_Vertices[model->m_Faces[i].vertIndex[0]].y) * model->m_Scale;
    v1.z = (model->m_Vertices[model->m_Faces[i].vertIndex[1]].z -
        model->m_Vertices[model->m_Faces[i].vertIndex[0]].z) * model->m_Scale;
    v2.x = (model->m_Vertices[model->m_Faces[i].vertIndex[2]].x -
        model->m_Vertices[model->m_Faces[i].vertIndex[0]].x) * model->m_Scale;
    v2.y = (model->m_Vertices[model->m_Faces[i].vertIndex[2]].y -
        model->m_Vertices[model->m_Faces[i].vertIndex[0]].y) * model->m_Scale;
    v2.z = (model->m_Vertices[model->m_Faces[i].vertIndex[2]].z -
        model->m_Vertices[model->m_Faces[i].vertIndex[0]].z) * model->m_Scale;
    cross = Cross(v1, v2);

    A = cross.x;
    B = cross.y;
    C = cross.z;
    D = (-A * model->m_Vertices[model->m_Faces[i].vertIndex[1]].x
        - B * model->m_Vertices[model->m_Faces[i].vertIndex[1]].y
        - C * model->m_Vertices[model->m_Faces[i].vertIndex[1]].z) 
      * model->m_Scale;

    for(j=0;j<collShip->m_nFaces;j++)
    {
      for(k=0;k<3;k++)
      {
        int inside = 0;
        i0 = (k==2) ? 1 : 0;
        i1 = (k==0) ? 1 : 2;
        p0.x = collShip->m_Vertices[collShip->m_Faces[j].vertIndex[i0]].x
          * collShip->m_Scale + collShip->m_Pos.x;
        p0.y = collShip->m_Vertices[collShip->m_Faces[j].vertIndex[i0]].y
          * collShip->m_Scale + collShip->m_Pos.y;
        p0.z = collShip->m_Vertices[collShip->m_Faces[j].vertIndex[i0]].z
          * collShip->m_Scale + collShip->m_Pos.z;
        p1.x = collShip->m_Vertices[collShip->m_Faces[j].vertIndex[i1]].x
          * collShip->m_Scale + collShip->m_Pos.x;
        p1.y = collShip->m_Vertices[collShip->m_Faces[j].vertIndex[i1]].y
          * collShip->m_Scale + collShip->m_Pos.y;
        p1.z = collShip->m_Vertices[collShip->m_Faces[j].vertIndex[i1]].z
          * collShip->m_Scale + collShip->m_Pos.z;

        /*printf("%f %f %f %f %f %f\n", p0.x, p0.y, p0.z, p1.x, p1.y, p1.z);*/

        f0 = A * p0.x + B * p0.y + C * p0.z;
        f1 = A * p1.x + B * p1.y + C * p1.z;

        if (f0 == f1)
          continue;

        t = -(f1 + D) / (f0 - f1);

        /*printf("t: %f, f0: %f, f1: %f\n", t, f0, f1);*/
        if ((t<0.0f) || (t>1.0f))
          continue;

        intersect.x = p0.x * t + p1.x * (1-t);
        intersect.y = p0.y * t + p1.y * (1-t);
        intersect.z = p0.z * t + p1.z * (1-t);

        if ((abs((int)cross.x) > abs((int)cross.y)) && 
            (abs((int)cross.x) > abs((int)cross.z)))
          axis = 0;
        else if ((abs((int)cross.y) > abs((int)cross.x)) && 
            (abs((int)cross.y) > abs((int)cross.z)))
          axis = 1;
        else
          axis = 2;

        /*printf("axis: %i\n", axis);*/
        if (axis == 0)
        {
          tx[0] = model->m_Vertices[model->m_Faces[i].vertIndex[0]].y
            * model->m_Scale;
          ty[0] = model->m_Vertices[model->m_Faces[i].vertIndex[0]].z
            * model->m_Scale;
          tx[1] = model->m_Vertices[model->m_Faces[i].vertIndex[1]].y
            * model->m_Scale;
          ty[1] = model->m_Vertices[model->m_Faces[i].vertIndex[1]].z
            * model->m_Scale;
          tx[2] = model->m_Vertices[model->m_Faces[i].vertIndex[2]].y
            * model->m_Scale;
          ty[2] = model->m_Vertices[model->m_Faces[i].vertIndex[2]].z
            * model->m_Scale;
          lx = intersect.y;
          ly = intersect.z;
        }
        else if (axis == 1)
        {
          tx[0] = model->m_Vertices[model->m_Faces[i].vertIndex[0]].x
            * model->m_Scale;
          ty[0] = model->m_Vertices[model->m_Faces[i].vertIndex[0]].z
            * model->m_Scale;
          tx[1] = model->m_Vertices[model->m_Faces[i].vertIndex[1]].x
            * model->m_Scale;
          ty[1] = model->m_Vertices[model->m_Faces[i].vertIndex[1]].z
            * model->m_Scale;
          tx[2] = model->m_Vertices[model->m_Faces[i].vertIndex[2]].x
            * model->m_Scale;
          ty[2] = model->m_Vertices[model->m_Faces[i].vertIndex[2]].z
            * model->m_Scale;
          lx = intersect.x;
          ly = intersect.z;
        }
        else
        {
          tx[0] = model->m_Vertices[model->m_Faces[i].vertIndex[0]].x
            * model->m_Scale;
          ty[0] = model->m_Vertices[model->m_Faces[i].vertIndex[0]].y
            * model->m_Scale;
          tx[1] = model->m_Vertices[model->m_Faces[i].vertIndex[1]].x
            * model->m_Scale;
          ty[1] = model->m_Vertices[model->m_Faces[i].vertIndex[1]].y
            * model->m_Scale;
          tx[2] = model->m_Vertices[model->m_Faces[i].vertIndex[2]].x
            * model->m_Scale;
          ty[2] = model->m_Vertices[model->m_Faces[i].vertIndex[2]].y
            * model->m_Scale;
          lx = intersect.x;
          ly = intersect.y;
        }

        tCenterX = (tx[0] + tx[1] + tx[2])/3;
        tCenterY = (ty[0] + ty[1] + ty[2])/3;

        for(l=0;l<3;l++)
        {
          int a = (l+1) % 3;
          enum {UP, DOWN};
          float b;

          if (tx[a] == tx[l])
          {
            slopeX = false;
            slope = 0.0f;
            b = 0.0f;
          }
          else
          {
            slopeX = true;
            slope = (ty[a] - ty[l]) / (tx[a] - tx[l]);
            b = ty[l] - slope * tx[l];
          }

          if (slopeX)
          {
            if (tCenterY > (slope * tCenterX + b))
            {
              if (ly > (slope * lx + b))
                inside++;
            }
            else if (tCenterY < (slope * tCenterX + b))
            {
              if (ly < (slope * lx + b))
                inside++;
            }
          }
          else
          {
            if ((lx > tx[l]) && (tCenterX > tx[l]))
              inside++;
            else if ((lx < tx[l]) && (tCenterX < tx[l]))
              inside++;
          }
        }

        if (inside == 3)
        {
          if(((SDL_GetTicks() - time1) > 100) && !crash)
          {
#ifdef SOUND_ENABLED
            Mix_PlayChannel(1, Wav[1], 0);
#endif
            time1 = SDL_GetTicks();
            lives--;
            if (lives == 0)
            {
              gameover = true;
              gameStarted = false;
            }
            else
              startTime -= 1000 * PENALTY;
            crash = true;
            xSpeed = ySpeed = zSpeed = 0.0f;
            explosion->Reset();
          }
        }
      }
    }
  }
}


/*---------------------------------------------------------------------------*/

void CheckCollision()
{
  float Dist, RadSum;
  float distX, distY, distZ; 
  int i;
  static int time1 = SDL_GetTicks();

  for(i=0;i<WORLD_OBJECTS;i++)
  {
    distX = realShip->m_Pos.x - 
      collWorld[i]->m_Center.x * collWorld[i]->m_Scale;
    distY = realShip->m_Pos.y - 
      collWorld[i]->m_Center.y * collWorld[i]->m_Scale;
    distZ = realShip->m_Pos.z - 
      collWorld[i]->m_Center.z * collWorld[i]->m_Scale;

    Dist = sqrt(distX * distX + distY * distY + distZ * distZ);

    RadSum = realShip->m_BoundingRadius + collWorld[i]->m_BoundingRadius;

    if ( RadSum > Dist)
    {
      sector = i;
      FaceCollision(collWorld[i]);
    }
  }

  for(i=0;i<MAX_STONES;i++)
  {
    distX = realShip->m_Pos.x - (collStones[i]->m_Center.x * 
        collStones[i]->m_Scale + collStones[i]->m_Pos.x);
    distY = realShip->m_Pos.y - (collStones[i]->m_Center.y *
        collStones[i]->m_Scale + collStones[i]->m_Pos.y);
    distZ = realShip->m_Pos.z - (collStones[i]->m_Center.z * 
        collStones[i]->m_Scale + collStones[i]->m_Pos.z);

    Dist = sqrt(distX * distX + distY * distY + distZ * distZ);

    RadSum = realShip->m_BoundingRadius + collStones[i]->m_BoundingRadius;

    if ( RadSum > Dist)
    {
      if (((SDL_GetTicks() - time1) > 2000) && !crash)
      {
#ifdef SOUND_ENABLED
        Mix_PlayChannel(1, Wav[1], 0);
#endif
        time1 = SDL_GetTicks();
        lives--;
        if (lives == 0)
          gameover = true;
        crash = true;
        xSpeed = ySpeed = zSpeed = 0.0f;
        explosion->Reset();
      }
    }
  }

  for(i=0;i<NUM_CRYSTALS;i++)
  {
    distX = realShip->m_Pos.x - (realCrystals[i]->m_Center.x * 
        realCrystals[i]->m_Scale + realCrystals[i]->m_Pos.x);
    distY = realShip->m_Pos.y - (realCrystals[i]->m_Center.y *
        realCrystals[i]->m_Scale + realCrystals[i]->m_Pos.y);
    distZ = realShip->m_Pos.z - (realCrystals[i]->m_Center.z * 
        realCrystals[i]->m_Scale + realCrystals[i]->m_Pos.z);

    Dist = sqrt(distX * distX + distY * distY + distZ * distZ);

    RadSum = realShip->m_BoundingRadius + realCrystals[i]->m_BoundingRadius;

    if ( RadSum > Dist)
    {
      if(realCrystals[i]->isActive)
      {
#ifdef SOUND_ENABLED
        Mix_Volume(2, 100);
        Mix_PlayChannel(2, Wav[2], 0);
#endif
        time1 = SDL_GetTicks();
        realCrystals[i]->isActive = false;
        numCrystals++;
        if (numCrystals >= NUM_CRYSTALS)
        {
#ifdef SOUND_ENABLED
          Mix_PlayChannel(3, Wav[3], 0);
#endif
          done = true;
        }
      }
    }
  }

  distX = realShip->m_Pos.x - (john->m_Center.x * 
      john->m_Scale + john->m_Pos.x);
  distY = realShip->m_Pos.y - (john->m_Center.y *
      john->m_Scale + john->m_Pos.y);
  distZ = realShip->m_Pos.z - (john->m_Center.z * 
      john->m_Scale + john->m_Pos.z);

  Dist = sqrt(distX * distX + distY * distY + distZ * distZ);

  RadSum = realShip->m_BoundingRadius + john->m_BoundingRadius;

  if ( RadSum > Dist)
  {
    FaceCollision(john);
  }

  distX = realShip->m_Pos.x - (theEnd->m_Center.x * 
      theEnd->m_Scale + theEnd->m_Pos.x);
  distY = realShip->m_Pos.y - (theEnd->m_Center.y *
      theEnd->m_Scale + theEnd->m_Pos.y);
  distZ = realShip->m_Pos.z - (theEnd->m_Center.z * 
      theEnd->m_Scale + theEnd->m_Pos.z);

  Dist = sqrt(distX * distX + distY * distY + distZ * distZ);

  RadSum = realShip->m_BoundingRadius + theEnd->m_BoundingRadius;

  if ( RadSum > Dist)
  {
    if (done)
    {
      finished = true;
    }
  }
}
