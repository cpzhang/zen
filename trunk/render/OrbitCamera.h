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
	float	speed_[2];
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
	float speed() const;

	void speed( float s );

	float turboSpeed() const;

	void turboSpeed( float s );
	void update( float dTime);
	bool handleKeyEvent( );
	bool handleMouseEvent();
	void create( float distance, float angleXZ, float angleYZ);
	void viewToPolar();

	void polarToView();

	void handleInput( float dTime );
private:
	void polarToViewImp_();
};