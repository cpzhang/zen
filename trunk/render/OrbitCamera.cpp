#include "OrbitCamera.h"

OrbitCamera::OrbitCamera()
{
	/*keyDown_.insert( std::make_pair( 'W', false ) );
	keyDown_.insert( std::make_pair( 'S', false ) );
	keyDown_.insert( std::make_pair( 'A', false ) );
	keyDown_.insert( std::make_pair( 'D', false ) );
	keyDown_.insert( std::make_pair( 'Q', false ) );
	keyDown_.insert( std::make_pair( 'E', false ) );*/
	angleYZ_ = 0.f;
	angleXZ_ =  0.f;
	invert_ = false;
	lastPos_ = Vector3::Zero;
	lastYaw_ = 0.0f;;
	lastPitch_ = 0.0f;
	center_ = Vector3::Zero;
	capture_ = false;
	dirty_ = true;
}

OrbitCamera::~OrbitCamera()
{

}

float OrbitCamera::getSpeed() const
{
	return speed_;
}

void OrbitCamera::setSpeed( float s )
{
	speed_ = s;
}

void OrbitCamera::update( float dTime )
{
// 	if (!capture_)
// 	{
// 		return;
// 	}
	if (0  && handleKeyEvent())
	{
		handleInput(dTime);
	}
	if (isDirty_())
	{
		polarToViewImp_();	
		setDirty(false);
	}
}

bool OrbitCamera::handleKeyEvent()
{
	//update the key down map.
	bool handled = false;

	/*KeyDownMap::iterator found = keyDown_.begin();

	while ( found != keyDown_.end() )
	{
		if ( isKeyDown(found->first) )
		{
			found->second = true;
			handled = true;
		}
		else
		{
			if ( found->second )
			{
				found->second = false;
				handled = true;
			}
		}
		++found;
	}*/
	if (handled)
	{
		setDirty(true);
	}
	return handled;
}

bool OrbitCamera::onMouseMove()
{
	bool handled = false;

	POINT pt;
	GetCursorPos(&pt);
	static Vector2 lastPos(pt.x, pt.y);
	if (isKeyDown( VK_RBUTTON) )
	{
		float dy = pt.y - lastPos.y;
		if ( dy != 0 )
		{
			if ( invert_ )
				angleYZ_ += ( dy * -0.01f );
			else
				angleYZ_ += ( dy * 0.01f );
			handled = true;
		}
		float dx = pt.x - lastPos.x;
		if ( dx != 0 )
		{
			angleXZ_ += ( dx * 0.01f );
			handled = true;
		}
	}
	if (isKeyDown(VK_MBUTTON))
	{
	}
	lastPos.x = pt.x;
	lastPos.y = pt.y;
	if (handled)
	{
		setDirty(true);
	}
	return handled;
}

void OrbitCamera::create( float distance, float angleXZ, float angleYZ )
{
	distance_ = distance;
	angleXZ_ = angleXZ;
	angleYZ_ = angleYZ;
	polarToViewImp_();
}
void OrbitCamera::polarToViewImp_()
{
	while(angleYZ_ < 0.0f)
	{
		angleYZ_ += MATH_PI_Two;
	}
	while(angleYZ_ > MATH_PI_Two)
	{
		angleYZ_ -= MATH_PI_Two;
	}
	float y = distance_ * cos(angleYZ_);
	float x = distance_ * sin(angleYZ_) * sin(angleXZ_);
	float z = distance_ * sin(angleYZ_) * cos(angleXZ_);
	Vector3 p3(x, y, z);
	Vector3 w3=-p3.unitVector();
	p3 += center_;
	Vector4 p4(p3.x, p3.y, p3.z, 1.0f);
	Vector3 u3;
	if (angleYZ_ > MATH_PI)
	{
		u3.crossProduct(-Vector3::AxisY, w3);
	}
	else
	{
		u3.crossProduct(Vector3::AxisY, w3);
	}
	u3.normalise();
	Vector3 v3;
	v3.crossProduct(w3, u3);
	v3.normalise();
	Matrix vw;
	vw.setRow(0, u3);
	vw.setRow(1, v3);
	vw.setRow(2, w3);
	vw.setRow(3, p4);
	u_ = u3;
	v_ = v3;
	w_ = w3;
	view_ = vw.getInverse();
}
void OrbitCamera::polarToView()
{
	if (!almostEqual( angleXZ_, lastYaw_ ) || !almostEqual( angleYZ_, lastPitch_ ))
	{
		polarToViewImp_();
	}
}

void OrbitCamera::handleInput( float dTime )
{
	//float movementSpeed = speed_;
	////frame rate independent speed, but capped
	//if ( dTime < 0.1f )
	//	movementSpeed *= dTime;
	//else
	//	movementSpeed *= 0.1f;

	////根据距离调整移动速度，距离越远，速度越快；反之，越慢
	//{
	//	float r = distance_ / 1000.0f;
	//	float a = r * MATH_PI_Half;
	//	movementSpeed *= sin(a);
	//}

	//if ( isKeyDown('W'))
	//{
	//	if ( keyDown_['W'] )
	//	{
	//		//move forward
	//		distance_ -= movementSpeed;
	//	}
	//}
	//else
	//{
	//	keyDown_['W'] = false;
	//}

	//if (isKeyDown('S') )
	//{
	//	if ( keyDown_['S'] )
	//	{
	//		//move back
	//		distance_ += movementSpeed;
	//	}
	//}
	//else
	//{
	//	keyDown_['S'] = false;
	//}

	//if (isKeyDown('A') )
	//{
	//	if ( keyDown_['A'] )
	//	{
	//		//center_ -= movementSpeed*u_;
	//		angleXZ_ -= movementSpeed;
	//	}
	//}
	//else
	//{
	//	keyDown_['A'] = false;
	//}

	//if ( isKeyDown('D') )
	//{
	//	if ( keyDown_['D'] )
	//	{
	//		//center_ += movementSpeed*u_;
	//		angleXZ_ += movementSpeed;
	//	}
	//}
	//else
	//{
	//	keyDown_['D'] = false;
	//}

	//if (isKeyDown('E') )
	//{
	//	if ( keyDown_['E'] )
	//	{
	//		//center_ -= movementSpeed*v_;
	//		angleYZ_ -= movementSpeed;
	//	}
	//}
	//else
	//{
	//	keyDown_['E'] = false;
	//}

	//if (isKeyDown('Q') )
	//{
	//	if ( keyDown_['Q'] )
	//	{
	//		//center_ += movementSpeed*v_;
	//		angleYZ_ += movementSpeed;
	//	}
	//}
	//else
	//{
	//	keyDown_['Q'] = false;
	//}
}

void OrbitCamera::setCapture( bool b )
{
	capture_ = b;
}

void OrbitCamera::setDirty( bool b )
{
	dirty_ = b;
}

bool OrbitCamera::isDirty_()
{
	return dirty_;
}

void OrbitCamera::onMouseWheel( float d )
{
	//根据距离调整移动速度，距离越远，速度越快；反之，越慢
	float r = distance_ / 1000.0f;
	float a = r * MATH_PI_Half;
	d = d * speed_ * sin(a);
	distance_ += d;
	setDirty(true);
}
