#include "PlaceModelState.h"
#include "misc/helper.h"
#include "misc/windowsHead.h"
#include "scene/Terrain.h"
#include "scene/SceneManager.h"
#include "Global.h"
#include "scene/Chunk.h"
#include "render/math.h"
#include "scene/QuadNode.h"
void PlaceModelState::update()
{
	
}

PlaceModelState::~PlaceModelState()
{

}

PlaceModelState::PlaceModelState()
{
	type_ = eState_PlaceModel;
}

void PlaceModelState::enter()
{
	
}

void PlaceModelState::leave()
{
	//getGlobal()->getBrushDecal()->setRadius(50);
}

void PlaceModelState::destroy()
{
}