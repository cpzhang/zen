#pragma once
#include "boundingbox.h"
#include "math.h"
#include "matrix.h"
#include "misc/stdHead.h"
#include "misc/helper.h"
#include "vector2.h"
//
class ApiRender_ OrbitCamera
{
public:
	Matrix	view_;
	float	speed_;
	//Stuff to handle key events correctly
	typedef std::map< int, bool>	KeyDownMap;
	KeyDownMap	keyDown_;
	/** The extents the camera may move to, applied in update() */
	BoundingBox limit_;
	float		angleYZ_;
	float		angleXZ_;
	bool	invert_;
	Vector3 lastPos_;
	float lastYaw_;
	float lastPitch_;
	float distance_;
	Vector3 center_;
	Vector3 u_, v_, w_;
public:
	OrbitCamera();
	~OrbitCamera();
public:
	float getSpeed() const;
	void setSpeed( float s );
	void update( float dTime);
	bool handleKeyEvent( );
	bool handleMouseEvent();
	void create( float distance, float angleXZ, float angleYZ);
	void viewToPolar();

	void polarToView();
	Vector3 getCenter()
	{
		return center_;
	}
	void setCenter(const Vector3& p)
	{
		center_ = p;
	}
	void handleInput( float dTime );
private:
	void polarToViewImp_();
};