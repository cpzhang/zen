#pragma once
#include "boundingbox.h"
#include "math.h"
#include "matrix.h"
#include "misc/stdHead.h"
#include "misc/helper.h"
#include "vector2.h"
#include "KeyFrames.h"
//
class ApiRender_ OrbitCamera
{
public:
	Matrix	view_;
	float	speed_;
	//Stuff to handle key events correctly
	typedef std::map< int, bool>	KeyDownMap;
	//KeyDownMap	keyDown_;
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
	float TerrainHeight_;
private:
	bool capture_;
	bool dirty_;
public:
	OrbitCamera();
	~OrbitCamera();
public:
	void onMouseWheel(float d);
	void setCapture(bool b);
	float getSpeed() const;
	void setSpeed( float s );
	void update( float dTime, float ch);
	bool handleKeyEvent( );
	bool onMouseMove();
	void create( float distance, float angleXZ, float angleYZ);
	void viewToPolar();

	void polarToView();
	Vector3 getCenter()
	{
		return center_;
	}
	void setCenter(const Vector3& p);
	void handleInput( float dTime );
	void setTerrainHeight(float f)
	{
		if (!almostEqual(f, TerrainHeight_))
		{
			TerrainHeight_ = f;
			setDirty(true);
		}
	}
private:
	void polarToViewImp_(float ch);
	void setDirty(bool b);
	bool isDirty_();
		
	float getAngleYZ(float ch);
public:
	static std::string tEvent_Dirty;
};

class ApiRender_ HeroCamera
{
public:
	HeroCamera();
public:
	void updateWSAD(float dTime)
	{
		bool moved = true;
		if (isKeyDown('W'))
		{
			angleY_ = camera_.angleXZ_ + MATH_PI;
		} 
		else if(isKeyDown('S'))
		{
			angleY_ = camera_.angleXZ_; 
		}
		else if(isKeyDown('A'))
		{
			angleY_ = camera_.angleXZ_ + MATH_PI_Half;
		}
		else if(isKeyDown('D'))
		{
			angleY_ = camera_.angleXZ_ - MATH_PI_Half;
		}
		else
		{
			moved = false;
		}
		if (moved)
		{
			Vector3 v(sin(angleY_), 0.0f, cos(angleY_));
			v = v * camera_.getSpeed() * dTime;
			Kfs_._keyFrames[0].v = CenterController_.getValue();
			Kfs_._keyFrames[1].v += v; 
			CenterController_.begin();
		}
		if (moved || !CenterController_.End_)
		{
			CenterController_.update(dTime);
			camera_.setCenter(CenterController_.getValue());
		}
	}
	void update( float dTime, float ch)
	{
		camera_.update(dTime, ch);
		updateWSAD(dTime);
	}
	Matrix getViewMatrix()
	{
		return camera_.view_;
	}
	bool onMouseMove()
	{
		return camera_.onMouseMove();
	}
	Vector3 getCenter()
	{
		return camera_.getCenter();
	}
	void setCenter(const Vector3& p)
	{
		camera_.setCenter(p);
	}
	float getAngleY()
	{
		return angleY_;
	}
	void onMouseWheel(float d)
	{
		camera_.onMouseWheel(d);
	}
	float getSpeed() const
	{
		return camera_.getSpeed();
	}
	void setSpeed( float s )
	{
		camera_.setSpeed(s);
	}
	void setTerrainHeight(float f)
	{
		camera_.setTerrainHeight(f);
	}
private:
	OrbitCamera camera_;
	float angleY_;
	KeyFrameController<Vector3> CenterController_;
	sKeyFrameSet<Vector3> Kfs_;
};