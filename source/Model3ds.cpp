/* Written by Nils Johan Torp */
// Model3ds.cpp: implementation of the CModel3ds class.
//
// $Id: Model3ds.cpp,v 1.1 2003/02/22 19:48:57 totto Exp $
//
//////////////////////////////////////////////////////////////////////

//#include "globals.h"
#include "Model3ds.h"
//#include "bitmap.h"
//#include "Vector.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


/*---------------------------------------------------------------------------*/

CModel3ds::CModel3ds()
{
}


/*---------------------------------------------------------------------------*/

CModel3ds::CModel3ds(const char* file,unsigned int Texture, 
    ob_type otype, float speed)
{
  m_strFileName = file ;
  m_Texture = Texture ;
  m_bHasTexture = 0;
  m_Scale = 1.0f;
  m_Pos.x = 0.0f;
  m_Pos.y = 0.0f;
  m_Pos.z = 0.0f;
  type = otype;
  curvePoints = ctrlPoints = NULL;
  stoneTilt = 0.0f;
  stoneFactor = 10.0f;
  stoneSpeed = speed;
  stoneOnZAxis = false;
  collRef = NULL;
  isActive = true;
}


/*---------------------------------------------------------------------------*/

CModel3ds::~CModel3ds()
{
  // free used memory
  delete[] m_Normals ;
  delete[] m_Vertices;
  delete[] m_Faces;
  delete[] m_TextureCoord ;

}


/*---------------------------------------------------------------------------*/

void CModel3ds::ReadChunk(Chunk *pChunk)
{
  // This reads the chunk ID which is 2 bytes.
  // The chunk ID is like OBJECT or MATERIAL.  It tells what data is
  // able to be read in within the chunks section.  
  pChunk->bytesRead = pak_fread(&pChunk->ID, 1, 2, m_pFile);
  // Then, we read the length of the chunk which is 4 bytes.
  // This is how we know how much to read in, or read past.
  pChunk->bytesRead += pak_fread(&pChunk->length, 1, 4, m_pFile);
}


/*---------------------------------------------------------------------------*/

int CModel3ds::Load()
{
  fprintf(logfile, "Loading %s... ", m_strFileName);
  m_CurrentChunk = new Chunk ;
  m_TempChunk    = new Chunk;
  // Open the 3DS file
  m_pFile = pak_fopen(m_strFileName, theFiles);

  // Make sure we have a valid file pointer (we found the file)
  if(!m_pFile) 
  {
    fprintf(stderr, "Unable to find the file: %s!", m_strFileName );
    return 0 ;
  }

  // Read the first chuck of the file to see if it's a 3DS file
  ReadChunk(m_CurrentChunk);

  // Make sure this is a 3DS file
  if (m_CurrentChunk->ID != PRIMARY)
  {
    fprintf(stderr, "Unable to load PRIMARY chunk from file: %s!",
	m_strFileName);
    return 0;
  }
  MainChunks(m_CurrentChunk);
  // if the object has a texture-map then generate the map
  if(m_bHasTexture)
    GenerateTexture(); 

  //CalculateNormals(); 

  CalcBoundingSphere();
  CalcFaceRadii();

  pak_fclose(m_pFile );	// Close the current file pointer
  if (m_CurrentChunk != NULL)
    delete m_CurrentChunk;	// Free the current chunk
  delete m_TempChunk;// Free our temporary chunk

  displayList = glGenLists(1);
  glNewList(displayList, GL_COMPILE);

  glBindTexture(GL_TEXTURE_2D, m_Texture );
  glBegin(GL_TRIANGLES);
  for(unsigned int i=0 ;i<m_nFaces ;i++){
    for(int whichVertex = 0; whichVertex < 3; whichVertex++)
    {
      // Get the index for each point of the face
      int index = m_Faces[i].vertIndex[whichVertex];
      // Give OpenGL the normal for this vertex.
      //glNormal3f(m_Normals[index ].x, m_Normals[ index ].y, 
      //m_Normals[ index ].z);

      // If the object has a texture associated with it, 
      // give it a texture coordinate.
      if(m_bHasTexture ) 
      {
	glTexCoord2f(m_TextureCoord[index].x, m_TextureCoord[ index ].y);
      }
      else 
	// We draw white
	glColor3ub(255, 255, 255);
      // Pass in the current vertex of the object (Corner of current face)
      glVertex3f(m_Vertices[ index ].x, m_Vertices[ index ].y, 
	  m_Vertices[ index ].z);
    }
  }
  glEnd();
  glEndList();

  fprintf(logfile, "OK\n");
  return 1;
}


/*---------------------------------------------------------------------------*/

// the two mainchunks OBJECTINFO and EDITKEYFRAME
void CModel3ds::MainChunks(Chunk *Previous)
{
  // This is used to read past unwanted data
  int *buffer = new int[5000];
  memset(buffer, 0, 5000 * sizeof(int));

  m_CurrentChunk = new Chunk;	// Allocate a new chunk
  while (Previous->bytesRead < Previous->length)
  {
    // Read next Chunk
    ReadChunk(m_CurrentChunk);
    // Check the chunk ID
    switch (m_CurrentChunk->ID)
    {

      case OBJECTINFO:
	ObjectInfo(m_CurrentChunk);
	break;
      case EDITKEYFRAME: 
	m_CurrentChunk->bytesRead += pak_fread(buffer, 1, 
	    m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_pFile);
	break;
      default:
	m_CurrentChunk->bytesRead += pak_fread(buffer, 1, 
	    m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_pFile);
	break;
    }
    // Add the bytes read from the last chunk to the previous chunk passed in.
    Previous->bytesRead += m_CurrentChunk->bytesRead;
  }
  // Free the current chunk and set it back to the 
  // previous chunk (since it started that way)
  delete m_CurrentChunk;
  delete[] buffer;
  m_CurrentChunk = Previous;

}


/*---------------------------------------------------------------------------*/

int CModel3ds::GetString(char *Str)
{
  int index = 0;
  // Read 1 byte of data which is the first letter of the string
  pak_fread(Str, 1, 1, m_pFile);
  // Loop until we get NULL
  while (*(Str + index++) != 0) 
    pak_fread(Str + index, 1, 1, m_pFile);
  // Return the string length, which is how many bytes 
  // we read in (including the NULL)
  return strlen(Str) + 1;

}


/*---------------------------------------------------------------------------*/

void CModel3ds::ObjectInfo(Chunk *Previous)
{
  int *buffer = new int[5000];	// This is used to read past unwanted data
  memset(buffer, 0, 5000 * sizeof(int));
  m_CurrentChunk = new Chunk;	// Allocate a new chunk

  // Below we check our chunk ID each time we read a new chunk.  Then, if
  // we want to extract the information from that chunk, we do so.
  // If we don't want a chunk, we just read past it.  
  // Continue to read the sub chunks until we have reached the length.
  // After we read ANYTHING we add the bytes read to the chunk and then check
  // check against the length.

  while (Previous->bytesRead < Previous->length)
  {	
    // Read next Chunk
    ReadChunk(m_CurrentChunk);
    // Check the chunk ID
    switch (m_CurrentChunk->ID){	
      case MATERIAL:	// This holds the material information
	// The only thing we care about is the textures filename
	Material(m_CurrentChunk);
	break;
      case OBJECT:
	// Read the object-Name
	m_CurrentChunk->bytesRead += GetString(m_strName);
	// Read objectinfo
	Object(m_CurrentChunk);
	break;
      default:
	m_CurrentChunk->bytesRead += pak_fread(buffer, 1, 
	    m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_pFile);
	break;
    }
    Previous->bytesRead += m_CurrentChunk->bytesRead;
  }
  delete m_CurrentChunk;
  delete[] buffer;
  m_CurrentChunk = Previous;

}


/*---------------------------------------------------------------------------*/

void CModel3ds::Object(Chunk *Previous)
{
  int *buffer = new int[5000];	// This is used to read past unwanted data
  memset(buffer, 0, 5000 * sizeof(int));
  m_CurrentChunk = new Chunk;	// Allocate a new chunk

  // Below we check our chunk ID each time we read a new chunk.  Then, if
  // we want to extract the information from that chunk, we do so.
  // If we don't want a chunk, we just read past it.  
  // Continue to read the sub chunks until we have reached the length.
  // After we read ANYTHING we add the bytes read to the chunk and then check
  // check against the length.

  while (Previous->bytesRead < Previous->length)
  {	
    // Read next Chunk
    ReadChunk(m_CurrentChunk);
    // Check the chunk ID
    switch (m_CurrentChunk->ID){
      case OBJECT_MESH:
	ObjectMesh(m_CurrentChunk);
	break;
      default:
	m_CurrentChunk->bytesRead += pak_fread(buffer, 1, 
	    m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_pFile);
	break;
    }
    Previous->bytesRead += m_CurrentChunk->bytesRead;
  }
  delete m_CurrentChunk;
  delete[] buffer;
  m_CurrentChunk = Previous;

}


/*---------------------------------------------------------------------------*/

void CModel3ds::ObjectMesh(Chunk *Previous)
{

  int *buffer = new int[5000];	// This is used to read past unwanted data
  memset(buffer, 0, 5000 * sizeof(int));
  m_CurrentChunk = new Chunk;// Allocate a new chunk

  // Below we check our chunk ID each time we read a new chunk.  Then, if
  // we want to extract the information from that chunk, we do so.
  // If we don't want a chunk, we just read past it.  

  // Continue to read the sub chunks until we have reached the length.
  // After we read ANYTHING we add the bytes read to the chunk and then check
  // check against the length.

  while (Previous->bytesRead < Previous->length)
  {	
    // Read next Chunk
    ReadChunk(m_CurrentChunk);
    // Check the chunk ID
    switch (m_CurrentChunk->ID){
      case OBJECT_VERTICES:
	ReadVertices(m_CurrentChunk); 
	break;
      case OBJECT_FACES:
	ReadVertexIndices(m_CurrentChunk); 
	break;
      case OBJECT_UV:
	ReadTextureVertices(m_CurrentChunk); 
	break;
      default:
	m_CurrentChunk->bytesRead += pak_fread(buffer, 1, 
	    m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_pFile);
	break;
    }
    Previous->bytesRead += m_CurrentChunk->bytesRead;
  }
  delete m_CurrentChunk;
  delete[] buffer;
  m_CurrentChunk = Previous;
}


/*---------------------------------------------------------------------------*/

void CModel3ds::ReadVertices(Chunk* Previous)
{
  // Like most chunks, before we read in the actual vertices, we need
  // to find out how many there are to read in.  Once we have that number
  // we then pak_fread() them into our vertice array.

  // Read in the number of vertices (int)
  m_nVertices = 0;
  Previous->bytesRead += pak_fread(&m_nVertices, 1, 2, m_pFile);
  // Allocate the memory for the verts and initialize the structure
  m_Vertices = new Vector3f[m_nVertices];
  memset(m_Vertices, 0, sizeof(Vector3f) * m_nVertices);
  // Read in the array of vertices (an array of 3 floats)
  Previous->bytesRead += pak_fread(m_Vertices, 1, 
      Previous->length - Previous->bytesRead, m_pFile);

  // Now we should have all of the vertices read in.  Because 3D Studio Max
  // Models with the Z-Axis pointing up (strange and ugly I know!), we need
  // to flip the y values with the z values in our vertices.  That way it
  // will be normal, with Y pointing up.  If you prefer to work with Z pointing
  // up, then just delete this next loop.  Also, because we swap the Y and Z
  // we need to negate the Z to make it come out correctly.

  // Go through all of the vertices that we just read and swap the 
  // Y and Z values

  for(unsigned int i = 0; i < m_nVertices ; i++)
  {
    // Store off the Y value
    float fTempY = m_Vertices[i].y;
    // Set the Y value to the Z value
    m_Vertices[i].y = m_Vertices[i].z;
    // Set the Z value to the Y value, 
    // but negative Z because 3D Studio max does the opposite.
    m_Vertices[i].z = -fTempY;
  }
}


/*---------------------------------------------------------------------------*/

void CModel3ds::ReadVertexIndices(Chunk* Previous)
{
  unsigned short index = 0; // This is used to read in the current face index

  // In order to read in the vertex indices for the object, we need to first
  // read in the number of them, then read them in.  Remember,
  // we only want 3 of the 4 values read in for each face.  The fourth is
  // a visibility flag for 3D Studio Max that doesn't mean anything to us.

  // Read in the number of faces that are in this object (int)
  memset(&m_nFaces, 0, sizeof(m_nFaces));
  Previous->bytesRead += pak_fread(&m_nFaces, 1, 2, m_pFile );

  // Alloc enough memory for the faces and initialize the structure
  m_Faces = new Face[m_nFaces];
  memset(m_Faces, 0, sizeof(Face) * m_nFaces);
  // Go through all of the faces in this object
  for(unsigned int i = 0; i < m_nFaces; i++){
    // Next, we read in the A then B then C index for the face, 
    // but ignore the 4th value.
    // The fourth value is a visibility flag for 3D Studio Max, 
    // we don't care about this.
    for(int j = 0; j < 4; j++){
      // Read the first vertice index for the current face 
      Previous->bytesRead += pak_fread(&index, 1, sizeof(index), m_pFile);
      if(j < 3){
	// Store the index in our face structure.
	m_Faces[i].vertIndex[j] = index;
      }
    }
  }
}


/*---------------------------------------------------------------------------*/

void CModel3ds::Material(Chunk *Previous)
{
  char tempTex[255];
  int *buffer = new int[5000];	// This is used to read past unwanted data
  memset(buffer, 0, 5000 * sizeof(int));
  // Allocate a new chunk to work with
  m_CurrentChunk = new Chunk;
  // Continue to read these chunks until we read the end of this sub chunk
  while (Previous->bytesRead < Previous->length)
  {
    // Read the next chunk
    ReadChunk(m_CurrentChunk);
    // Check which chunk we just read in
    switch (m_CurrentChunk->ID)
    {
      case MATMAP:	// This is the header for the texture info
	// Proceed to read in the material information
	break;
      case MATMAPFILE:// This stores the file name of the material
	// Here we read in the material's file name

	m_CurrentChunk->bytesRead += pak_fread(m_strTextureFile, 1, 
	    m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_pFile);
	sprintf(tempTex, "textures/%s", m_strTextureFile);
	sprintf(m_strTextureFile, "%s", tempTex);
	m_bHasTexture =1 ;
	break;
      default:  
	// Read past the ignored or unknown chunks
	m_CurrentChunk->bytesRead += pak_fread(buffer, 1, 
	    m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_pFile);
	break;
    }
    // Add the bytes read from the last chunk to the previous chunk passed in.
    Previous->bytesRead += m_CurrentChunk->bytesRead;
  }
  // Free the current chunk and set it back to the previous chunk 
  // (since it started that way)
  delete m_CurrentChunk;
  delete[] buffer;
  m_CurrentChunk = Previous;
}


/*---------------------------------------------------------------------------*/

void CModel3ds::ReadTextureVertices(Chunk* Previous)
{

  // Read in the number of Tex coordinates there are (int)
  memset(&m_nTextureCoord, 0, sizeof(m_nTextureCoord));
  Previous->bytesRead += pak_fread(&m_nTextureCoord, 1, 2, m_pFile);
  // Allocate memory to hold the UV coordinates
  m_TextureCoord  = new Vector2f[m_nTextureCoord];
  memset(m_TextureCoord,0,sizeof(Vector2f)*m_nTextureCoord); 
  // Read in the texture coodinates (an array 2 float)
  Previous->bytesRead += pak_fread(m_TextureCoord , 1, 
      Previous->length - Previous->bytesRead, m_pFile);
  //for(unsigned int i=0;i<m_nTextureCoord;i++)
}


/*---------------------------------------------------------------------------*/

void CModel3ds::Render()
{
  if (!isActive)
    return;
  glPushMatrix();
  if ((type == STONE_BREAKABLE) || (type == STONE_UNBREAKABLE) ||
      (type == CRYSTAL))
  {
    glTranslatef(m_Pos.x, m_Pos.y, m_Pos.z);
    calcPath(stoneTilt, stoneFactor, stoneSpeed, stoneOnZAxis);
    if (collRef)
      collRef->calcPath(stoneTilt, stoneFactor, stoneSpeed, stoneOnZAxis);
  }
  glScalef(m_Scale, m_Scale, m_Scale);
  glCallList(displayList);	
  glPopMatrix();
}


/*---------------------------------------------------------------------------*/

void CModel3ds::CalculateNormals()
{
  Vector3f vVector1, vVector2, vNormal, vPoly[3];

  /*
   What are vertex normals?  And how are they different from other
   normals?  Well, if you find the normal to a triangle, you are
   finding a "Face Normal".  If you give OpenGL a face normal for
   lighting, it will make your object look really flat and not very
   round.  If we find the normal for each vertex, it makes the smooth
   lighting look.  This also covers up blocky looking objects and they
   appear to have more polygons than they do.    Basically, what you
   do is first calculate the face normals, then you take the average
   of all the normals around each vertex.  It's just averaging.  That
   way you get a better approximation for that vertex.  Here we
   allocate all the memory we need to calculate the normals
  */

  Vector3f *pNormals= new Vector3f[m_nFaces];
  Vector3f *pTempNormals	= new Vector3f[m_nFaces];
  m_Normals	= new Vector3f[m_nVertices];

  // Go though all of the faces of this object
  for(unsigned int i=0; i < m_nFaces; i++)
  {
    // To cut down LARGE code, we extract the 3 points of this face
    vPoly[0] = m_Vertices[m_Faces[i].vertIndex[0] ];
    vPoly[1] = m_Vertices[m_Faces[i].vertIndex[1]];
    vPoly[2] = m_Vertices[m_Faces[i].vertIndex[2]];
    // Now let's calculate the face normals 
    // (Get 2 vectors and find the cross product of those 2)
    
    // Get the vector of the polygon (we just need 2 sides for the normal)
    vVector1 = Vector(vPoly[0], vPoly[2]);

    // Get a second vector of the polygon
    vVector2 = Vector(vPoly[2], vPoly[1]);

    // Return the cross product of the 2 vectors 
    // (normalize vector, but not a unit vector)
    vNormal  = Cross(vVector1, vVector2);

    // Save the un-normalized normal for the vertex normals
    pTempNormals[i] = vNormal;	

    // Normalize the cross product to give us the polygons normal
    vNormal  = Normalize(vNormal);
    pNormals[i] = vNormal;// Assign the normal to the list of normals
  }
  //////////////// Now Get The Vertex Normals /////////////////
  Vector3f vSum = {0.0, 0.0, 0.0};
  Vector3f vZero = vSum;
  int shared=0;

  // Go through all of the vertices
  for (unsigned int k = 0; k < m_nVertices; k++)
  {
    // Go through all of the triangles
    for (unsigned int j = 0; j < m_nFaces; j++)	
    {// Check if the vertex is shared by another face
      if (m_Faces[j].vertIndex[0] == k || 
	  m_Faces[j].vertIndex[1] == k || 
	  m_Faces[j].vertIndex[2] == k)
      {
	// Add the un-normalized normal of the shared face
	vSum = AddVector(vSum, pTempNormals[j]);
	shared++;// Increase the number of shared triangles
      }
    }      
    // Get the normal by dividing the sum by the shared.  
    // We negate the shared so it has the normals pointing out.
    m_Normals[k] = DivideVectorByScaler(vSum, float(-shared));
    // Normalize the normal for the final vertex normal
    m_Normals[k] = Normalize(m_Normals[k]);	
    vSum = vZero;	// Reset the sum
    shared = 0;// Reset the shared
  }
  // Free our memory and start over on the next object
  delete [] pTempNormals;
  delete [] pNormals;
}


/*---------------------------------------------------------------------------*/

void CModel3ds::GenerateTexture()
{
  // Temporary surface
  SDL_Surface* bitmap = LoadBMP(m_strTextureFile );
  glGenTextures(1,&m_Texture);	
  glBindTexture( GL_TEXTURE_2D,m_Texture );
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


/*---------------------------------------------------------------------------*/

void CModel3ds::CalcBoundingSphere()
{
  unsigned int i;
  float minX = m_Vertices[0].x;
  float minY = m_Vertices[0].y;
  float minZ = m_Vertices[0].z;
  float maxX = m_Vertices[0].x;
  float maxY = m_Vertices[0].y;
  float maxZ = m_Vertices[0].z;
  float maxDist = 0.0f;
  float xDist, yDist, zDist;
  float tempDist;


  for(i=1;i<m_nVertices;i++)
  {
    minX = (m_Vertices[i].x < minX) ? m_Vertices[i].x : minX;
    minY = (m_Vertices[i].y < minY) ? m_Vertices[i].y : minY;
    minZ = (m_Vertices[i].z < minZ) ? m_Vertices[i].z : minZ;
    maxX = (m_Vertices[i].x > maxX) ? m_Vertices[i].x : maxX;
    maxY = (m_Vertices[i].y > maxY) ? m_Vertices[i].y : maxY;
    maxZ = (m_Vertices[i].z > maxZ) ? m_Vertices[i].z : maxZ;
  }

  m_Center.x = (minX + maxX) / 2.0f;
  m_Center.y = (minY + maxY) / 2.0f;
  m_Center.z = (minZ + maxZ) / 2.0f;

  for(i=0;i<m_nVertices;i++)
  {
    xDist = (m_Vertices[i].x - m_Center.x) * m_Scale;
    yDist = (m_Vertices[i].y - m_Center.y) * m_Scale;
    zDist = (m_Vertices[i].z - m_Center.z) * m_Scale;  
    tempDist = (xDist * xDist + yDist * yDist + zDist * zDist);
    maxDist = (tempDist > maxDist) ? tempDist : maxDist;
  }

  m_BoundingRadius = sqrt(maxDist);
}


/*---------------------------------------------------------------------------*/

void CModel3ds::CalcFaceRadii()
{
  unsigned int i, j;
  float minX;
  float minY;
  float minZ;
  float maxX;
  float maxY;
  float maxZ;
  float maxDist = 0.0f;
  float xDist, yDist, zDist;
  float tempDist;

  for(i=0;i<m_nFaces;i++)
  {
    minX = m_Vertices[m_Faces[i].vertIndex[0]].x;
    minY = m_Vertices[m_Faces[i].vertIndex[0]].y;
    minZ = m_Vertices[m_Faces[i].vertIndex[0]].z;
    maxX = m_Vertices[m_Faces[i].vertIndex[0]].x;
    maxY = m_Vertices[m_Faces[i].vertIndex[0]].y;
    maxZ = m_Vertices[m_Faces[i].vertIndex[0]].z;
    for(j=1;j<3;j++)
    {
      minX = (m_Vertices[m_Faces[i].vertIndex[j]].x < minX) ?
	m_Vertices[m_Faces[i].vertIndex[j]].x : minX;
      minY = (m_Vertices[m_Faces[i].vertIndex[j]].y < minY) ?
	m_Vertices[m_Faces[i].vertIndex[j]].y : minY;
      minZ = (m_Vertices[m_Faces[i].vertIndex[j]].z < minZ) ?
	m_Vertices[m_Faces[i].vertIndex[j]].z : minZ;
      maxX = (m_Vertices[m_Faces[i].vertIndex[j]].x > maxX) ?
	m_Vertices[m_Faces[i].vertIndex[j]].x : maxX;
      maxY = (m_Vertices[m_Faces[i].vertIndex[j]].y > maxY) ?
	m_Vertices[m_Faces[i].vertIndex[j]].y : maxY;
      maxZ = (m_Vertices[m_Faces[i].vertIndex[j]].z > maxZ) ?
	m_Vertices[m_Faces[i].vertIndex[j]].z : maxZ;
    }

    m_Faces[i].center.x = (minX + maxX) / 2.0f;
    m_Faces[i].center.y = (minY + maxY) / 2.0f;
    m_Faces[i].center.z = (minZ + maxZ) / 2.0f;

    for(j=0;j<3;j++)
    {
      xDist = (m_Vertices[m_Faces[i].vertIndex[j]].x - m_Faces[i].center.x)
	* m_Scale;
      yDist = (m_Vertices[m_Faces[i].vertIndex[j]].y - m_Faces[i].center.y)
	* m_Scale;
      zDist = (m_Vertices[m_Faces[i].vertIndex[j]].z - m_Faces[i].center.z)
	* m_Scale;
      tempDist = (xDist * xDist + yDist * yDist + zDist * zDist);
      maxDist = (tempDist > maxDist) ? tempDist : maxDist;
    }

    m_Faces[i].radius = sqrt(maxDist);
    /*      m_Faces[i].center.y,
	    m_Faces[i].center.z, m_Faces[i].radius);*/
  }
}


/*---------------------------------------------------------------------------*/

void CModel3ds::calcPath(float tilt, float factor, float speed, bool onZAxis)
{
  static float moveAngle = 0.0f;
  static int rotAngle = 0;
  float a, b, c;

  a = factor * cos(moveAngle);
  b = factor * sin(moveAngle) * sin(tilt);
  c = factor * sin(moveAngle) * cos(tilt);
  if (onZAxis)
  {
    float temp = c;
    c = a;
    a = temp;
  }
  m_Pos.x = m_Origin.x + a;
  m_Pos.y = m_Origin.y + b;
  m_Pos.z = m_Origin.z + c;
  moveAngle = ((moveAngle+speed)>360.0f) ? 0.0f : moveAngle + speed;
  glRotatef(rotAngle / 100.0f, 0.7f, 0.2f, 1.0f);
  ++rotAngle;
  rotAngle %= 36000;
}


/*---------------------------------------------------------------------------*/

void CModel3ds::printFName()
{
  fprintf(stdout, "Filename is: %s\n", m_strFileName);
  fflush(stdout);
}
