#pragma once
#include "boundingbox.h"
#include "math.h"
#include "matrix.h"
#include "misc/stdHead.h"
#include "misc/helper.h"
#include "vector2.h"
//
class ApiRender_ MouseCamera
{
public:
	Matrix	view_;
	float	speed_[2];
	//Stuff to handle key events correctly
	typedef std::map< int, bool>	KeyDownMap;
	KeyDownMap	keyDown_;
	/** The extents the camera may move to, applied in update() */
	BoundingBox limit_;
	float		pitch_;
	float		yaw_;
	bool	invert_;
	Vector3 lastPos_;
	float lastYaw_;
	float lastPitch_;
public:
	MouseCamera();
	~MouseCamera();
public:
	void create( float distance, float angleXZ, float angleYZ ){}
	float getSpeed() const;
	void setSpeed( float s );
	float turboSpeed() const;
	void turboSpeed( float s );
	void update( float dTime);
	bool handleKeyEvent( );
	bool handleMouseEvent();
	void init( const Matrix & v );
	void init( const Vector3 & eye, const Vector3 & lookAt, const Vector3 & up );
	void viewToPolar();
	void setPosition(const Vector3 & eye);
	void polarToView();

	void handleInput( float dTime );
};