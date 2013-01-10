#include "Entity.h"
#include "tinyXML2/tinyxml2.h"
#include "misc/FileSystem.h"
#include "Part.h"
#include "Skeleton.h"
void Entity::render()
{
	for (size_t i = 0; i != Parts_.size(); ++i)
	{
		Part* pi = Parts_[i];
		if (pi)
		{
			pi->render();
		}
	}
}

void Entity::addPart( const tstring& resourceId )
{
	Part* pi = getPartManager()->get(resourceId);
	Parts_.push_back(pi);
}

bool Entity::create( const tstring& resourceId )
{
	//
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.LoadFile(resourceId.c_str()))
	{
		return false;
	}
	FileName_ = resourceId;
	tinyxml2::XMLElement* r = doc.RootElement();
	Name_ = r->Attribute("name");
	tinyxml2::XMLElement* tex= r->FirstChildElement("part");
	std::string parentPath = FileSystem::standardFilePath(FileName_);
	parentPath = FileSystem::getParent(parentPath);
	parentPath = FileSystem::getParent(parentPath);
	parentPath += "/";
	while (tex)
	{
		std::string subEntityFileName = tex->Attribute("file");
		Part* sub = getPartManager()->get(parentPath + subEntityFileName);
		sub->setEntity(this);
		Parts_.push_back(sub);
		tex = tex->NextSiblingElement("part");
	}
	//skeleton
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement("skeleton");
		if (mat)
		{
			std::string skeleton = mat->Attribute("file");
			Skeleton_ = getSkeletonManager()->get(parentPath + skeleton);
		}
	}
	
	return true;
}

void Entity::destroy()
{

}

size_t Entity::getPartNumber()
{
	return Parts_.size();
}

Part* Entity::getPart( size_t index )
{
	return Parts_[index];
}

Skeleton* Entity::getSkeleton()
{
	return Skeleton_;
}

Create_Singleton_Imp(EntityManager, ApiModel_)