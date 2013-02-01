#include "MouseCamera.h"

MouseCamera::MouseCamera()
{
	keyDown_.insert( std::make_pair( 'W', false ) );
	keyDown_.insert( std::make_pair( 'S', false ) );
	keyDown_.insert( std::make_pair( 'A', false ) );
	keyDown_.insert( std::make_pair( 'D', false ) );
	keyDown_.insert( std::make_pair( 'Q', false ) );
	keyDown_.insert( std::make_pair( 'E', false ) );
	pitch_ = 0.f;
	yaw_ =  0.f;
	invert_ = false;
	lastPos_ = Vector3::Zero;
	lastYaw_ = 0.0f;;
	lastPitch_ = 0.0f;
}

MouseCamera::~MouseCamera()
{

}

float MouseCamera::getSpeed() const
{
	return speed_[0];
}

void MouseCamera::setSpeed( float s )
{
	speed_[0] = s;
}

float MouseCamera::turboSpeed() const
{
	return speed_[1];
}

void MouseCamera::turboSpeed( float s )
{
	speed_[1] = s;
}

void MouseCamera::update( float dTime )
{
	if (handleMouseEvent())
	{
		polarToView();
	}
	if (handleKeyEvent())
	{
		handleInput(dTime);
	}
}

bool MouseCamera::handleKeyEvent()
{
	//update the key down map.
	bool handled = false;

	KeyDownMap::iterator found = keyDown_.begin();

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
	}

	return handled;
}

bool MouseCamera::handleMouseEvent()
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
				pitch_ += ( dy * -0.01f );
			else
				pitch_ += ( dy * 0.01f );
			handled = true;
		}
		float dx = pt.x - lastPos.x;
		if ( dx != 0 )
		{
			yaw_ += ( dx * 0.01f );
			handled = true;
		}
	}
	lastPos.x = pt.x;
	lastPos.y = pt.y;
	return handled;
}

void MouseCamera::init( const Matrix & v )
{
	view_ = v;
	viewToPolar();
}

void MouseCamera::init( const Vector3 & eye, const Vector3 & lookAt, const Vector3 & up )
{
	D3DXMatrixLookAtLH(&view_, &eye, &lookAt, &up);
	viewToPolar();
}

void MouseCamera::viewToPolar()
{
	Matrix invView( view_ );
	invView.invert();
	Vector3 * dir = (Vector3*)&invView.applyToUnitAxisVector(2);
	pitch_ = -atan2f( dir->y,	sqrtf( dir->z * dir->z + dir->x * dir->x ) );
	yaw_ = atan2f( dir->x, dir->z );
}

void MouseCamera::polarToView()
{
	if (!almostEqual( yaw_, lastYaw_ ) || !almostEqual( pitch_, lastPitch_ ))
	{
		//calculate the view matrix
		view_.invert();
		Vector3 pos( view_.applyToOrigin() );
		lastPos_ = pos;
		lastYaw_ = yaw_;
		lastPitch_ = pitch_;

		Matrix rot = Matrix::Identity;
		D3DXMatrixRotationYawPitchRoll( &rot, yaw_, pitch_, 0.f );
		Matrix tra = Matrix::Identity;
		tra.setTranslate(pos);
		rot.postMultiply(tra);
		view_.invert(rot);
	}
}

void MouseCamera::handleInput( float dTime )
{
	Matrix oldView = view_;
	bool viewChanged = false;
	view_.invert();

	Vector3 forward = view_.applyToUnitAxisVector( 2 );
	Vector3 up = view_.applyToUnitAxisVector( 1 );
	Vector3 right = view_.applyToUnitAxisVector( 0 );

	float movementSpeed;

	if (isKeyDown(VK_CAPITAL))
		movementSpeed = getSpeed();
	else
		movementSpeed = turboSpeed();

	//frame rate independent setSpeed, but capped
	if ( dTime < 0.1f )
		movementSpeed *= dTime;
	else
		movementSpeed *= 0.1f;

	if ( isKeyDown('W'))
	{
		if ( keyDown_['W'] )
		{
			//move forward
			view_._41 += forward.x * movementSpeed;
			view_._42 += forward.y * movementSpeed;
			view_._43 += forward.z * movementSpeed;
			viewChanged = true;
		}
	}
	else
	{
		keyDown_['W'] = false;
	}

	if (isKeyDown('S') )
	{
		if ( keyDown_['S'] )
		{
			//move back
			view_._41 -= forward.x * movementSpeed;
			view_._42 -= forward.y * movementSpeed;
			view_._43 -= forward.z * movementSpeed;
			viewChanged = true;
		}
	}
	else
	{
		keyDown_['S'] = false;
	}

	if (isKeyDown('A') )
	{
		if ( keyDown_['A'] )
		{
			//move left
			view_._41 -= right.x * movementSpeed;
			view_._42 -= right.y * movementSpeed;
			view_._43 -= right.z * movementSpeed;
			viewChanged = true;
		}
	}
	else
	{
		keyDown_['A'] = false;
	}

	if ( isKeyDown('D') )
	{
		if ( keyDown_['D'] )
		{
			//move right
			view_._41 += right.x * movementSpeed;
			view_._42 += right.y * movementSpeed;
			view_._43 += right.z * movementSpeed;
			viewChanged = true;
		}
	}
	else
	{
		keyDown_['D'] = false;
	}

	if (isKeyDown('E') )
	{
		if ( keyDown_['E'] )
		{
			//move up
			//view_._41 += up.x * movementSpeed;
			view_._42 += up.y * movementSpeed;
			//view_._43 += up.z * movementSpeed;
			viewChanged = true;
		}
	}
	else
	{
		keyDown_['E'] = false;
	}

	if (isKeyDown('Q') )
	{
		if ( keyDown_['Q'] )
		{
			//move down
			//view_._41 -= up.x * movementSpeed;
			view_._42 -= up.y * movementSpeed;
			//view_._43 -= up.z * movementSpeed;
			viewChanged = true;
		}
	}
	else
	{
		keyDown_['Q'] = false;
	}

	if (viewChanged)
	{
		// Cap the camera position
		view_._41 = clamp( limit_.minBounds().x, view_._41, limit_.maxBounds().x );
		view_._42 = clamp( limit_.minBounds().y, view_._42, limit_.maxBounds().y );
		view_._43 = clamp( limit_.minBounds().z, view_._43, limit_.maxBounds().z );
	}

	if (viewChanged)
	{
		view_.invert();
	}
	else
	{
		// Nothing changed, restore old view.
		view_ = oldView;
	}
}

void MouseCamera::setPosition( const Vector3 & eye )
{
	view_.invert();
	view_._41 = eye.x;
	view_._42 = eye.x;
	view_._43 = eye.x;
	view_.invert();
}
