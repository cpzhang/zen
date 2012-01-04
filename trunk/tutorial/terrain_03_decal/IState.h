#pragma once
enum eState
{
	eState_None,
	eState_TerrainHeight,
	eState_Size,
};
class IState
{
public:
	IState()
	{
		type_ = eState_None;
	}
	virtual ~IState(){};
public:
	virtual void update() {};
	virtual void destroy(){}
	virtual void enter()
	{

	}
	virtual void leave()
	{

	}
protected:
	eState type_;
};