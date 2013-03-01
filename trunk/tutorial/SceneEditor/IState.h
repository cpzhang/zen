#pragma once
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
		  BrushStrength_ = 1.0f;
	  }
	  virtual ~IState(){};

#define InterfaceIdleHandler(terminal)\
public:\
	virtual void update() ##terminal\
	virtual void destroy() ##terminal\
	virtual void enter() ##terminal\
	virtual void leave() ##terminal\

	  virtual void setBrushStrength(float f)
	  {
		  BrushStrength_ = f;
	  }
	  float getBrushStrength() const
	  {
		  return BrushStrength_;
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
protected:
	eState type_;
	float BrushStrength_;
	
#define IState_Base InterfaceIdleHandler(=0;)
#define IState_Null InterfaceIdleHandler({};)
#define IState_Derived InterfaceIdleHandler(;)
	IState_Null
};