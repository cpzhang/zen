//#include <cctype>
#include "Skeleton.h"
#include "BoneNode.h"
#include "MZ.h"
#include "misc/FileSystem.h"
#include "tinyxml2/tinyxml2.h"
BoneNode* Skeleton::createBoneNode( const std::string& name )
{
	BoneNode* b = new BoneNode(this, name);
	
	if (b != NULL)
	{
		_boneNodes[name] = b;
	}

	return b;
}

Skeleton::Skeleton()
{
	_clear();
}

Skeleton::~Skeleton()
{
	_clear();
}

bool Skeleton::create(const std::string& fileName)
{
	FilePath_ = fileName;
	//
	std::ifstream f(fileName.c_str(), std::ios::binary);
	if (!f.good())
	{
		return false;
	}

	u32 version = 0;
	//	[Tag Size Data]
	int t;
	int s;
	while(f.good())
	{
		t = 0;
		s = 0;
		f.read((char*)&t, sizeof(int));
		f.read((char*)&s, sizeof(int));

		if (s <= 0)
		{
			continue;
		}

		char c[5];
		c[0] = *((char*)&t + 3);
		c[1] = *((char*)&t + 2);
		c[2] = *((char*)&t + 1);
		c[3] = *((char*)&t + 0);
		c[4] = 0;

		switch (t)
		{
		case 'MVER':
			{
				f.read((char*)&_version, sizeof(_version));
			}break;
		case 'MBON': // นว๗ภ
			{
				u32 nBones;
				f.read((char*)&nBones,sizeof(nBones));

				for (u32 i = 0;i < nBones;i++)
				{
					//
					Bone* bone = new Bone;
					
					//
					f.read((char*)&bone->id,sizeof(bone->id));

					u8 JointnameLen; 
					f.read((char*)&JointnameLen,sizeof(JointnameLen));

					char name[MODEL_NAME_NODE_SIZE+1];
					f.read((char*)name,JointnameLen);
					name[JointnameLen] = 0;
				
					//
					bone->name = name;

					//ascii2unicode(name, bone->name);

					int parent;
					f.read((char*)&parent,sizeof(parent));

					//
					bone->parent = parent;

					Matrix initialMatrix = Matrix::Identity;
					{
						f.read((char*)&initialMatrix, sizeof(initialMatrix));
					}
					bone->initialMatrix = initialMatrix;
					//
					BoneNode* node = createBoneNode(bone->name);
					//
					node->attachObject(bone);
				}
				if (nBones != 0)
				{
					_relate();
				}
				
			}break;
		default:
			{
				f.ignore(s);
			}break;
		}
	}
	//
	{
		tstring fn = FileSystem::removeFileExtension(fileName);
		fn += ".animation";
		decodeSkinAnimationXML(fn);
	}
	return true;
}

void Skeleton::destroy()
{
	for (BoneNodeMap::iterator it = _boneNodes.begin(); it != _boneNodes.end(); ++it)
	{
		BoneNode* r = it->second;
		if (r)
		{
			Bone* b = r->getBone();
			delete b;
			r->detachObject();
			delete r;
		}
	}
	/*for (NameSkinMap::iterator it = NameSkins_.begin(); it != NameSkins_.end(); ++it)
	{
		Skin* k = it->second;
		delete k;
	}*/
	NameSkins_.clear();
		
	_clear();
}

u16 Skeleton::getNumBoneNodes( void ) const
{
	return _boneNodes.size();
}

BoneNode* Skeleton::getBoneNode( const std::string& name )
{
	if (_boneNodes.find(name) == _boneNodes.end())
	{
		return NULL;
	}

	return _boneNodes[name];
}

bool Skeleton::hasBoneNode( const std::string& name )
{
	if (_boneNodes.find(name) == _boneNodes.end())
	{
		return false;
	}

	return true;
}

void Skeleton::update( const AnimationTime& a, Skin* s )
{
	AnimationTime at = a;
	at.current = at.current * 3 / 100;
	at.current++;
	at.current = at.current * 33.33333333f;
	for (size_t i = 0; i != _roots.size(); ++i)
	{
		BoneNodeMap::iterator it = _boneNodes.find(_roots[i]);
		if (it == _boneNodes.end())
		{
			return;
		}
		it->second->update(at, s);	
	}
}

void Skeleton::_clear()
{
	_boneNodes.clear();
	_matrices.clear();
	_roots.clear();
}

BoneNode* Skeleton::_getBoneNode( int id )
{
	for (BoneNodeMap::iterator it = _boneNodes.begin(); it != _boneNodes.end(); ++it)
	{
		BoneNode* node = it->second;
		Bone* bone = node->getBone();
		if (id == bone->id)
		{
			return node;
		}
	}
	return NULL;
}

void Skeleton::_relate()
{
	for (BoneNodeMap::iterator it = _boneNodes.begin(); it != _boneNodes.end(); ++it)
	{
		BoneNode* node = it->second;
		if (node)
		{
			Bone* bone = node->getBone();
			if (bone)
			{
				int parentID = bone->parent;
				BoneNode* parentNode = _getBoneNode(parentID);
				if (parentNode)
				{
					parentNode->addChild(node);
				}
				else
				{
					_roots.push_back(bone->name);
				}
			}
		}
	}
	if (_roots.empty())
	{
	//	throw Exception("no bone root found!");
	}
	
	//
	_matrices.resize(_boneNodes.size(), Matrix::Identity);
	_matricesFull.resize(_boneNodes.size(), Matrix::Identity);
}

BoneNodeMapIterator Skeleton::getCommandMapIterator( void ) const
{
	return BoneNodeMapIterator(_boneNodes.begin(), _boneNodes.end());
}

tstring Skeleton::getFilePath()
{
	return FilePath_;
}

bool Skeleton::decodeSkinAnimationXML( const std::string& fileName )
{
	SkinAnimationXMLFilePath_ = fileName;
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.LoadFile(fileName.c_str()))
	{
		return false;
	}
	tinyxml2::XMLElement* r = doc.RootElement();
	if (NULL == r)
	{
		return false;
	}
	tinyxml2::XMLElement* mat = r->FirstChildElement("animation");
	while(mat)
	{
		sSkinAnimation sa;
		sa.name = mat->Attribute("name");
		sa.skinFilePath = FileSystem::getParent(fileName);
		sa.skinFilePath = sa.skinFilePath + "\\" + mat->Attribute("skin");
		sa.duration = mat->FloatAttribute("duration");
		SkinAnimations_.push_back(sa);
		mat = mat->NextSiblingElement("animation");
	}
	return true;
}

int Skeleton::getSkinAnimationNumber()
{
	return SkinAnimations_.size();
}

sSkinAnimation* Skeleton::getSkinAnimation( int index )
{
	return &SkinAnimations_[index];
}

const tstring& Skeleton::getSkin( const tstring& skinAnimationName, AnimationTime& at)
{
	sSkinAnimation* a = NULL;
	for (int i = 0; i != SkinAnimations_.size(); ++i)
	{
		if (SkinAnimations_[i].name == skinAnimationName)
		{
			a = &SkinAnimations_[i];
			break;
		}
	}
	if (NULL == a && !SkinAnimations_.empty())
	{
		a = &SkinAnimations_[0];
	}
	if (NULL == a)
	{
		return "";
	}
	at.end = a->duration;
	return a->skinFilePath;
}

int Skeleton::getBoneID( const std::string& name )
{
	if (_boneNodes.find(name) == _boneNodes.end())
	{
		return Bone::tInvalidID;
	}
	return _boneNodes[name]->getBone()->id;
}
