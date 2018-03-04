/* Written by Nils Johan Torp */
// ParticleSystem.h: interface for the CParticleSystem class.
//
// $Id: particle.h,v 1.1 2003/02/22 19:48:57 totto Exp $
//
//////////////////////////////////////////////////////////////////////


#ifndef CQ3D_HEADER_PARTICLE
#define CQ3D_HEADER_PARTICLE

#include "globals.h"
#include "bitmap.h"
#include "Vector.h"
//#define RAND_MAX 65536
#define FRAND   (((float)rand()-(float)rand())/RAND_MAX)
struct particle{
  Vector3f pos			;	// Holds the current position
  Vector3f dir			;	// Holds the direction 
  float red,green,blue	;	// Holds the color of the particle
  float life				;	// Holds how long the particle will
  float fade				;	// Life is decreased with this until life is zero then the particle is dead
  float size				;
  float sizedelta			;
};
class CParticleSystem  
{


  private :
    int m_wait;
    int m_nActive;
    int m_Frames			;	// How many frame since reset
    int m_nParticles		;	// Number of particles
    particle* m_pParticles  ;	// Pointer to our particle-array	
    GLuint m_texture		;	
    Vector3f m_location		;	// The particle-systems orientation
    const char* m_strFile	;	// The 
  private :
    void update();
    void grow();
    void InitParticles();
    void GenerateTexture();




  public:
    void Reset();

    void Render();
    CParticleSystem(int nmbr ,Vector3f loc ,const char* bmpfile);
    virtual ~CParticleSystem();

};

#endif 


