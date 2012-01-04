#pragma once
#include "boundingbox.h"
#include "math.h"
//
class MouseCamera
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
	MouseCamera()
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
public:
	float speed() const
	{
		return speed_[0];
	}

	void speed( float s )
	{
		speed_[0] = s;
	}

	float turboSpeed() const
	{
		return speed_[1];
	}

	void turboSpeed( float s )
	{
		speed_[1] = s;
	}
	void update( float dTime)
	{
		if (handleKeyEvent())
		{
			handleInput(dTime);
		}
		else if (handleMouseEvent())
		{
			polarToView();
		}
	}
	bool handleKeyEvent( )
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
	bool handleMouseEvent()
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
	void view( const Matrix & v )
	{
		view_ = v;
		viewToPolar();
	}
	void viewToPolar()
	{
		Matrix invView( view_ );
		invView.invert();
		Vector3 * dir = (Vector3*)&invView.applyToUnitAxisVector(2);
		pitch_ = -atan2f( dir->y,	sqrtf( dir->z * dir->z + dir->x * dir->x ) );
		yaw_ = atan2f( dir->x, dir->z );
	}

	void polarToView()
	{
		Matrix oldView = view_;
		//calculate the view matrix
		view_.invert();

		Vector3 pos( view_.applyToOrigin() );

		if (!almostEqual( pos.x, lastPos_.x ) || !almostEqual( pos.y, lastPos_.y ) || !almostEqual( pos.z, lastPos_.z ) ||
			!almostEqual( yaw_, lastYaw_ ) || !almostEqual( pitch_, lastPitch_ ))
		{
			lastPos_ = pos;
			lastYaw_ = yaw_;
			lastPitch_ = pitch_;

			view_.setIdentity();

			Matrix rot;
			D3DXMatrixRotationYawPitchRoll( &rot, yaw_, pitch_, 0.f );

			view_.setTranslate( pos );
			view_.preMultiply( rot );

			view_.invert();
		}
		else
		{
			view_ = oldView;
		}
	}

	void handleInput( float dTime )
	{
		Matrix oldView = view_;
		bool viewChanged = false;
		view_.invert();

		Vector3 forward = view_.applyToUnitAxisVector( 2 );
		Vector3 up = view_.applyToUnitAxisVector( 1 );
		Vector3 right = view_.applyToUnitAxisVector( 0 );

		float movementSpeed;

		if (isKeyDown(VK_CAPITAL))
			movementSpeed = speed();
		else
			movementSpeed = turboSpeed();

		//frame rate independent speed, but capped
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
				view_._41 += up.x * movementSpeed;
				view_._42 += up.y * movementSpeed;
				view_._43 += up.z * movementSpeed;
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
				view_._41 -= up.x * movementSpeed;
				view_._42 -= up.y * movementSpeed;
				view_._43 -= up.z * movementSpeed;
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
};