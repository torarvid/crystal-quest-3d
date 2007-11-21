/* Written by Nils Johan Torp */
// Vector structure
//
// $Id: Vector.h,v 1.1 2003/02/22 19:48:57 totto Exp $
//
#include "globals.h"
//#include "Model3ds.h"

#ifndef CQ3D_HEADER_VECTOR
#define CQ3D_HEADER_VECTOR
// This is our 3D point structure. This will be used to store the vertices of our model.
struct Vector3f {
  float x, y, z;
};

// This is our 2D point structure. This will be used to store the UV coordinates.
struct Vector2f{
  float x, y;
};

// Face structure

Vector3f makeVector(float x, float y, float z);

// This calculates a vector between 2 points and returns the result
Vector3f Vector(Vector3f vPoint1, Vector3f vPoint2);


// This adds 2 vectors together and returns the result
Vector3f AddVector(Vector3f vVector1, Vector3f vVector2);
Vector3f MultiplyVectorByScalar(Vector3f vVector1, const float Scalar);

// This divides a vector by a single number (scalar) and returns the result
Vector3f DivideVectorByScaler(Vector3f vVector1, float Scaler);

// This returns the cross product between 2 vectors
Vector3f Cross(Vector3f vVector1, Vector3f vVector2);

// This returns the normal of a vector
Vector3f Normalize(Vector3f vNormal);
#endif
