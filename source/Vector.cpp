/* Written by Nils Johan Torp */

/* $Id: Vector.cpp,v 1.1 2003/02/22 19:48:57 totto Exp $ */

#include "Vector.h"
// This computes the magnitude of a normal. (magnitude = sqrt(x^2 + y^2 + z^2)
#define Mag(Normal) (sqrt(Normal.x*Normal.x + \
      Normal.y*Normal.y + Normal.z*Normal.z))


/*---------------------------------------------------------------------------*/

Vector3f makeVector(float x, float y, float z)
{
  Vector3f result;
  result.x = x;
  result.y = y;
  result.z = z;

  return result;
}
// This calculates a vector between 2 points and returns the result
Vector3f Vector(Vector3f vPoint1, Vector3f vPoint2)
{
  Vector3f vVector; // The variable to hold the resultant vector

  vVector.x = vPoint1.x - vPoint2.x; // Subtract point1 and point2 x's
  vVector.y = vPoint1.y - vPoint2.y; // Subtract point1 and point2 y's
  vVector.z = vPoint1.z - vPoint2.z; // Subtract point1 and point2 z's

  return vVector;// Return the resultant vector
}


/*---------------------------------------------------------------------------*/

// This adds 2 vectors together and returns the result
Vector3f AddVector(Vector3f vVector1, Vector3f vVector2)
{
  Vector3f vResult; // The variable to hold the resultant vector

  vResult.x = vVector2.x + vVector1.x;// Add Vector1 and Vector2 x's
  vResult.y = vVector2.y + vVector1.y;// Add Vector1 and Vector2 y's
  vResult.z = vVector2.z + vVector1.z;// Add Vector1 and Vector2 z's

  return vResult;// Return the resultant vector
}


/*---------------------------------------------------------------------------*/

// This divides a vector by a single number (scalar) and returns the result
Vector3f DivideVectorByScaler(Vector3f vVector1, float Scaler)
{
  Vector3f vResult; // The variable to hold the resultant vector

  vResult.x = vVector1.x / Scaler;  // Divide Vector1's x value by the scaler
  vResult.y = vVector1.y / Scaler;  // Divide Vector1's y value by the scaler
  vResult.z = vVector1.z / Scaler;  // Divide Vector1's z value by the scaler

  return vResult;// Return the resultant vector
}


/*---------------------------------------------------------------------------*/

// This divides a vector by a single number (scalar) and returns the result
Vector3f MultiplyVectorByScalar(Vector3f vVector1, const float Scalar)
{
  Vector3f vResult; // The variable to hold the resultant vector

  vResult.x = vVector1.x * Scalar;  // Divide Vector1's x value by the scaler
  vResult.y = vVector1.y * Scalar;  // Divide Vector1's y value by the scaler
  vResult.z = vVector1.z * Scalar;  // Divide Vector1's z value by the scaler

  return vResult;// Return the resultant vector
}


/*---------------------------------------------------------------------------*/

// This returns the cross product between 2 vectors
Vector3f Cross(Vector3f vVector1, Vector3f vVector2)
{
  Vector3f vCross;// The vector to hold the cross product
  // Get the X value
  vCross.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
  // Get the Y value
  vCross.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
  // Get the Z value
  vCross.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

  return vCross;// Return the cross product
}


/*---------------------------------------------------------------------------*/

// This returns the normal of a vector
Vector3f Normalize(Vector3f vNormal)
{
  double Magnitude; // This holds the magitude        

  Magnitude = Mag(vNormal); // Get the magnitude

  vNormal.x /= (float)Magnitude;// Divide the vector's X by the magnitude
  vNormal.y /= (float)Magnitude;// Divide the vector's Y by the magnitude
  vNormal.z /= (float)Magnitude;// Divide the vector's Z by the magnitude

  return vNormal;// Return the normal
}
