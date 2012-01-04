#pragma once
//
class Camera
{
public:
public:
	float getFov() const
	{
		return fov_;
	}
	float getAspectRatio() const
	{
		return aspectRatio_;
	}
	float getNearPlane() const
	{
		return nearPlane_;
	}
	float getFarPlane() const
	{
		return farPlane_;
	}

private:
	float	nearPlane_;
	float	farPlane_;
	float	fov_;
	float	aspectRatio_;
	float	viewHeight_;
	bool	ortho_;
};