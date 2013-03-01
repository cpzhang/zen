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

	  void setBrushStrength(float f)
	  {
		  BrushStrength_ = f;
	  }
	  float getBrushStrength() const
	  {
		  return BrushStrength_;
	  }
protected:
	eState type_;
	float BrushStrength_;
	
#define IState_Base InterfaceIdleHandler(=0;)
#define IState_Null InterfaceIdleHandler({};)
#define IState_Derived InterfaceIdleHandler(;)
	IState_Null
};