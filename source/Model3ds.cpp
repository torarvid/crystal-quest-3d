// Model3ds.cpp: implementation of the CModel3ds class.
//
//////////////////////////////////////////////////////////////////////

#ifdef WIN32
#include "StdAfx.h"
#endif

#include "Model3ds.h"
#include <memory.h>
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#endif

#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library

CModel3ds::CModel3ds(char* filename)
{
	m_strFilename = filename ;
	m_bDrawmode  = GL_TRIANGLES ;
}

CModel3ds::~CModel3ds()
{
	delete[] m_Vertices ;
	delete[] m_Faces ;
	delete[] m_Normals ;
}




void CModel3ds::Draw()
{
  static int testvar = 0;
  int col = 40; 
  glBegin(m_bDrawmode);
  for(int i= 0 ; i< m_nFaces   ; i++){
    glColor3ub(col, col, col);
    testvar = ++testvar % 2;
	   glNormal3f( m_Normals[m_Faces[i].a].x, m_Normals[m_Faces[i].a].y,  m_Normals[m_Faces[i].a].z);
	   glVertex3f( m_Vertices[m_Faces[i].a].x, m_Vertices[m_Faces[i].a].y,  m_Vertices[m_Faces[i].a].z);
	   glNormal3f( m_Normals[m_Faces[i].b].x, m_Normals[m_Faces[i].b].y,  m_Normals[m_Faces[i].b].z);
	   glVertex3f( m_Vertices[m_Faces[i].b].x, m_Vertices[m_Faces[i].b].y,  m_Vertices[m_Faces[i].b].z);
	   glNormal3f( m_Normals[m_Faces[i].c].x, m_Normals[m_Faces[i].c].y,  m_Normals[m_Faces[i].c].z);	   
	   glVertex3f( m_Vertices[m_Faces[i].c].x, m_Vertices[m_Faces[i].c].y,  m_Vertices[m_Faces[i].c].z);		
	   col = testvar ? col : ((col + 37) % 256);
  }	
  	
  glEnd();					
}

void CModel3ds::Load()
{   static int test = 0 ;
	int len ;
	// Open the 3DS file
	m_pFile = fopen(m_strFilename, "rb");
	// Make sure we have a valid file pointer 
	if(!m_pFile) 
		printf("Unable to find the file.\n");
	// check if the file is a .3ds file
	unsigned short id ;
	fread(&id,1,2,m_pFile );
	if(id!= 0x4d4d ){
		printf("not a 3ds-file");
		return ;
	}
	fread(&len,1,4,m_pFile );
	// parse the file until we reach something we need(VERTICES,FACES)
	while( !feof( m_pFile )){
		if(test <100)
			printf("%x\n",id);
		test++;
		fread(&id,1,2,m_pFile );
		switch(id){
		case 0x3D3D :
			printf("model found");
			break ;
		case VERTICES:
			
			ReadVertices();
			break;
		case FACES :
		
			ReadFaces(); 
			break;
		case MATMAPFILE :
			break;
		
		}
	
	}

	CalculateNormals(); 	
}

void CModel3ds::ReadVertices()
{
	int i,l ;
	// get the number of bytes to read.The last eight bytes is the id of the next 
	fread(&l,1,4,m_pFile );
	// get number of Verts
	//m_nVertices  = ReadShort();
	fread(&m_nVertices,1,2,m_pFile );
	// allocate memory 
	m_Vertices = new Vector3f[m_nVertices ];
	memset(m_Vertices, 0, sizeof(Vector3f) * m_nVertices);
	// read the vertices into our array of vertices
	fread(m_Vertices, 1, sizeof(Vector3f) * m_nVertices, m_pFile);
	// Now we should have all of the vertices read in.  Because 3D Studio Max
	// Models with the Z-Axis pointing up (strange and ugly I know!), we need
	// to flip the y values with the z values in our vertices.  That way it
	// will be normal, with Y pointing up.  If you prefer to work with Z pointing
	// up, then just delete this next loop.  Also, because we swap the Y and Z
	// we need to negate the Z to make it come out correctly.

	// Go through all of the vertices that we just read and swap the Y and Z values
	for(i = 0; i < m_nVertices; i++)
	{
		// Store off the Y value
		float fTempY = m_Vertices[i].y;

		// Set the Y value to the Z value
		m_Vertices[i].y = m_Vertices[i].z;

		// Set the Z value to the Y value, 
		// but negative Z because 3D Studio max does the opposite.
		m_Vertices[i].z = -fTempY;
	}	
	// print the result
	printf("Number of verts : %i\n",m_nVertices);
	for(i=0 ;i < m_nVertices ; i++);
	{
	  printf("Vertex %i :  x= %f  ,y= %f , z= %f\n" ,i,m_Vertices[i].x ,m_Vertices[i].y ,m_Vertices[i].z); 
	}
	
}

void CModel3ds::ReadFaces()
{
	// get the number of bytes to read.The last eight bytes is the id of the next
	int l ;
	fread(&l,1,4,m_pFile );
	// get number of Faces
		fread(&m_nFaces,1,2,m_pFile );
	// allocate memory 
	m_Faces = new Face[m_nFaces];
	memset(m_Faces ,0,sizeof(Face)*m_nFaces);
	// read the Faces 
	fread(m_Faces ,1,sizeof(Face)*m_nFaces,m_pFile);
	// print the result
	printf("Number of faces : %i\n" ,m_nFaces);
	for(int i=0 ; i<m_nFaces ; i++); 
	//	printf("Face %i a= %i ,b= %i ,c= %i \n",i,m_Faces[i].a ,m_Faces[i].b ,m_Faces[i].c );
}

void CModel3ds::CalculateNormals()
{
	Vector3f vVector1, vVector2, vNormal, vPoly[3];



	// What are vertex normals?  And how are they different from other normals?
	// Well, if you find the normal to a triangle, you are finding a "Face Normal".
	// If you give OpenGL a face normal for lighting, it will make your object look
	// really flat and not very round.  If we find the normal for each vertex, it makes
	// the smooth lighting look.  This also covers up blocky looking objects and they appear
	// to have more polygons than they do.    Basically, what you do is first
	// calculate the face normals, then you take the average of all the normals around each
	// vertex.  It's just averaging.  That way you get a better approximation for that vertex.

	
	

		// Here we allocate all the memory we need to calculate the normals
		Vector3f *pNormals		= new Vector3f[m_nFaces];
		Vector3f *pTempNormals	= new Vector3f[m_nFaces];
		m_Normals	= new Vector3f[m_nVertices];

		// Go though all of the faces of this object
		for(int i=0; i < m_nFaces; i++)
		{												
			// To cut down LARGE code, we extract the 3 points of this face
			vPoly[0] = m_Vertices[m_Faces[i].a];
			vPoly[1] = m_Vertices[m_Faces[i].b];
			vPoly[2] = m_Vertices[m_Faces[i].c];

			// Now let's calculate the face normals (Get 2 vectors and find the cross product of those 2)

			vVector1 = Vector(vPoly[0], vPoly[2]);		// Get the vector of the polygon (we just need 2 sides for the normal)
			vVector2 = Vector(vPoly[2], vPoly[1]);		// Get a second vector of the polygon

			vNormal  = Cross(vVector1, vVector2);		// Return the cross product of the 2 vectors (normalize vector, but not a unit vector)
			pTempNormals[i] = vNormal;					// Save the un-normalized normal for the vertex normals
			vNormal  = Normalize(vNormal);				// Normalize the cross product to give us the polygons normal

			pNormals[i] = vNormal;						// Assign the normal to the list of normals
		}

		//////////////// Now Get The Vertex Normals /////////////////

		Vector3f vSum = {0.0, 0.0, 0.0};
		Vector3f vZero = vSum;
		int shared=0;
		int i;
		for (i = 0; i < m_nVertices; i++)			// Go through all of the vertices
		{
			for (int j = 0; j < m_nFaces; j++)	// Go through all of the triangles
			{												// Check if the vertex is shared by another face
				if (m_Faces[j].a  == i || 
					m_Faces[j].b == i || 
					m_Faces[j].c == i)
				{
					vSum = AddVector(vSum, pTempNormals[j]);// Add the un-normalized normal of the shared face
					shared++;								// Increase the number of shared triangles
				}
			}      
			
			// Get the normal by dividing the sum by the shared.  We negate the shared so it has the normals pointing out.
			m_Normals[i] = DivideVectorByScaler(vSum, float(-shared));

			// Normalize the normal for the final vertex normal
			m_Normals[i] = Normalize(m_Normals[i]);	

			vSum = vZero;									// Reset the sum
			shared = 0;										// Reset the shared
		}
		for(i=0 ; i<m_nVertices ; i++){
			//printf("Normal %i  :x= %f ,y= %f ,z= %f \n" ,i,m_Normals[i].x ,m_Normals[i].y ,m_Normals[i].z   );
		}
		// Free our memory and start over on the next object
		delete [] pTempNormals;
		delete [] pNormals;
	

}

void CModel3ds::setDrawMode(unsigned char mode)
{
	m_bDrawmode = mode ; 
}
