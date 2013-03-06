#include "NavState.h"
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
#include "font/FlowText.h"
#include "scene/navigation.h"
NavState::~NavState()
{

}

NavState::NavState()
{
	type_ = eState_Nav;
	Begin_ = Vector3::Zero;
	End_ =  Vector3::Zero;
}
void NavState::update()
{
}

void NavState::enter()
{
	
}

void NavState::leave()
{
	
}

void NavState::destroy()
{
}

void NavState::onMouseLeftButtonUp()
{
	if (getSceneManager() && getSceneManager()->getTerrain())
	{
		Vector2 p = getSceneManager()->getPickingPoint();
		Begin_.x = p.x;
		Begin_.z = p.y;
		Begin_.y = getSceneManager()->getTerrain()->getHeightFromeWorldSpacePosition(Begin_.x, Begin_.z);
	}
}

void NavState::onMouseRightButtonUp()
{
// 	if (getSceneManager() && getSceneManager()->getTerrain())
// 	{
// 		Vector2 p = getSceneManager()->getPickingPoint();
// 		End_.x = p.x;
// 		End_.z = p.y;
// 		End_.y = getSceneManager()->getTerrain()->getHeightFromeWorldSpacePosition(End_.x, End_.z);
// 		//
// 		NAVIGATION* n = getGlobal()->getRecastNav();
// 		if (n)
// 		{
// 			NAVIGATIONPATH p;
// 			p.start_location = Begin_;
// 			p.end_location = End_;
// 			NAVIGATIONPATHDATA d;
// 			if (NAVIGATION_get_path( n, &p, &d) >= 0)
// 			{
// 				getGlobal()->resetPath();
// 				for (int i = 0; i != d.path_point_count; ++i)
// 				{
// 					getGlobal()->addPath(d.path_point_array[i]);
// 				}
// 			}
// 		}
// 	}
}
