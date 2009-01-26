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
	std::string resId(resourceId);
	if (!FileSystem::isFileExist(resId))
	{
		resId = FileSystem::getDataDirectory() + "\\" + resId;
		if (!FileSystem::isFileExist(resId))
		{
			return false;
		}
	}
	
	//
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.LoadFile(resId.c_str()))
	{
		return false;
	}
	FileName_ = resId;
	tinyxml2::XMLElement* r = doc.RootElement();
	Name_ = r->Attribute("name");
	tinyxml2::XMLElement* tex= r->FirstChildElement("part");
	std::string parentPath = FileSystem::standardFilePath(FileName_);
	parentPath = FileSystem::getParent(parentPath);
	parentPath += "\\";
	while (tex)
	{
		std::string subEntityFileName = tex->Attribute("file");
		Parts_.push_back(parentPath + subEntityFileName);
		tex = tex->NextSiblingElement("part");
	}
	//skeleton
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement("skeleton");
		if (mat)
		{
			std::string skeleton = mat->Attribute("file");
			NameSkeleton_ = parentPath + skeleton;
		}
	}
	//
	tex= r->FirstChildElement("particle");
	while (tex)
	{
		std::string subEntityFileName = tex->Attribute("file");
		NameParticles_.push_back(FileSystem::getDataDirectory() + "\\" + subEntityFileName);
		//
		{
			const char* n = tex->Attribute("bone");
			if (NULL != n)
			{
				NameParticleBones_.push_back(n);
			}
		}
		tex = tex->NextSiblingElement("particle");
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

size_t Entity::getParticleNumber()
{
	return NameParticles_.size();
}

const tstring& Entity::getParticleName( const size_t index )
{
	return NameParticles_[index];
}

const tstring& Entity::getParticleBoneName( const size_t index )
{
	return NameParticleBones_[index];
}

//Create_Singleton_Imp(EntityManager, ApiModel_)