#include "PartInstance.h"
#include "Part.h"
//#include "MeshManager.h"
PartInstance::PartInstance()
{

}

void PartInstance::render()
{
	part_->render();
}

void PartInstance::clear_()
{
	part_ = NULL;
}

bool PartInstance::create( const tstring& resourceId )
{
	part_ = getPartManager()->get(resourceId);
	return true;
}

void PartInstance::destroy()
{

}

Part* PartInstance::getPart()
{
	return part_;
}

Create_Singleton_Imp(PartInstanceManager, ApiModel_)