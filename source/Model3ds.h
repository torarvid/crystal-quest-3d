/* Written by Nils Johan Torp */
// Model3ds.h: interface for the CModel3ds class.
//
// $Id: Model3ds.h,v 1.1 2003/02/22 19:48:57 totto Exp $
//
//////////////////////////////////////////////////////////////////////

#ifndef CQ3D_HEADER_MODEL3DS
#define CQ3D_HEADER_MODEL3DS

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PRIMARY 0x4D4D

//>------ Main Chunks
#define OBJECTINFO 0x3D3D// This gives the version of the mesh and is found right before the material and object information
#define EDITKEYFRAME 0xB000// This is the header for all of the key frame info

//>------ sub defines of OBJECTINFO
#define MATERIAL 0xAFFF// This stored the texture info
#define OBJECT 0x4000// This stores the faces, vertices, etc...

//>------ sub defines of MATERIAL


#define MATMAP 0xA200// This is a header for a new material
#define MATMAPFILE 0xA300// This holds the file name of the texture
#define OBJECT_MESH 0x4100// This lets us know that we are reading a new object

//>------ sub defines of OBJECT_MESH
#define OBJECT_VERTICES 0x4110 // The objects vertices
#define OBJECT_FACES 0x4120 // The objects faces
#define OBJECT_MATERIAL 0x4130 // This is found if the object has a material, either texture map or color
#define OBJECT_UV 0x4140 // The UV texture coordinates

#include "bitmap.h"
#include "Vector.h"
#include "globals.h"



// This holds the chunk info
struct Chunk
{
  unsigned short int ID; // The chunk's ID
  unsigned int length; // The length of the chunk
  unsigned int bytesRead; // The amount of bytes read within that chunk
};

// This is our face structure. This is is used for indexing into the vertex 
// and texture coordinate arrays. From this information we know which vertices
// from our vertex array go to which face, along with the correct texture coordinates.
struct Face
{
  unsigned int vertIndex[3]; // indicies for the verts that make up this triangle
  int coordIndex[3]; // indicies for the tex coords to texture this face
  Vector3f center;
  float radius;
};

typedef enum
{
  SHIP,
  WORLD,
  STONE_BREAKABLE,
  STONE_UNBREAKABLE,
  CRYSTAL
}ob_type;

class CModel3ds 
{
  public:
    void Render();
    int Load();
    CModel3ds();
    CModel3ds(char* file,unsigned int Texture, ob_type type = WORLD, 
	float speed = 0.0002f);
    virtual ~CModel3ds();
    Vector3f m_Center; 
    Vector3f m_Pos;
    Vector3f m_Origin;
    float m_Scale;
    float m_BoundingRadius;
    Face* m_Faces;
    float* m_FaceRadius;
    Vector3f* m_FaceCenter;
    unsigned int m_nFaces ;
    Vector3f* m_Vertices;
    ob_type type; //Object type - default: WORLD
    void (*path_func)(CModel3ds *model);
    Vector3f *ctrlPoints;
    Vector3f *curvePoints;
    CModel3ds *collRef;
    float stoneTilt, stoneFactor, stoneSpeed;
    bool stoneOnZAxis;
    void printFName();
    bool isActive;

  private:
    char m_strName[255];
    char m_strTextureFile[255];
    PAK_file* m_pFile ;
    unsigned int m_nVertices ;
    unsigned int m_nTextureCoord ;
    unsigned int m_Texture ;
    int m_bHasTexture ;
    Vector3f* m_Normals ;
    Vector2f* m_TextureCoord ;
    Chunk *m_CurrentChunk;
    Chunk *m_TempChunk;
    int displayList;
    char* m_strFileName;

  private:
    void MainChunks(Chunk* current);
    void Object(Chunk* Previous);
    void Material(Chunk* Previous);
    void ObjectInfo(Chunk* Previous);
    void ObjectMesh(Chunk* Previous);
    void ReadTextureVertices(Chunk* Previous);
    void ReadVertexIndices(Chunk* Previous);
    void ReadVertices(Chunk* Previous);
    int GetString(char* Str);
    void CalculateNormals();
    void ReadChunk(Chunk *pChunk);
    void GenerateTexture();
    void CalcBoundingSphere();
    void CalcFaceRadii();
    void calcPath(
	float tilt = 0.0f,
	float factor = 10.0f,
	float speed = 0.02,
	bool onZAxis = false
	);
};

extern void path1(CModel3ds *model);

#endif 
