#include "Entity.h"
#include "tinyXML2/tinyxml2.h"
#include "misc/FileSystem.h"
#include "Part.h"
#include "Skeleton.h"
void Entity::addPart( const tstring& resID)
{
	Parts_.push_back(resID);
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
	//parentPath = FileSystem::getParent(parentPath);
	parentPath += "/";
	while (tex)
	{
		std::string subEntityFileName = tex->Attribute("file");
		//Part* sub = ModelResManager::getInstance()->get<Part>();
		//sub->setEntity(this);
		Parts_.push_back(parentPath + subEntityFileName);
		tex = tex->NextSiblingElement("part");
	}
	//skeleton
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement("skeleton");
		if (mat)
		{
			std::string skeleton = mat->Attribute("file");
			//Skeleton_ = ModelResManager::getInstance()->get<Skeleton>(parentPath + skeleton);
			NameSkeleton_ = parentPath + skeleton;
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

const tstring&  Entity::getPart( size_t index )
{
	return Parts_[index];
}

const tstring& Entity::getSkeleton()
{
	return NameSkeleton_;
}
//Create_Singleton_Imp(EntityManager, ApiModel_)