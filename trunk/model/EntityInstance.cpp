#include "EntityInstance.h"
#include "tinyXML2/tinyxml2.h"
#include "misc/FileSystem.h"
#include "Entity.h"
#include "Skeleton.h"
#include "Skin.h"
void EntityInstance::render()
{
	Entity_->render();
}

bool EntityInstance::create( const tstring& resourceId )
{
	//
	Entity_ =  getEntityManager()->get(resourceId);
	FileName_ = resourceId;
	return true;
}

void EntityInstance::destroy()
{

}

void EntityInstance::setEntity( Entity* e )
{
	Entity_ = e;
	SkinCurrent_ = NULL;
}

void EntityInstance::setAnimation( const tstring& resourceId )
{
	if (Entity_)
	{
		Skeleton* k = Entity_->getSkeleton();
		if (k)
		{
			SkinCurrent_ = k->getSkin(resourceId, AnimationTime_);
		}
	}
}

void EntityInstance::update( float delta )
{
	if (SkinCurrent_)
	{
		AnimationTime_.current += delta * Speed_;
		if (AnimationTime_.current > AnimationTime_.end)
		{
			AnimationTime_.current = AnimationTime_.start;
		}
		Entity_->getSkeleton()->update(AnimationTime_, SkinCurrent_);
	}
}

void EntityInstance::setSpeed( float s )
{
	Speed_ = s;
}

Create_Singleton_Imp(EntityInstanceManager, ApiModel_)