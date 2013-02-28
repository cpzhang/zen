#pragma once
//
#include "misc/helper.h"
#include "ConfigRender.h"
class ApiRender_ Camera
{
public:
	Camera( float nP, float fP, float f, float aR ) :
		nearPlane_( nP ),
		farPlane_( fP ),
		fov_( f ),
		aspectRatio_( aR ),
		viewHeight_( 100 ),
		ortho_( false )
	{

	}
	Camera( const Camera& camera )
	{
		*this=camera;
	}

public:
	float getFov() const
	{
		return fov_;
	}
	float getAspectRatio() const
	{
		return aspectRatio_;
	}
	void setAspectRatio(float ar)
	{
		aspectRatio_ = ar;
	}
	void setNearPlane(float f)
	{
		nearPlane_ = f;
	}
	void setFarPlane(float f)
	{
		farPlane_= f;
	}
	float getNearPlane() const
	{
		return nearPlane_;
	}
	float getFarPlane() const
	{
		return farPlane_;
	}
	bool isOrtho() const
	{
		return ortho_;
	}
	void setOrtho(bool b)
	{
		ortho_ = b;
	}
	float getWidth() const
	{
		return (viewHeight_ * aspectRatio_);
	}
	float getHeight() const
	{
		return viewHeight_;
	}
	void setHeight(float h)
	{
		viewHeight_ = h;
	}
private:
	float	nearPlane_;
	float	farPlane_;
	float	fov_;
	float	aspectRatio_;
	float	viewHeight_;
	bool	ortho_;
};