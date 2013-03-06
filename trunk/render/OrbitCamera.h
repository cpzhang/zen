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
		//平滑插值
		//if (1)
		{
			if (moved)
			{
				Vector3 v(sin(angleY_), 0.0f, cos(angleY_));
				vIdealPos = vPosition + v * camera_.getSpeed() * dTime;
				Rt_ = eWalkType_WSAD;
			}
			Vector3 vDisplace = vPosition - vIdealPos;
			float fd = vDisplace.lengthSquared();
			if (fd >= 0.001f)
			{
				static float fDampingK = 6.0f;
				{
					Vector3 vSpringAccel = -((fDampingK*fDampingK/4.0f) * vDisplace) + (-fDampingK * vVelocity);
					vVelocity += vSpringAccel * dTime*0.001f;
					vPosition += vVelocity * dTime*0.001f;
					camera_.setCenter(vPosition);
				}
			}
			else
			{
				vPosition = camera_.getCenter();
				vVelocity = Vector3::Zero;
			}
		} 
	}
	bool getDst(float dTime, Vector3& dst)
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
		//平滑插值
		if (moved)
		{
			Vector3 v(sin(angleY_), 0.0f, cos(angleY_));
			dst = vPosition + v * camera_.getSpeed() * dTime;
			//Rt_ = eWalkType_WSAD;
		}
		return moved;
	}
	void update( float dTime, float ch)
	{
		camera_.update(dTime, ch);
		if (isKeyDown('W') || isKeyDown('S') || isKeyDown('A') || isKeyDown('D'))
		{
			//Rt_ = eWalkType_WSAD;
		}
		if (Rt_ == eWalkType_Mouse)
		{
			if (Kfs_.numKeyFrames() > 0)
			{
				if (!CenterController_.End_)
				{
					CenterController_.update(dTime);
					Vector3 oc = camera_.getCenter();
					Vector3 nc = CenterController_.getValue();
					Vector3 dp = nc - oc;
					dp.y = 0.0f;
					dp.normalise();
					float ag = atan(dp.x / dp.z);
					if (dp.z > 0.001f)
					{
						angleY_ = ag;
					}
					else if (dp.z < -0.001f)
					{

						angleY_ = MATH_PI + ag;
					}
					camera_.setCenter(nc);
					vPosition = nc;
					vVelocity = Vector3::Zero;
				}
			}
		}
		
		if(Rt_ == eWalkType_WSAD)
		{
			//updateWSAD(dTime);
		}
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
	void resetPath()
	{
		Kfs_.clear();
	}
	void addPath(const Vector3& p)
	{
		if (Kfs_.numKeyFrames() == 0)
		{
			Kfs_.addKeyFrame(sKeyFrame<Vector3>(0, p));
		}
		else
		{
			Vector3 lp = Kfs_.getKeyFrame(Kfs_.numKeyFrames() - 1)->v;
			float t = 10*(p - lp).length() / camera_.getSpeed() + Kfs_.getKeyFrame(Kfs_.numKeyFrames() - 1)->time;
			Kfs_.addKeyFrame(sKeyFrame<Vector3>(t, p));
		}
	}
	void beginPath()
	{
		CenterController_.init(&Kfs_, camera_.getCenter(), false);
		CenterController_.begin();
		Rt_ = eWalkType_Mouse;
	}
private:
	OrbitCamera camera_;
	float angleY_;
	KeyFrameController<Vector3> CenterController_;
	sKeyFrameSet<Vector3> Kfs_;
	enum eWalkType
	{
		eWalkType_WSAD,
		eWalkType_Mouse,
	};
	eWalkType Rt_;
	Vector3 vIdealPos;
	Vector3 vPosition;
	Vector3 vVelocity;
};