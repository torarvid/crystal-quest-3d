// Model3ds.h: interface for the CModel3ds class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODEL3DS_H__5ED662F5_9E10_4A1E_BFAA_519451FC640F__INCLUDED_)
#define AFX_MODEL3DS_H__5ED662F5_9E10_4A1E_BFAA_519451FC640F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Vector.h"
#include <stdio.h>

enum {
	VERTICES = 0x4110,
	FACES = 0x4120 ,
	MATMAPFILE = 0xA300 ,
	OBJECT = 4100 
};


class CModel3ds  
{
private:
	unsigned char m_bDrawmode ;
	Face* m_Faces ;
	Vector3f* m_Vertices ; 
	Vector3f* m_Normals ;
	unsigned short m_nVertices ;
	unsigned short m_nFaces ;
	char* m_strFilename ;
	FILE* m_pFile;
	void CalculateNormals();
	void ReadVertices();
	void ReadFaces();
public:
	void setDrawMode(unsigned char);
	void Load();
	void Draw();
	CModel3ds(char* );
	virtual ~CModel3ds();
};

#endif // !defined(AFX_MODEL3DS_H__5ED662F5_9E10_4A1E_BFAA_519451FC640F__INCLUDED_)
