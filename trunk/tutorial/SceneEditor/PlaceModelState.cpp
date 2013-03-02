#include "PlaceModelState.h"
#include "misc/helper.h"
#include "misc/windowsHead.h"
//
#include "render/math.h"
//
#include "model/EntityInstance.h"
//
#include "scene/Terrain.h"
#include "scene/SceneManager.h"
#include "scene/Chunk.h"
#include "scene/QuadNode.h"
//
#include "Global.h"
#include "FlowText.h"
void PlaceModelState::update()
{
	if (ModelShadow_)
	{
		Vector2 pp = getSceneManager()->getPickingPoint();
		//float h = getSceneManager()->getTerrain()->getHeightFromeWorldSpacePosition(pp.x, pp.y);
		ModelShadow_->setPosition(Vector3(pp.x, 0.0f, pp.y));
	}
}

PlaceModelState::~PlaceModelState()
{

}

PlaceModelState::PlaceModelState()
{
	type_ = eState_PlaceModel;
	ModelShadow_ = NULL;
}

void PlaceModelState::enter()
{
	
}

void PlaceModelState::leave()
{
	
}

void PlaceModelState::destroy()
{
}

void PlaceModelState::setModelFile( const tstring& mf )
{
	//ModelFile_ = "model\\";
	ModelFile_ = mf;
	std::ostringstream ss;
	ss<<"放置物件："<<ModelFile_;
	FlowText::getSingletonP()->add(ss.str(), Vector4(1, 1, 1, 1));
	ModelShadow_ = getSceneManager()->addEntityInstance(ModelFile_);
	ModelShadow_->scale(Vector3(0.1f, 0.1f, 0.1f));
}

tstring PlaceModelState::getModelFile()
{
	return ModelFile_;
}

void PlaceModelState::onMouseLeftButtonUp()
{
	//加入场景
	ModelShadow_ = NULL;
}

void PlaceModelState::onMouseRightButtonUp()
{
	//取消加入
	if (ModelShadow_)
	{
		getSceneManager()->removeEntityInstance(ModelShadow_);
		ModelShadow_ = NULL;
	}
}
