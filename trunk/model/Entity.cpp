#include "Entity.h"
#include "tinyXML2/tinyxml2.h"
#include "misc/FileSystem.h"
#include "PartInstance.h"
void Entity::render()
{
	for (size_t i = 0; i != PartInstances_.size(); ++i)
	{
		PartInstance* pi = PartInstances_[i];
		if (pi)
		{
			pi->render();
		}
	}
}

void Entity::addPartInstance( const tstring& resourceId )
{
	PartInstance* pi = getPartInstanceManager()->get(resourceId);
	PartInstances_.push_back(pi);
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
		PartInstance* sub = getPartInstanceManager()->get(parentPath + subEntityFileName);
		PartInstances_.push_back(sub);
		tex = tex->NextSiblingElement("part");
	}
	return true;
}

void Entity::destroy()
{

}

size_t Entity::getPartInstanceNumber()
{
	return PartInstances_.size();
}

PartInstance* Entity::getPartInstance( size_t index )
{
	return PartInstances_[index];
}

Create_Singleton_Imp(EntityManager, ApiModel_)