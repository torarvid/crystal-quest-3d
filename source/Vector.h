// Vector structure
struct Vector3f 
{
	float x, y, z;
};
// Face structure
struct Face{
	short a,b,c,UnKnown ;	
};


// This calculates a vector between 2 points and returns the result
Vector3f Vector(Vector3f vPoint1, Vector3f vPoint2);


// This adds 2 vectors together and returns the result
Vector3f AddVector(Vector3f vVector1, Vector3f vVector2);

// This divides a vector by a single number (scalar) and returns the result
Vector3f DivideVectorByScaler(Vector3f vVector1, float Scaler);

// This returns the cross product between 2 vectors
Vector3f Cross(Vector3f vVector1, Vector3f vVector2);

// This returns the normal of a vector
Vector3f Normalize(Vector3f vNormal);
