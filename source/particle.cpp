/* Written by Nils Johan Torp */
// ParticleSystem.cpp: implementation of the CParticleSystem class.
//
// $Id: particle.cpp,v 1.1 2003/02/22 19:48:57 totto Exp $
//
//////////////////////////////////////////////////////////////////////

#include "particle.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParticleSystem::CParticleSystem(int nmbr ,Vector3f loc ,char* bmpfile)
{
  m_nParticles = nmbr ;
  m_location.x  =loc.x ;
  m_location.y  =loc.y ;
  m_location.z  =loc.z ;

  m_strFile = bmpfile ;
  m_Frames =0			;
  InitParticles()		;
  GenerateTexture()	; 
  m_nActive = 0;
  m_wait = 40;
}

CParticleSystem::~CParticleSystem()
{

}

void CParticleSystem::GenerateTexture()
{	
  // Temporary surface
  SDL_Surface* bitmap = LoadBMP(m_strFile  );
  glGenTextures(1,&m_texture);	
  glBindTexture( GL_TEXTURE_2D,m_texture );
  // looks better this way ,but slower though
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
      GL_LINEAR_MIPMAP_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
      GL_LINEAR_MIPMAP_LINEAR );
  // generate our texture and mipmaps
  // using this instead of glTexImage2D(,,,,) because 
  // glTexImage2D(,,,,)takes only 2^n pixels(2,4,8,16,32,64.....)
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bitmap->w, bitmap->h, 
      GL_RGB, GL_UNSIGNED_BYTE, bitmap->pixels);
  // free used memory    
  if ( bitmap )
    SDL_FreeSurface( bitmap);

} 

void CParticleSystem::Render()
{
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glDisable(GL_FOG);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  float viewMatrix[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
  Vector3f right = makeVector(viewMatrix[0], viewMatrix[4], viewMatrix[8]);
  Vector3f up = makeVector(viewMatrix[1], viewMatrix[5], viewMatrix[9]);

  glBindTexture(GL_TEXTURE_2D ,m_texture);
  glBegin(GL_QUADS);
  int i;
  int increment = (int)(0.5f + m_nParticles / m_wait);

  if (m_Frames < m_wait)
  {
    for(i=m_nActive;i<m_nActive + increment; i++)
    {
      m_pParticles[i].pos.x = m_location.x + 
	0.005*FRAND*(m_nActive / increment) ;
      m_pParticles[i].pos.y = m_location.y + 
	0.005*FRAND*(m_nActive / increment)  ;
      m_pParticles[i].pos.z = m_location.z + 
	0.005*FRAND*(m_nActive / increment) ;
    }
  }

  for(i=0 ; i< m_nActive;i++){
    if(m_pParticles[i].size >0){
      glColor3f(m_pParticles[i].red ,m_pParticles[i].green,
	  m_pParticles[i].blue );
      Vector3f a = AddVector(right, up);
      float c = -m_pParticles[i].size;
      Vector3f d = MultiplyVectorByScalar(a, c);
      Vector3f b = AddVector(m_pParticles[i].pos, d);

      glTexCoord2f(0.0, 0.0); 
      glVertex3f(b.x, b.y, b.z);

      Vector3f e = MultiplyVectorByScalar(up, -1);
      a = AddVector(right, e);
      c = m_pParticles[i].size;
      d = MultiplyVectorByScalar(a, c);
      b = AddVector(m_pParticles[i].pos, d);
      glTexCoord2f(1.0, 0.0); glVertex3f(b.x, b.y, b.z);

      a = AddVector(right, up);
      c = m_pParticles[i].size;
      d = MultiplyVectorByScalar(a, c);
      b = AddVector(m_pParticles[i].pos, d);
      glTexCoord2f(1.0, 1.0); glVertex3f(b.x, b.y, b.z);

      e = MultiplyVectorByScalar(right, -1);
      a = AddVector(up, e);
      c = m_pParticles[i].size;
      d = MultiplyVectorByScalar(a, c);
      b = AddVector(m_pParticles[i].pos, d);
      glTexCoord2f(0.0, 1.0); glVertex3f(b.x, b.y, b.z);
    }

  }
  glEnd();
  m_Frames++;
  if(m_Frames >m_wait)
    update();
  else
    grow();
  glColor3f(1.0f,1.0f,1.0f);

  m_nActive = ((m_nActive + increment) >= (m_nParticles - increment)) ? 
    m_nParticles - increment: m_nActive + increment;

  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
  glEnable(GL_FOG);
  glEnable(GL_DEPTH_TEST);


}

void CParticleSystem::InitParticles()
{	
  //srand( (unsigned)time( NULL ) );

  //	Alocate memory for our particles
  m_pParticles = new particle[m_nParticles]; 
  for(int i=0 ; i<m_nParticles ; i++){


    m_pParticles[i].red  =  1.0f-FRAND/10;
    m_pParticles[i].green = 0.5f+FRAND/2;
    m_pParticles[i].blue = 0.05f;

    //m_pParticles[i].sizedelta = 0.01f;
    m_pParticles[i].size = 0.0f ;
    m_pParticles[i].pos.x = m_location.x + 0.5*FRAND ;
    m_pParticles[i].pos.y = m_location.y + 0.5*FRAND  ;
    m_pParticles[i].pos.z = m_location.z + 0.5*FRAND ;
    m_pParticles[i].dir.x	= float((rand()%2000)-1000.0f)/100000.0f;
    m_pParticles[i].dir.y	= float((rand()%2000)-1000.0f)/100000.0f;
    m_pParticles[i].dir.z	= 0.0f; //float((rand()%50)-25.0f)/4000.0f;



  }
  //Reset();
}


void CParticleSystem::Reset()
{
  //srand( (unsigned)time( NULL ) );
  m_Frames = 0;
  //	Alocate memory for our particles
  m_pParticles = new particle[m_nParticles]; 
  m_nActive = 0;
  for(int i=0 ; i<m_nParticles ; i++){


    m_pParticles[i].red  =  1.0f;
    m_pParticles[i].green = 0.5f+FRAND/2;
    m_pParticles[i].blue = 0.05f;
    m_pParticles[i].sizedelta = 0.001f ;
    m_pParticles[i].size = 0.3f ;
    m_pParticles[i].pos.x = m_location.x + 0.5*FRAND ;
    m_pParticles[i].pos.y = m_location.y + 0.5*FRAND ;
    m_pParticles[i].pos.z = m_location.z + 0.5*FRAND ;
    m_pParticles[i].dir.x	= float((rand()%2000)-1000.0f)/7000.0f;
    m_pParticles[i].dir.y	= float((rand()%2000)-1000.0f)/7000.0f;
    m_pParticles[i].dir.z	= float((rand()%2000)-1000.0f)/7000.0f;


  }

}

void CParticleSystem::update()
{
  for( int i=0 ; i<m_nParticles ; i++){
    m_pParticles[i].size -= m_pParticles[i].sizedelta ;

    m_pParticles[i].pos.x  += m_pParticles[i].dir.x ;
    m_pParticles[i].pos.y  += m_pParticles[i].dir.y;
    m_pParticles[i].pos.z  += m_pParticles[i].dir.z  ;

  }

}

/* This is a reeeally good function! */
void CParticleSystem::grow()
{
  int i;

  for(i=0;i<m_nParticles;i++)
  {

  }
}
