#include "Mesh.h"
#include "Mz.h"
#include "misc/stdHead.h"
#include "misc/FileSystem.h"
#include "tinyXML2/tinyxml2.h"
#include "render/BufferLocker.h"
#include "Skin.h"
//
Mesh::Mesh()
{
	clear();
}

Mesh::~Mesh()
{

}

bool Mesh::createFromMZ(size_t sub, Mz* mz)
{
	vdt_ = eVertexDeclarationType_PositionTextureBoneWeightColorNormal;
	//
	if (sub >= mz->mSubmeshes.size())
	{
		return false;
	}

	_name = mz->mSubmeshes[sub].name;

	//
	size_t vStart = mz->mSubmeshes[sub].vstart;
	_vertices.resize(mz->mSubmeshes[sub].vcount);
	std::ostringstream ss;
	ss<<"mesh "<<sub;
	//Record(ss);
	Vector4 c;
	for (size_t i = 0; i != _vertices.size(); ++i)
	{
		memcpy(&_vertices[i].position_[0], mz->mVertices[i + vStart].pos, sizeof(mz->mVertices[i].pos));
		memcpy(&_vertices[i].normal_[0], mz->mVertices[i + vStart].normal, sizeof(mz->mVertices[i].normal));
		memcpy(&_vertices[i].texcoord_[0], mz->mVertices[i+ vStart].texcoords, sizeof(mz->mVertices[i].texcoords));
		memcpy(&c.x, mz->mVertices[i+ vStart].color, sizeof(mz->mVertices[i].color));
		_vertices[i].color_ARGB_ = c.getARGB();
		for(size_t k = 0; k != 4; ++k)
		{
			u8 id = mz->mVertices[i+ vStart].bones[k];
			if (id < 255)
			{
				++_bones[id];
			}
			//_vertices[i].bones_[k] = id;
		}
		memcpy(&_vertices[i].weights_[0], mz->mVertices[i+ vStart].weights, sizeof(mz->mVertices[i].weights));
	}
	//骨骼重现分配
	for (size_t i = 0; i != _vertices.size(); ++i)
	{
		for(size_t k = 0; k != 4; ++k)
		{
			u8 id = mz->mVertices[i+ vStart].bones[k];
			//
			if (_bones.find(id) == _bones.end())
			{
				break;
			}
			size_t d = std::distance(_bones.begin(), _bones.find(id));
			_vertices[i].bones_[k] = d;
		}
	}
	//
	_faces.resize(mz->mSubmeshes[sub].icount/3);
	size_t iStart = mz->mSubmeshes[sub].istart / 3;
	for (size_t i = 0; i != _faces.size(); ++i)
	{
		_faces[i].index[0] = mz->mFaces[i + iStart].index[0] - vStart;
		_faces[i].index[1] = mz->mFaces[i + iStart].index[1] - vStart;
		_faces[i].index[2] = mz->mFaces[i + iStart].index[2] - vStart;
	}

	//
	//initBuffer_();
	//onLoaded_();
	return true;
}

void Mesh::destroy()
{
	vertexBuffer_.destroy();
	vertexBufferHW_.destroy();
	indexBuffer_.destroy();
	clear();
}

void Mesh::clear()
{
	version_ = 0;
	_vertices.clear();
	_faces.clear();
	vdt_ = eVertexDeclarationType_Null;
	//mAABB.reset();
}

bool Mesh::initBuffer_()
{
	{
		vertexBuffer_.create(_vertices.size() * sVDT_PositionTextureBoneWeightColorNormal::getSize(),0, 0, D3DPOOL_MANAGED);
		BufferLocker<VertexBuffer, sVDT_PositionTextureBoneWeightColorNormal> vl(vertexBuffer_);
		vl.fill(&_vertices[0], _vertices.size()*sVDT_PositionTextureBoneWeightColorNormal::getSize());
	}
	{
		vertexBufferHW_.create(_vertices.size() * sVDT_PositionTextureBoneWeightColorNormal::getSize(),0, 0, D3DPOOL_MANAGED);
		BufferLocker<VertexBuffer, sVDT_PositionTextureBoneWeightColorNormal> vl(vertexBufferHW_);
		vl.fill(&_vertices[0], _vertices.size()*sVDT_PositionTextureBoneWeightColorNormal::getSize());
	}
	//
	{
		indexBuffer_.create(_faces.size() * sizeof(sFace), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED);
		BufferLocker<IndexBuffer, u16> vl(indexBuffer_);
		vl.fill(&_faces[0], _faces.size()*sizeof(sFace));
	}

	return true;
}

void Mesh::render()
{
	vertexBufferHW_.apply(0, 0, sVDT_PositionTextureBoneWeightColorNormal::getSize());
	indexBuffer_.apply();
 	getRenderContex()->setVertexDeclaration(sVDT_PositionTextureBoneWeightColorNormal::getType());
	getRenderContex()->drawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _vertices.size(), 0, _faces.size());
}

void Mesh::render( const std::vector<Matrix>& ms )
{
	if (ms.empty())
	{
		return render();
	}
	VertexPTBWCVec tvs(_vertices);
	{
		for (size_t i = 0; i != tvs.size(); ++i)
		{
			sVDT_PositionTextureBoneWeightColorNormal& p = tvs[i];
			Vector3 t = Vector3::Zero; 
			for (size_t k = 0; k != 4; ++k)
			{
				if (p.weights_[k] < 0.00001f)
				{
					break;
				}
				t += ms[p.bones_[k]].applyVector(p.position_) * p.weights_[k];
			}
			p.position_ = t;
		}
		BufferLocker<VertexBuffer, sVDT_PositionTextureBoneWeightColorNormal> vl(vertexBuffer_);
		vl.fill(&tvs[0], tvs.size()*sVDT_PositionTextureBoneWeightColorNormal::getSize());
	}
	vertexBuffer_.apply(0, 0, sVDT_PositionTextureBoneWeightColorNormal::getSize());
	indexBuffer_.apply();
	getRenderContex()->setVertexDeclaration(sVDT_PositionTextureBoneWeightColorNormal::getType());
	getRenderContex()->drawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _vertices.size(), 0, _faces.size());
}

int Mesh::getFaceNumber()
{
	return _faces.size();
}

int Mesh::getVertexNumber()
{
	return _vertices.size();
}

bool Mesh::save( const std::string& path )
{
	//============================================================================
	// 开始写入数据
	ChunkSet cs;

	//============================================================================
	// 版本号
	cs.beginChunk("MVER");	
	cs.write(&version_, sizeof(version_));
	cs.endChunk();

	//============================================================================
	// 顶点
// 	sVDT_Null* vdt = getVDTManager()->mLayouts[vdt_];
 	cs.beginChunk("MVTX");
	cs.write(&_vertices[0], sVDT_PositionTextureBoneWeightColorNormal::getSize() * _vertices.size());
 	cs.endChunk();

	//============================================================================
	// 索引
	cs.beginChunk("MFAC");
	cs.write(&_faces[0], sizeof(sFace) * _faces.size());
	cs.endChunk();

	//============================================================================
	// 保存文件，结束
	cs.save(path);

	return true;
}

void Mesh::saveSkeleton( const std::string& fileName )
{
	if (_bones.empty())
	{
		return;
	}
	//
	Mz* mz;
	//============================================================================
	// 开始写入数据
	ChunkSet cs;

	//============================================================================
	// 版本号
	cs.beginChunk("MVER");	
	cs.write(&mz->mVersion, sizeof(mz->mVersion));
	cs.endChunk();

	//============================================================================
	// 顶点
	cs.beginChunk("MBON");
	u32 nBones = _bones.size();
	cs.write(&nBones, sizeof(nBones));
	nBones = mz->mBones.size();
	for (size_t i = 0; i != nBones; ++i)
	{
		BoneIDReferenceMap::iterator it = _bones.find(i);
		if (it == _bones.end())
		{
			continue;
		}
		Bone& b = mz->mBones[i];
		it = _bones.find(b.parent);
		if (it == _bones.end())
		{
			continue;
		}
		cs.write(&b.id, sizeof(b.id));

		u8 nameLength = b.name.size();
		cs.write(&nameLength, sizeof(nameLength));

		cs.write(b.name.c_str(), b.name.size());

		cs.write(&b.parent, sizeof(b.parent));

		cs.write(&b.initialMatrix, sizeof(b.initialMatrix));
	}

	cs.endChunk();

	//============================================================================
	// 保存文件，结束
	cs.save(fileName);
}

void Mesh::saveSkin( const std::string& fileName )
{
	//
	if (_bones.empty())
	{
		return;
	}
	//
	Mz* mz;
	for (size_t i = 0; i != mz->mSkins.size(); ++i)
	{
		std::string path = fileName + "/" + mz->mAnimations[i].name + ".skin";
		Skin& s = mz->mSkins[i];
		//============================================================================
		// 开始写入数据
		ChunkSet cs;

		//============================================================================
		// 版本号
		cs.beginChunk("MVER");	
		cs.write(&mz->mVersion, sizeof(mz->mVersion));
		cs.endChunk();

		//============================================================================
		// 顶点
		cs.beginChunk("MBON");
		u32 nBones = _bones.size();
		cs.write(&nBones, sizeof(nBones));
		for (size_t i = 0; i != nBones; ++i)
		{
			sBoneKFs& b = s.boneKFs[_bones[i]];
			//
			u32 num = b.translationKFs.numKeyFrames();
			cs.write(&num, sizeof(num));
			writeSequence(cs, b.translationKFs._keyFrames);
			//
			num = b.rotationKFs.numKeyFrames();
			cs.write(&num, sizeof(num));
			writeSequence(cs, b.rotationKFs._keyFrames);
			//
			num = b.scaleKFs.numKeyFrames();
			cs.write(&num, sizeof(num));
			writeSequence(cs, b.scaleKFs._keyFrames);
		}

		cs.endChunk();

		//============================================================================
		// 保存文件，结束
		cs.save(fileName);
	}
}
struct sV
{

	//
	Vector3	position_;
	//
	Vector2	texcoord_;
	//
	Vector4 bones_;
	//
	Vector4    weights_;
	//
	u32		color_ARGB_;

	Vector3	normal_;
};
bool Mesh::create( const std::string& fileName )
{
	filePath_ = fileName;
	std::ifstream f(fileName.c_str(), std::ios::binary);
	if (!f.good())
	{
		return false;
	}

	//	[Tag Size Data]
	int t;
	size_t s;
	while(f.good())
	{
		t = 0;
		s = 0;
		f.read((char*)&t, sizeof(int));
		f.read((char*)&s, sizeof(size_t));

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
				f.read((char*)&version_, s);
			}
			break;

		case 'MVTX':
			{

 				vdt_ = eVertexDeclarationType_PositionTextureBoneWeightColorNormal;
				_vertices.resize(s / sVDT_PositionTextureBoneWeightColorNormal::getSize());
 				f.read((char*)&_vertices[0], s);
			}
			break;
		case 'MFAC':
			{
				_faces.resize(s / sizeof(sFace));
				f.read((char*)&_faces[0], s);
			}
			break;
		}
	}

	initBuffer_();
	//
	tstring bm = FileSystem::removeFileExtension(filePath_) + ".boneMapping";
	loadBoneMapping(bm);
	onLoaded_();
	return true;
}

void Mesh::saveBoneMapping( const std::string& fileName, Mz* mz)
{
	if (_bones.empty())
	{
		return;
	}
	//============================================================================
	// 开始写入数据
	ChunkSet cs;

	//============================================================================
	// 版本号
	cs.beginChunk("MVER");	
	cs.write(&mz->mVersion, sizeof(mz->mVersion));
	cs.endChunk();

	//============================================================================
	// 顶点
	cs.beginChunk("MBON");
	u32 nBones = _bones.size();
	cs.write(&nBones, sizeof(nBones));

	for(BoneIDReferenceMap::iterator it = _bones.begin(); it != _bones.end(); ++it)
	{
		u8 id = std::distance(_bones.begin(), it);
		u8 boneID = it->first;
		//cs.write(&id, sizeof(u8));
		cs.write(&boneID, sizeof(u8));
	}

	cs.endChunk();

	//============================================================================
	// 保存文件，结束
	cs.save(fileName);
}

void Mesh::loadBoneMapping( const tstring& fileName )
{
	std::ifstream f(fileName.c_str(), std::ios::binary);
	if (!f.good())
	{
		return;
	}

	//	[Tag Size Data]
	int t;
	size_t s;
	while(f.good())
	{
		t = 0;
		s = 0;
		f.read((char*)&t, sizeof(int));
		f.read((char*)&s, sizeof(size_t));

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
				f.read((char*)&version_, s);
			}
			break;

		case 'MBON':
			{
				u32 nBones;
				f.read((char*)&nBones, sizeof(u32));
				//
				for (size_t i = 0; i != nBones; ++i)
				{
					u8 id;
					f.read((char*)&id, sizeof(u8));
					++_bones[id];
				}
			}
			break;
		}
	}
}

void Mesh::saveMaterial( const std::string& fileName )
{
	tinyxml2::XMLDocument doc;
	// 
	tinyxml2::XMLDeclaration* dec = doc.NewDeclaration("xml version=\"1.0\"");
	doc.LinkEndChild(dec);
	//
	tinyxml2::XMLElement* ele = doc.NewElement("material");
	doc.LinkEndChild(ele);
	//
	doc.SaveFile(fileName.c_str());
}

void Mesh::onLoaded_()
{
	_initAABB();
}

void Mesh::_initAABB()
{
	for (size_t i = 0; i != _vertices.size(); ++i)
	{
		mAABB.addBounds(_vertices[i].position_);
	}
}

BoundingBox* Mesh::getAABB()
{
	return &mAABB;
}

int Mesh::getVersion()
{
	return version_;
}

tstring Mesh::getFilePath()
{
	return filePath_;
}

//Create_Singleton_Imp(MeshManager, ApiModel_)