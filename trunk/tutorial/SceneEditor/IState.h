#pragma once
#include "misc/stdHead.h"
enum eState
{
	eState_None,
	eState_TerrainHeight,
	eState_Data,
	eState_TerrainTexture,
	eState_PlaceModel,
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

#define InterfaceIdleHandler(terminal)\
public:\
	virtual void update() ##terminal\
	virtual void destroy() ##terminal\
	virtual void enter() ##terminal\
	virtual void leave() ##terminal\

	  virtual void setModelFile(const tstring& mf)
	  {

	  }
	  //
	  virtual void setSculptorStrength(float f)
	  {
	  }
	  virtual float getSculptorStrength() const
	  {
		  return 0.0f;
	  }
	  virtual void setSculptorRadius(float f)
	  {
	  }
	  virtual float getSculptorRadius()
	  {
		  return 0.0f;
	  }
	  //
	  virtual void setBrushStrength(float f)
	  {
	  }
	  virtual float getBrushStrength() const
	  {
		  return 0.0f;
	  }
	  virtual void setBrushRadiusInner(float f)
	  {
	  }
	  virtual void setBrushRadiusOuter(float f)
	  {
	  }
	  virtual float getBrushRadiusInner() const
	  {
		  return 0.0f;
	  }
	  virtual float getBrushRadiusOuter() const
	  {
		  return 0.0f;
	  }
	  //
	  virtual void onMouseLeftButtonUp(){} ;
	  virtual void onMouseLeftButtonDown(){} ;
	  virtual void onMouseRightButtonDown(){};
	  virtual void onMouseRightButtonUp(){};
protected:
	eState type_;
	
#define IState_Base InterfaceIdleHandler(=0;)
#define IState_Null InterfaceIdleHandler({};)
#define IState_Derived InterfaceIdleHandler(;)
	IState_Null
};