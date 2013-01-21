#include "Mz.h"
#include "misc/FileSystem.h"
#include "tinyXML2/tinyxml2.h"
#include "Material.h"
#include "Skin.h"
#include "render/Vector3.h"
Matrix getBasisMatrix()
{
	Matrix m;
	m.setIdentity();
	m.setRow(0, Vector4(1, 0, 0, 0));
	m.setRow(1, Vector4(0, 1, 0, 0));
	m.setRow(2, Vector4(0, 0, -1, 0));
	return m;
}
Matrix getBasisMatrixInvese()
{
	Matrix m = getBasisMatrix();
	return m.getInverse();
}
Matrix yz2dx(const Matrix& m)
{
	return getBasisMatrixInvese() * m * getBasisMatrix();
}
Vector3 yz2dx(const Vector3& m)
{
	return getBasisMatrix().applyVector(m);
}
Quaternion yz2dx(const Quaternion& m)
{
	Matrix t;
	t.setRotate(&m);
	t = yz2dx(t);
	Quaternion q(t);
	return q;
}
bool Mz::load( const std::string& fileName )
{
	//
	destroy();

	mzFileName_ = fileName;
	std::string mziFileName(fileName + "i");
	mziFileName = FileSystem::standardFilePath(mziFileName);
	loadMzI(mziFileName);
	//
	std::ifstream f(fileName.c_str(), std::ios::binary);
	if (!f.good())
	{
		return NULL;
	}

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
				decodeVersion(f, s);
			}
			break;

		case 'MSUB':
			{
				decodeSubMesh(f, s);
			}
			break;
		case 'MVTX':
			{
				decodeVertices(f, s);
			}
			break;
		case 'MFAC':
			{
				decodeFaces(f, s);
			}
			break;
		case 'MMTX': // 模型的材质
			{
				decodeMaterial(f, s);
			}
			break;
		case 'MANM':
			{
				decodeAnimation(f, s);
			}
			break;
		case 'MWRD': 
			{
				//废弃
				u32 numBones;
				f.read((char*)&numBones,sizeof(numBones));

				struct tranMatrix
				{
					float m_mat[4][3];
				};
				for(int i = 0;i < numBones;i++)
				{
					tranMatrix tm;
					f.read((char*)&tm,sizeof(tm));
				}
			}
			break;
		case 'MBON': // 模型骨骼信息
			{
				decodeBone(f, s);
			}
			break;
		default:
			{
				f.ignore(s);
				//Error("遇到无法识别的数据块");
			}
			break;
		}
	}
	f.close();
	//
	if (!mParticleEmitter.empty())
	{
		tstring n = FileSystem::removeParent(mzFileName_);
		n = FileSystem::removeFileExtension(n);
		tstring fn = FileSystem::getDataDirectory();
		std::ostringstream ss;
		for (size_t i = 0; i != mParticleEmitter.size(); ++i)
		{
			ss.str("");
			ss.clear();
			ss<<fn<<"/particle/"<<n<<"_"<<i<<".particle";
			mParticleEmitter[i].save(ss.str());
		}
	}
	return true;
}

void Mz::loadMzI( const std::string& fileName )
{
	//
	fbFileNames_.clear();
	//
	std::ifstream f(fileName.c_str(), std::ios::binary);
	if (!f.good())
	{
		return;
	}
	u8 size;
	f.read((char*)&size, sizeof(size));
	// 读取动作文件
	for (u32 i = 0;i < size;i++)
	{
		u8 len;
		f.read((char*)&len, sizeof(len));
		char str[MAX_PATH]={0};
		f.read(str, len);
		fbFileNames_.push_back(str);
	}

	f.close();

	//
	mAnimationNames.clear();
	for (size_t i = 0; i != fbFileNames_.size(); ++i)
	{
		size_t p = fbFileNames_[i].find_last_of('_');
		size_t q = fbFileNames_[i].find_last_of('.');
		if (p == std::string::npos || ++p == std::string::npos || q == std::string::npos)
		{
			continue;	
		}
		mAnimationNames.push_back(fbFileNames_[i].substr(p, q - p));
		//
		p = fileName.find_last_of('/');
		if (p != std::string::npos)
		{
			std::string fn = fileName.substr(0, p) + "/" + fbFileNames_[i];
			loadFb(fn);
		}
	}
}

void Mz::saveAnimation( const std::string& fileName )
{
	tinyxml2::XMLDocument doc;
	// 
	tinyxml2::XMLDeclaration* dec = doc.NewDeclaration("xml version=\"1.0\"");
	doc.LinkEndChild(dec);
	//
	tinyxml2::XMLElement* ele = doc.NewElement("animations");
	std::string fileFinalName = FileSystem::removeParent(mzFileName_);
	fileFinalName = FileSystem::removeFileExtension(fileFinalName);

	ele->SetAttribute("name", fileFinalName.c_str());
	for (size_t i = 0; i != mAnimations.size(); ++i)
	{
		tinyxml2::XMLElement* a = doc.NewElement("animation");
		a->SetAttribute("name", mAnimations[i].name.c_str());
		a->SetAttribute("duration", mAnimations[i].end);
		std::string skinPath;
		skinPath += mAnimations[i].name;
		skinPath += ".skin";
		a->SetAttribute("skin", skinPath.c_str());
		ele->LinkEndChild(a);
	}
	doc.LinkEndChild(ele);
	//
	doc.SaveFile(fileName.c_str());
}

void addRenderState(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* ele, D3DRENDERSTATETYPE rs, u32 vl)
{
	if(Material::tRenderStateDefault_[rs] != vl)
	{
		tinyxml2::XMLElement* a = doc.NewElement("RenderState");
		a->SetAttribute("state", rs);
		a->SetAttribute("value", vl);
		ele->LinkEndChild(a);
	}
}
void addTextureStageState(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* ele, u32 stage, D3DTEXTURESTAGESTATETYPE tp, u32 vl)
{
	DWORD va = 0;
	if (tp == D3DTSS_COLOROP)
	{
		if (stage == 0)
		{
			va = D3DTOP_MODULATE;
		} 
		else
		{
			va = D3DTOP_DISABLE;
		}
	}
	else if (tp == D3DTSS_ALPHAOP)
	{
		if (stage == 0)
		{
			va = D3DTOP_SELECTARG1;
		} 
		else
		{
			va = D3DTOP_DISABLE;
		}
	}
	else if(tp == D3DTSS_TEXCOORDINDEX)
	{
		va = stage;
	}
	else
	{
		va = Material::tTextureStageStateDefault_[tp];
	}
	if(va != vl)
	{
		tinyxml2::XMLElement* a = doc.NewElement("TextureStageState");
		a->SetAttribute("stage", stage);
		a->SetAttribute("type", tp);
		a->SetAttribute("value", vl);
		ele->LinkEndChild(a);
	}
}
template<class T>
void addKeyFrame(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* ele, const tstring& name, const sKeyFrameSet<T>& kfs)
{
	tinyxml2::XMLElement* a = doc.NewElement(name.c_str());
	for (size_t i = 0; i != kfs.numKeyFrames(); ++i)
	{
		tinyxml2::XMLElement* b = doc.NewElement("KeyFrame");
		b->SetAttribute("time", kfs.getKeyFrame(i)->time);
		b->SetAttribute("value", kfs.getKeyFrame(i)->v);
		a->LinkEndChild(b);
	}
	ele->LinkEndChild(a);
}
void Mz::saveMaterial( const std::string& fileName )
{
	Material::initRenderStateDefault();
	std::string parentPath/* = FileSystem::getParent*/(fileName);
	std::string parentPathOld = FileSystem::getParent(mzFileName_);
	for (size_t i = 0; i != mMaterials.size(); ++i)
	{
		sMat& s = mMaterials[i];
		std::string path = fileName + "/" + s.mName + ".material";
		std::string imagePathOld = parentPathOld + "/" + s.mTextureName;
		std::string imagePathNew = parentPath + "/" + s.mTextureName;
		FileSystem::createFolder(imagePathNew);
		CopyFile(imagePathOld.c_str(), imagePathNew.c_str(), false);
		//============================================================================
		tinyxml2::XMLDocument doc;
		// 
		tinyxml2::XMLDeclaration* dec = doc.NewDeclaration("xml version=\"1.0\"");
		doc.LinkEndChild(dec);
		//
		tinyxml2::XMLElement* ele = doc.NewElement("material");
		ele->SetAttribute("name", s.mName.c_str());
		{
			tinyxml2::XMLElement* a = doc.NewElement("Fx");
			a->SetAttribute("file", "shader/aPTSkin.fx");
			ele->LinkEndChild(a);
		}
		{
			tinyxml2::XMLElement* a = doc.NewElement("Texture");
			a->SetAttribute("name", "g_Texture0");
			std::string fileName;//("image");
			//fileName += "/";
			fileName = s.mTextureName;
			a->SetAttribute("file", fileName.c_str());
			ele->LinkEndChild(a);
		}
		//保存非默认值，首先过滤
		addRenderState(doc, ele, D3DRS_ZENABLE, s.mZEnable);
		addRenderState(doc, ele, D3DRS_ZWRITEENABLE, s.mZWriteEnable);
		addRenderState(doc, ele, D3DRS_ALPHATESTENABLE, s.mAlphaTestEnable);
		if (s.mAlphaTestEnable)
		{
			addRenderState(doc, ele, D3DRS_ALPHAREF, s.mAlphaRef);
			addRenderState(doc, ele, D3DRS_ALPHAFUNC, s.mCmpFunc);
		}
		if (s.mSrcBlend == D3DBLEND_ONE && s.mDestBlend == D3DBLEND_ZERO)
		{
			addRenderState(doc, ele, D3DRS_ALPHABLENDENABLE, false);
		}
		else
		{
			addRenderState(doc, ele, D3DRS_ALPHABLENDENABLE, true);
			addRenderState(doc, ele, D3DRS_SRCBLEND, s.mSrcBlend);
			addRenderState(doc, ele, D3DRS_DESTBLEND, s.mDestBlend);
		}
		//TextureStageState
		if (mVersion >= 30)
		{
			if (mUseVertexColor)
			{
				addTextureStageState(doc, ele, 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
				addTextureStageState(doc, ele, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
				addTextureStageState(doc, ele, 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

				addTextureStageState(doc, ele, 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
				addTextureStageState(doc, ele, 0, D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
				addTextureStageState(doc, ele, 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

				addTextureStageState(doc, ele, 1, D3DTSS_COLOROP, D3DTOP_MODULATE);
				addTextureStageState(doc, ele, 1, D3DTSS_COLORARG1, D3DTA_CURRENT );
				addTextureStageState(doc, ele, 1, D3DTSS_COLORARG2, D3DTA_TFACTOR );

				addTextureStageState(doc, ele, 1, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
				addTextureStageState(doc, ele, 1, D3DTSS_ALPHAARG1,D3DTA_CURRENT );
				addTextureStageState(doc, ele, 1, D3DTSS_ALPHAARG2, D3DTA_TFACTOR );

				addTextureStageState(doc, ele, 1, D3DTSS_TEXCOORDINDEX, 1);		//默认值为1
			}
			else
			{
				addTextureStageState(doc, ele, 0, D3DTSS_ALPHAOP,	D3DTOP_MODULATE);
				addTextureStageState(doc, ele, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
				addTextureStageState(doc, ele, 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

				addTextureStageState(doc, ele, 0, D3DTSS_COLOROP,	D3DTOP_MODULATE);
				addTextureStageState(doc, ele, 0, D3DTSS_COLORARG1,D3DTA_TEXTURE);
				addTextureStageState(doc, ele, 0, D3DTSS_COLORARG2,D3DTA_TFACTOR);
			}
		}
		else
		{
			if (mUseVertexColor)
			{
				addTextureStageState(doc, ele, 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
				addTextureStageState(doc, ele, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
				addTextureStageState(doc, ele, 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

				addTextureStageState(doc, ele, 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
				addTextureStageState(doc, ele, 0, D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
				addTextureStageState(doc, ele, 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

				addTextureStageState(doc, ele, 1, D3DTSS_COLOROP, D3DTOP_MODULATE);
				addTextureStageState(doc, ele, 1, D3DTSS_COLORARG1, D3DTA_CURRENT );
				addTextureStageState(doc, ele, 1, D3DTSS_COLORARG2, D3DTA_TFACTOR );

				addTextureStageState(doc, ele, 1, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
				addTextureStageState(doc, ele, 1, D3DTSS_ALPHAARG1,D3DTA_CURRENT );
				addTextureStageState(doc, ele, 1, D3DTSS_ALPHAARG2, D3DTA_TFACTOR );

				addTextureStageState(doc, ele, 1, D3DTSS_TEXCOORDINDEX, 1);		//默认值为1
			}
			else
			{
				addTextureStageState(doc, ele, 0, D3DTSS_ALPHAOP,	D3DTOP_MODULATE);
				addTextureStageState(doc, ele, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
				addTextureStageState(doc, ele, 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

				addTextureStageState(doc, ele, 0, D3DTSS_COLOROP,	D3DTOP_MODULATE);
				addTextureStageState(doc, ele, 0, D3DTSS_COLORARG1,D3DTA_TEXTURE);
				addTextureStageState(doc, ele, 0, D3DTSS_COLORARG2,D3DTA_TFACTOR);
			}
		}
		// 
		if (s.mFlowU || s.mScale || s.mRotate || s.mRows > 1 || s.mCols > 1)
		{
			addTextureStageState(doc, ele, 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		}
		doc.LinkEndChild(ele);
		//
		addKeyFrame<float>(doc, ele, "Alpha", s.mAlphaKFs);
		addKeyFrame<Vector3>(doc, ele, "Color", s.mColorKFs);
		if (s.mFlowU)
		{
			addKeyFrame<float>(doc, ele, "FlowU", s.mFlowUKFs);
		}
		if (s.mFlowV)
		{
			addKeyFrame<float>(doc, ele, "FlowV", s.mFlowVKFs);
		}
		if (s.mRotate)
		{
			addKeyFrame<float>(doc, ele, "Angle", s.mRotationKFs);
		}
		//addKeyFrame<Vector3>(doc, ele, "Scale", s.mScaleKFs);
		if (s.mRows > 1 || s.mCols > 1)
		{
			tinyxml2::XMLElement* a = doc.NewElement("UVSequence");
			a->SetAttribute("Rows", s.mRows);
			a->SetAttribute("Cols", s.mCols);
			a->SetAttribute("Interval", s.mChangeInterval);
			a->SetAttribute("Style", s.mChangeStyle);
			ele->LinkEndChild(a);
		}

		//
		doc.SaveFile(path.c_str());
	}
}
void Mz::saveSubEntity( const std::string& fileName )
{
	for (size_t i = 0; i != mSubmeshes.size(); ++i)
	{
		sSubMeshWithName& mesh = mSubmeshes[i];
		std::string path = fileName + "/" + mesh.name + ".part";
		//============================================================================
		tinyxml2::XMLDocument doc;
		// 
		tinyxml2::XMLDeclaration* dec = doc.NewDeclaration("xml version=\"1.0\"");
		doc.LinkEndChild(dec);
		//
		tinyxml2::XMLElement* ele = doc.NewElement("part");
		ele->SetAttribute("name", mesh.name);
		{
			tinyxml2::XMLElement* a = doc.NewElement("mesh");
			std::string meshPath;
			//meshPath = "/";
			meshPath += mesh.name;
			meshPath += ".mesh";
			a->SetAttribute("file", meshPath.c_str());
			ele->LinkEndChild(a);
		}
		{
			tinyxml2::XMLElement* a = doc.NewElement("material");
			std::string meshPath;
			//meshPath = "/";
			meshPath += mMaterials[mesh.matId].mName;
			meshPath += ".material";
			a->SetAttribute("file", meshPath.c_str());
			ele->LinkEndChild(a);
		}
		if(0){
			tinyxml2::XMLElement* a = doc.NewElement("boneMapping");
			std::string meshPath;
			//meshPath = "/";
			meshPath += mesh.name;
			meshPath += ".boneMapping";
			a->SetAttribute("file", meshPath.c_str());
			ele->LinkEndChild(a);
		}
		if(0){
			tinyxml2::XMLElement* a = doc.NewElement("skeleton");
			std::string meshPath;
			meshPath = "/";
			std::string fileFinalName = FileSystem::removeParent(mzFileName_);
			fileFinalName = FileSystem::removeFileExtension(fileFinalName);
			meshPath += fileFinalName;
			meshPath += ".skeleton";
			a->SetAttribute("file", meshPath.c_str());
			ele->LinkEndChild(a);
		}
		if(0){
			tinyxml2::XMLElement* a = doc.NewElement("animation");
			std::string meshPath;
			meshPath = "/";
			std::string fileFinalName = FileSystem::removeParent(mzFileName_);
			fileFinalName = FileSystem::removeFileExtension(fileFinalName);
			meshPath += fileFinalName;
			meshPath += ".animation";
			a->SetAttribute("file", meshPath.c_str());
			ele->LinkEndChild(a);
		}
		doc.LinkEndChild(ele);
		//
		doc.SaveFile(path.c_str());
	}
}

void Mz::decodeBone( std::ifstream& f, int s )
{
	u32 nBones;
	f.read((char*)&nBones,sizeof(nBones));
	Skin k;
	for (u32 i = 0;i < nBones;i++)
	{
		sBoneKFs bkf;
		int id;
		f.read((char*)&id,sizeof(id));
		bkf.id = id;
		u8 JointnameLen; 
		f.read((char*)&JointnameLen,sizeof(JointnameLen));
		char name[MODEL_NAME_NODE_SIZE+1];
		f.read((char*)name,JointnameLen);
		name[JointnameLen] = 0;
		int parent;
		f.read((char*)&parent,sizeof(parent));
		Matrix initialMatrix = Matrix::Identity;
		Vector3 pivot;

		if (mVersion < 5)
		{
			f.read((char*)&pivot,sizeof(pivot));
			initialMatrix.setTranslate(pivot);
		}
		else
		{
			//版本号为5以后，导出了初始矩阵
			tranMatrix tm;
			f.read((char*)&tm,sizeof(tm));
			//此tm是D3D矩阵模式，不需要转置
			for (int i = 0;i < 4;i++)
			for (int j = 0;j < 3;j++)
			{
				initialMatrix[i][j] = tm.m_mat[i][j];
			}
			
			pivot.x = tm.m_mat[3][0];
			pivot.y = tm.m_mat[3][1];
			pivot.z = tm.m_mat[3][2];
			//
			initialMatrix = yz2dx(initialMatrix);
		}
		//
		{
			Bone bone;
			bone.id = id;
			bone.parent = parent;
			bone.initialMatrix = initialMatrix;
			bone.name = name;
			mBones.push_back(bone);
		}
		u32 nKeyframes;
		f.read((char*)&nKeyframes,sizeof(nKeyframes));
		for (u32 j = 0;j < nKeyframes;j++)
		{
			sKeyFrame<Vector3> kf;
			f.read((char*)&kf,sizeof(kf));
			kf.v = yz2dx(kf.v);
			bkf.translationKFs.addKeyFrame(kf);
		}
		f.read((char*)&nKeyframes,sizeof(nKeyframes));
		for (u32 j = 0;j < nKeyframes;j++)
		{
			sKeyFrame<Quaternion> kf;
			f.read((char*)&kf,sizeof(kf));
			kf.v = yz2dx(kf.v);
			bkf.rotationKFs.addKeyFrame(kf);
		}
		f.read((char*)&nKeyframes,sizeof(nKeyframes));
		for (u32 j = 0;j < nKeyframes;j++)
		{
			sKeyFrame<Vector3> kf;
			f.read((char*)&kf,sizeof(kf));
			//kf.v = yz2dx(kf.v);
			bkf.scaleKFs.addKeyFrame(kf);
		}
		bool hasRibbonSystem;
		bool hasParticleSystem;
		f.read((char*)&hasRibbonSystem,sizeof(hasRibbonSystem));
		f.read((char*)&hasParticleSystem,sizeof(hasParticleSystem));
		if (hasRibbonSystem)
		{
			decodeRibbon(f, s);
		}
		if (hasParticleSystem)
		{
			decodeParticle(f, s, name);
		}
		k.boneKFs.push_back(bkf);
	}
	if (nBones != 0 && !mLoadFBs && mSkins.empty())
	{
		mSkins.push_back(k);
	}
}

void Mz::decodeRibbon( std::ifstream& f, int s )
{
	bool visible;
	f.read((char*)&visible,sizeof(visible));
	float above;
	f.read((char*)&above,sizeof(above));
	float below;
	f.read((char*)&below,sizeof(below));
	short edgePerSecond;
	f.read((char*)&edgePerSecond,sizeof(edgePerSecond));
	float edgeLife;
	f.read((char*)&edgeLife,sizeof(edgeLife));
	float gravity;
	f.read((char*)&gravity,sizeof(gravity));
	short rows;
	f.read((char*)&rows,sizeof(rows));
	short cols;
	f.read((char*)&cols,sizeof(cols));
	short slot;
	f.read((char*)&slot,sizeof(slot));
	Vector3 color;
	f.read((char*)&color,sizeof(color));
	float alpha;
	f.read((char*)&alpha,sizeof(alpha));
	short blendMode;
	f.read((char*)&blendMode,sizeof(blendMode));
	u8 textureFilenameLen;
	f.read((char*)&textureFilenameLen,sizeof(textureFilenameLen));
	char filename[256];
	f.read((char*)filename,textureFilenameLen);
	filename[textureFilenameLen] = 0;
	//
	u32 nKeyframes;
	//visibility
	f.read((char*)&nKeyframes,sizeof(nKeyframes));
	for (u32 j = 0;j < nKeyframes;j++)
	{
		sKeyFrame<bool> kf;
		f.read((char*)&kf,sizeof(kf));
	}
	//heightAbove
	f.read((char*)&nKeyframes,sizeof(nKeyframes));
	for (u32 j = 0;j < nKeyframes;j++)
	{
		sKeyFrame<float> kf;
		f.read((char*)&kf,sizeof(kf));
	}
	//heightBelow
	f.read((char*)&nKeyframes,sizeof(nKeyframes));
	for (u32 j = 0;j < nKeyframes;j++)
	{
		sKeyFrame<float> kf;
		f.read((char*)&kf,sizeof(kf));
	}
	//textureSlot
	f.read((char*)&nKeyframes,sizeof(nKeyframes));
	for (u32 j = 0;j < nKeyframes;j++)
	{
		sKeyFrame<short> kf;
		f.read((char*)&kf,sizeof(kf));
	}
	//color
	f.read((char*)&nKeyframes,sizeof(nKeyframes));
	for (u32 j = 0;j < nKeyframes;j++)
	{
		sKeyFrame<Vector3> kf;
		f.read((char*)&kf,sizeof(kf));
	}
	//alpha
	f.read((char*)&nKeyframes,sizeof(nKeyframes));
	for (u32 j = 0;j < nKeyframes;j++)
	{
		sKeyFrame<float> kf;
		f.read((char*)&kf,sizeof(kf));
	}
}

void Mz::loadFb( const std::string& fileName )
{
	//
	std::ifstream f(fileName.c_str(), std::ios::binary);
	if (!f.good())
	{
		return;
	}
	mLoadFBs = true;
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
				f.read((char*)&version, sizeof(version));
			}break;
		case 'MANM': // 动画
			{
				u32 t;
				f.read((char*)&t, sizeof(t));
				//
				sAnimation a;
				a.begin = t;
				//
				f.read((char*)&t, sizeof(t));
				//
				a.end = t;
				a.name = mAnimationNames.back();
				//
				mAnimations.push_back(a);
			}break;
		case 'MBON': // 骨骼
			{
				Skin k;
				u32 numBones;
				f.read((char*)&numBones,sizeof(numBones));
				for (u32 i = 0;i < numBones;i++)
				{
					sBoneKFs s;

					int id;
					f.read((char*)&id,sizeof(id));

					//
					s.id = id;

					int parent;
					f.read((char*)&parent,sizeof(parent));

					u32 nKeyframes;
					// translate
					f.read((char*)&nKeyframes,sizeof(nKeyframes));
					for (u32 j = 0;j < nKeyframes;j++)
					{
						sKeyFrame<Vector3> kf;
						f.read((char*)&kf,sizeof(kf));
						kf.v = yz2dx(kf.v);
						s.translationKFs.addKeyFrame(kf);
					}
					// rotate
					f.read((char*)&nKeyframes,sizeof(nKeyframes));
					for (u32 j = 0;j < nKeyframes;j++)
					{
						sKeyFrame<Quaternion> kf;
						f.read((char*)&kf,sizeof(kf));
						kf.v = yz2dx(kf.v);
						s.rotationKFs.addKeyFrame(kf);
					}
					//scale
					f.read((char*)&nKeyframes,sizeof(nKeyframes));
					for (u32 j = 0;j < nKeyframes;j++)
					{
						sKeyFrame<Vector3> kf;
						f.read((char*)&kf,sizeof(kf));
						//kf.v = yz2dx(kf.v);
						s.scaleKFs.addKeyFrame(kf);
					}
					//
					f.ignore(2);
					//
					k.boneKFs.push_back(s);
				}
				mSkins.push_back(k);
			}break;
		default:
			{
				f.ignore(s);
			}break;
		}
	}
}

void Mz::saveSkeleton( const std::string& fileName )
{
	//============================================================================
	// 开始写入数据
	ChunkSet cs;

	//============================================================================
	// 版本号
	cs.beginChunk("MVER");	
	cs.write(&mVersion, sizeof(mVersion));
	cs.endChunk();

	//============================================================================
	// 顶点
	cs.beginChunk("MBON");
	u32 nBones = mBones.size();
	cs.write(&nBones, sizeof(nBones));
	for (size_t i = 0; i != nBones; ++i)
	{
		Bone& b = mBones[i];
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

void Mz::saveSkin( const std::string& fileName )
{
	if (mAnimations.empty())
	{
		return;
	}
	for (size_t i = 0; i != mSkins.size(); ++i)
	{
		std::string path = fileName + "/" + mAnimations[i].name + ".skin";
		Skin& s = mSkins[i];
		//============================================================================
		// 开始写入数据 
		ChunkSet cs;

		//============================================================================
		// 版本号
		cs.beginChunk("MVER");	
		cs.write(&mVersion, sizeof(mVersion));
		cs.endChunk();

		//============================================================================
		// 顶点
		cs.beginChunk("MBON");
		u32 nBones = s.boneKFs.size();
		cs.write(&nBones, sizeof(nBones));
		for (size_t i = 0; i != nBones; ++i)
		{
			sBoneKFs& b = s.boneKFs[i];
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
		cs.save(path);
	}
}

Mesh* Mz::create( size_t sub, Mesh* m )
{
	if (NULL == m)
	{
		return NULL;
	}

	if (!m->createFromMZ(sub, this))
	{
		m->destroy();
		delete m;
		m = NULL;
	}
	return m;
}

void Mz::destroy()
{
	clear();
}

size_t Mz::getSubMeshNumber()
{
	return mSubmeshes.size();
}

std::string Mz::getSubMeshName( size_t sub )
{
	return mSubmeshes[sub].name;
}

void Mz::decodeVersion( std::ifstream& f, int s )
{
	f.read(&mVersion, s);
}

void Mz::decodeSubMesh( std::ifstream& f, int s )
{
	if (mVersion >= 20)
	{
		mSubmeshes.resize(s / sizeof(sSubMesh), sSubMeshWithName());
		size_t i = 0;
		while (s > 0)
		{
			f.read((char*)&mSubmeshes[i], sizeof(sSubMesh));
			f.read(mSubmeshes[i].name, mSubmeshes[i].nameLen);

			s -= (sizeof(sSubMesh) + mSubmeshes[i].nameLen);

			++i;
		}
		mSubmeshes.resize(i);
	} 
	else
	{
		mSubmeshes.resize(s / sizeof(sSubMeshWithName_V3), sSubMeshWithName());
		size_t i = 0;
		sSubMeshWithName_V3 v3;
		while (s > 0)
		{
			f.read((char*)&v3, sizeof(sSubMeshWithName_V3));
			memcpy(&mSubmeshes[i], &v3, sizeof(sSubMesh) - sizeof(u8));
			mSubmeshes[i].nameLen = strlen(v3.name);
			memcpy(mSubmeshes[i].name, v3.name, sizeof(mSubmeshes[i].name));
			s -= sizeof(sSubMeshWithName_V3);

			++i;
		}
	}
	for(int i = 0; i != mSubmeshes.size(); ++i)
	{
		sSubMeshWithName& s = mSubmeshes[i];
		for(int k = 0; k != 128; ++k)
		{
			if(s.name[k] == '|' || s.name[k] == ' ')
			{
				s.name[k] = '_';
			}
		}
	}
}

void Mz::decodeVertices( std::ifstream& f, int s )
{
	bool useVertexColor = false;
	if (mVersion >= 15)
	{
		f.read((char*)&useVertexColor, sizeof(useVertexColor));
		s -= sizeof(useVertexColor);
	}
	if (mVersion >= 19)
	{
		if (useVertexColor)
		{
			mVertices.resize(s / sizeof(sVertex_V3Color));
			size_t i = 0;
			sVertex_V3Color v3;
			while (s > 0)
			{
				f.read((char*)&v3, sizeof(sVertex_V3Color));
				mVertices[i] = v3;
				s -= sizeof(sVertex_V3Color);
				++i;
			}
		}
		else
		{
			mVertices.resize(s / sizeof(sVertex));
			//f.read((char*)&mVertices[0], s);
			size_t i = 0;
			sVertex v3;
			while (s > 0)
			{
				f.read((char*)&v3, sizeof(sVertex));
				mVertices[i] = v3;
				s -= sizeof(sVertex);
				++i;
			}
		}
	}
	else
	{
		mVertices.resize(s / sizeof(sVertex_V3), sVertex_V3Color());
		size_t i = 0;
		sVertex_V3 v3;
		while (s > 0)
		{
			f.read((char*)&v3, sizeof(sVertex_V3));
			mVertices[i] = v3;
			s -= sizeof(sVertex_V3);
			++i;
		}
	}
	mUseVertexColor = useVertexColor;
	for(size_t i = 0; i != mVertices.size(); ++i)
	{
		mVertices[i].pos[2] = -mVertices[i].pos[2];
	}
}

void Mz::decodeFaces( std::ifstream& f, int s )
{
	if (mVersion >= 19)
	{
		mFaces.resize(s / sizeof(Mesh::sFace));
		f.read((char*)&mFaces[0], s);
	}
	else
	{
		mFaces.resize(s / sizeof(sFace_V3));
		size_t i = 0;
		sFace_V3 v3;
		while (s > 0)
		{
			f.read((char*)&v3, sizeof(sFace_V3));
			mFaces[i].index[0] = v3.index[0];
			mFaces[i].index[1] = v3.index[1];
			mFaces[i].index[2] = v3.index[2];
			s -= sizeof(sFace_V3);
			++i;
		}
	}
}

void Mz::decodeMaterial( std::ifstream& f, int s )
{
	u32 numMaterials;
	f.read((char*)&numMaterials, sizeof(numMaterials));
	for (int i = 0; i < numMaterials; ++i)
	{
		sMat mat;
		u8 matNameLen;
		f.read((char*)&matNameLen, sizeof(matNameLen));
		std::string matName(matNameLen + 1, 0);
		//matName.reserve(matNameLen + 1);
		f.read((char*)matName.c_str(), matNameLen);
		//
		mat.mName = matName.c_str();
		// lighting
		u8 lighting;
		f.read((char*)&lighting,sizeof(lighting));
		// ambient diffuse specular emissive
		Vector4 ambient;
		f.read((char*)&ambient,sizeof(ambient));
		//
		Vector4 diffuse;
		f.read((char*)&diffuse,sizeof(diffuse));
		mat.mDiffuse = diffuse;
		//
		Vector4 specular;
		f.read((char*)&specular,sizeof(specular));
		//
		Vector4 emissive;
		f.read((char*)&emissive,sizeof(emissive));
		//
		u8 iTrans = true;
		f.read((char*)&iTrans,sizeof(iTrans));
		//
		u8 twoSide;
		f.read((char*)&twoSide,sizeof(twoSide));
		mat.mCullMode = (D3DCULL)twoSide;
		if (mVersion >= 4)
		{
			//
			u8 tcFlowU;
			f.read((char*)&tcFlowU,sizeof(tcFlowU));
			mat.mFlowU = tcFlowU;
			//
			u8 tcFlowV;
			f.read((char*)&tcFlowV,sizeof(tcFlowV));
			mat.mFlowV = tcFlowV;
			//
			if (mVersion >= 29)
			{
				u8 tcRotate;
				f.read((char*)&tcRotate,sizeof(tcRotate));
				mat.mRotate = tcRotate;
				//
				u8 tcAngleReset;
				f.read((char*)&tcAngleReset,sizeof(tcAngleReset));
				//
				u8 tcPositionReset;
				f.read((char*)&tcPositionReset,sizeof(tcPositionReset));
			}

			// static flow speedU
			float tcFlowSpeedU;
			f.read((char*)&tcFlowSpeedU,sizeof(tcFlowSpeedU));
			//
			u32 nKeyframes;
			if (mVersion >= 26)
			{
				f.read((char*)&nKeyframes,sizeof(nKeyframes));
				if (nKeyframes)
				{
					//mat.mFlowU = true;
				}
				sKeyFrame<float> kf;
				for (size_t i = 0; i != nKeyframes; ++i)
				{
					f.read((char*)&kf, sizeof(kf));
					mat.mFlowUKFs.addKeyFrame(kf);
				}
			}
			if (mat.mFlowUKFs.numKeyFrames() == 0)
			{
				mat.mFlowUKFs.setStaticData(tcFlowSpeedU);
			}

			//
			float tcFlowSpeedV;
			f.read((char*)&tcFlowSpeedV,sizeof(tcFlowSpeedV));
			//
			if (mVersion >= 26)
			{
				f.read((char*)&nKeyframes,sizeof(nKeyframes));
				if (nKeyframes)
				{
					//mat.mFlowV = true;
				}
				sKeyFrame<float> kf;
				for (size_t i = 0; i != nKeyframes; ++i)
				{
					f.read((char*)&kf, sizeof(kf));
					mat.mFlowVKFs.addKeyFrame(kf);
				}
			}
			if (mat.mFlowVKFs.numKeyFrames() == 0)
			{
				mat.mFlowVKFs.setStaticData(tcFlowSpeedU);
			}
			//版本号28加入flow time，纯粹为了与登陆动画兼容
			u16 ft = 0.0f;
			if (mVersion >= 28)
			{
				f.read((char*)&ft,sizeof(ft));
			}

			//版本号29加入RotateSpeed关键桢
			if (mVersion >= 29)
			{
				f.read((char*)&nKeyframes,sizeof(nKeyframes));
				sKeyFrame<float> kf;
				for (size_t i = 0; i != nKeyframes; ++i)
				{
					f.read((char*)&kf, sizeof(kf));
					mat.mRotationKFs.addKeyFrame(kf);
				}
			}
		}

		//版本号9加入材质不投影
		bool dontCastShadow = false;
		if (mVersion >= 9)
		{
			f.read((char*)&dontCastShadow,sizeof(dontCastShadow));
		}

		//版本号12加入不打光材质
		bool unlit = false;
		if (mVersion >= 12)
		{
			f.read((char*)&unlit,sizeof(unlit));
		}

		//版本号14加入深度测试和写入
		bool noDepthCheck = false;
		bool noDepthWrite = false;
		if (mVersion >= 14)
		{
			f.read((char*)&noDepthCheck,sizeof(noDepthCheck));
			f.read((char*)&noDepthWrite,sizeof(noDepthWrite));
		}

		//版本号27加入additiveLayer
		bool additiveLayer = false;
		if(mVersion >= 27)
		{
			f.read((char*)&additiveLayer,sizeof(additiveLayer));
		}
		//版本号7加入透明度
		if (mVersion >= 7)
		{
			float staticTransparency;
			f.read((char*)&staticTransparency,sizeof(staticTransparency));
			u32 nKeyframes;
			f.read((char*)&nKeyframes,sizeof(nKeyframes));
			sKeyFrame<float> kf;
			for (size_t i = 0; i != nKeyframes; ++i)
			{
				f.read((char*)&kf, sizeof(kf));
				mat.mAlphaKFs.addKeyFrame(kf);
				diffuse.w = kf.v;
			}
		}

		//版本号10加入颜色关键桢
		if (mVersion >= 10)
		{
			Vector3 staticColor;
			f.read((char*)&staticColor,sizeof(staticColor));
			u32 nKeyframes;
			f.read((char*)&nKeyframes,sizeof(nKeyframes));
			sKeyFrame<Vector3> kf;
			for (size_t i = 0; i != nKeyframes; ++i)
			{
				f.read((char*)&kf, sizeof(kf));
				mat.mColorKFs.addKeyFrame(kf);
				diffuse.x = kf.v.x;
				diffuse.y = kf.v.y;
				diffuse.z = kf.v.z;
			}
		}
		mat.mDiffuse = diffuse;
		short rows = 1;
		short cols = 1;
		short changeInterval = 30;
		short changeStyle = 0;
		//版本号7加入序列图
		if (mVersion >= 7)
		{
			f.read((char*)&rows,sizeof(rows));
			f.read((char*)&cols,sizeof(cols));
			f.read((char*)&changeStyle,sizeof(changeStyle));
			if(mVersion >= 26)
			{
				u32 nKeyframes;
				f.read((char*)&nKeyframes,sizeof(nKeyframes));
				sKeyFrame<s16> kf;
				for (size_t i = 0; i != nKeyframes; ++i)
				{
					f.read((char*)&kf.time, sizeof(kf.time));
					f.read((char*)&kf.v, sizeof(kf.v));
				}
			}
			f.read((char*)&changeInterval,sizeof(changeInterval));
			mat.mRows = rows;
			mat.mCols = cols;
			mat.mChangeStyle = changeStyle;
			mat.mChangeInterval = changeInterval;
		}
		mat.mAlphaRef = 0;
		//
		u32 nTexture;
		f.read((char*)&nTexture,sizeof(nTexture));
		for (u32 j = 0;j < nTexture;j++)
		{
			u8 filenameLen;
			f.read((char*)&filenameLen,sizeof(filenameLen));
			char str[128];
			f.read((char*)str,filenameLen);
			str[filenameLen] = 0;
			mat.mTextureName = str;
			u8 opType;
			f.read((char*)&opType,sizeof(opType));
			//if (mat.mAlphaTestEnable)
			{
				mat.mCmpFunc = D3DCMP_GREATEREQUAL;
				mat.mAlphaRef = 0xBE;
			}
			switch(opType)
			{
			case 0:
				{
					mat.mSrcBlend = D3DBLEND_ONE;
					mat.mDestBlend = D3DBLEND_ZERO;
					mat.mAlphaTestEnable = false;
					mat.mZEnable = (D3DZBUFFERTYPE)true;
					mat.mZWriteEnable = true;
					break;
				}
			case 1:
				{
					mat.mSrcBlend = D3DBLEND_ONE;
					mat.mDestBlend = D3DBLEND_ZERO;
					mat.mAlphaTestEnable = true;
					mat.mZEnable = (D3DZBUFFERTYPE)true;
					mat.mZWriteEnable = true;
					break;
				}
			case 2:
				{
					mat.mSrcBlend = D3DBLEND_SRCALPHA;
					mat.mDestBlend = D3DBLEND_INVSRCALPHA;
					mat.mAlphaTestEnable = true;
					mat.mZEnable = (D3DZBUFFERTYPE)true;
					mat.mZWriteEnable = false;
					mat.mAlphaRef = 0x0F;
					break;
				}
			case 3:
				{
					mat.mSrcBlend = D3DBLEND_SRCCOLOR;
					mat.mDestBlend = D3DBLEND_ONE;
					mat.mAlphaTestEnable = false;
					mat.mZEnable = (D3DZBUFFERTYPE)true;
					mat.mZWriteEnable = false;
					break;
				}
			case 4:
				{
					mat.mSrcBlend = D3DBLEND_SRCALPHA;
					mat.mDestBlend = D3DBLEND_ONE;
					mat.mAlphaTestEnable = false;
					mat.mZEnable = (D3DZBUFFERTYPE)true;
					mat.mZWriteEnable = false;
					break;
				}
			case 5:
				{
					mat.mSrcBlend = D3DBLEND_DESTCOLOR;
					mat.mDestBlend = D3DBLEND_SRCCOLOR;
					mat.mAlphaTestEnable = false;
					mat.mZEnable = (D3DZBUFFERTYPE)true;
					mat.mZWriteEnable = false;
					break;
				}
			case 6:
				{
					mat.mSrcBlend = D3DBLEND_DESTCOLOR;
					mat.mDestBlend = D3DBLEND_SRCCOLOR;
					mat.mAlphaTestEnable = false;
					mat.mZEnable = (D3DZBUFFERTYPE)true;
					mat.mZWriteEnable = false;
					break;
				}
			}
		}
		if (noDepthCheck)
		{
			mat.mZEnable = (D3DZBUFFERTYPE)0;
		}
		if (noDepthWrite)
		{
			mat.mZWriteEnable = false;
		}
		mMaterials.push_back(mat);
	}
}

void Mz::decodeAnimation( std::ifstream& f, int s )
{
	u32 nAnims; 
	f.read((char*)&nAnims,sizeof(nAnims));
	sAnimation a;
	for (u32 i = 0;i < nAnims;i++)
	{
		u8 AnimnameLen; 
		f.read((char*)&AnimnameLen,sizeof(AnimnameLen));
		char name[64];
		f.read((char*)name,AnimnameLen);
		name[AnimnameLen] = 0;

		u32 startTime,endTime;
		f.read((char*)&startTime,sizeof(startTime));
		f.read((char*)&endTime,sizeof(endTime));
		a.name = name;
		a.begin = startTime;
		a.end = endTime;
		mAnimations.push_back(a);
	}
}

void Mz::saveEntity( const std::string& fileName )
{

}

void Mz::clear()
{
	mLoadFBs = false;
}

void Mz::decodeParticle( std::ifstream& f, int s , char* b)
{
	bool visible;
	f.read((char*)&visible,sizeof(visible));
	float speed;
	f.read((char*)&speed,sizeof(speed));
	float variation;
	f.read((char*)&variation,sizeof(variation));
	float coneAngle;
	f.read((char*)&coneAngle,sizeof(coneAngle));
	float gravity;
	f.read((char*)&gravity,sizeof(gravity));
	float explosiveForce = 0;
	if (mVersion >= 17)
	{
		f.read((char*)&explosiveForce,sizeof(explosiveForce));
	}
	float life;
	f.read((char*)&life,sizeof(life));
	float lifeVar = 0.0f;
	//版本号5之后，生命值加入随机值支持
	if (mVersion >= 5)
	{
		f.read((char*)&lifeVar,sizeof(lifeVar));
	}
	float emissionRate;
	f.read((char*)&emissionRate,sizeof(emissionRate));
	short initialNum = 0;
	if (mVersion >= 17)
	{
		f.read((char*)&initialNum,sizeof(initialNum));
	}
	short limitNum = 32;
	if (mVersion >= 11)
	{
		f.read((char*)&limitNum,sizeof(limitNum));
	}
	bool attachToEmitter = false;
	if (mVersion >= 13)
	{
		f.read((char*)&attachToEmitter,sizeof(attachToEmitter));
	}
	bool moveWithEmitter = false;
	if (mVersion >= 18)
	{
		f.read((char*)&moveWithEmitter,sizeof(moveWithEmitter));
	}
	bool forTheSword = false;
	if(mVersion >= 23)
	{
		f.read((char*)&forTheSword,sizeof(forTheSword));
	}
	float forTheSwordInitialAngle = 0.0f;
	if(mVersion >= 24)
	{
		f.read((char*)&forTheSwordInitialAngle,sizeof(forTheSwordInitialAngle));
	}
	bool wander = false;
	float wanderRadius = 0.0f;
	float wanderSpeed = 0.0f;
	if(mVersion >= 25)
	{
		f.read((char*)&wander,sizeof(wander));
		f.read((char*)&wanderRadius,sizeof(wanderRadius));
		f.read((char*)&wanderSpeed,sizeof(wanderSpeed));
	}
	float aspectRatio = 1.0f;
	//版本号5之后加入宽高比支持
	if (mVersion >= 5)
	{
		f.read((char*)&aspectRatio,sizeof(aspectRatio));
	}
	float angleBegin = 0.0f;
	if (mVersion >= 6)
	{
		f.read((char*)&angleBegin,sizeof(angleBegin));
	}
	float angleEnd = 0.0f;
	if (mVersion >= 6)
	{
		f.read((char*)&angleEnd,sizeof(angleEnd));
	}
	float rotationSpeed = 0.0f;
	if (mVersion >= 6)
	{
		f.read((char*)&rotationSpeed,sizeof(rotationSpeed));
	}
	float rotationSpeedVar = 0.0f;
	if (mVersion >= 15)
	{
		f.read((char*)&rotationSpeedVar,sizeof(rotationSpeedVar));
	}
	float width;
	f.read((char*)&width,sizeof(width));
	float length;
	f.read((char*)&length,sizeof(length));
	float height = 0.0f;
	if (mVersion >= 16)
	{
		f.read((char*)&height,sizeof(height));
	}
	short blendMode;
	f.read((char*)&blendMode,sizeof(blendMode));
	short rows;
	f.read((char*)&rows,sizeof(rows));
	short cols;
	f.read((char*)&cols,sizeof(cols));
	short changeStyle = 0;
	if (mVersion >= 6)
	{
		f.read((char*)&changeStyle,sizeof(changeStyle));
	}
	short changeInterval = 1000;
	if (mVersion >= 6)
	{
		f.read((char*)&changeInterval,sizeof(changeInterval));
	}
	float tailLength;
	f.read((char*)&tailLength,sizeof(tailLength));
	float timeMiddle;
	f.read((char*)&timeMiddle,sizeof(timeMiddle));
	Vector3 colorStart;
	f.read((char*)&colorStart,sizeof(colorStart));
	Vector3 colorMiddle;
	f.read((char*)&colorMiddle,sizeof(colorMiddle));
	Vector3 colorEnd;
	f.read((char*)&colorEnd,sizeof(colorEnd));
	Vector3 alpha;
	f.read((char*)&alpha,sizeof(alpha));
	Vector3 scale;
	f.read((char*)&scale,sizeof(scale));
	Vector3 scaleVar = Vector3(0.0f,0.0f,0.0f);
	//版本号5之后，加入缩放随机值
	if (mVersion >= 5)
	{
		f.read((char*)&scaleVar,sizeof(scaleVar));
	}
	bool fixedSize = false;
	if(mVersion >= 21)
	{
		f.read((char*)&fixedSize,sizeof(fixedSize));
	}
	typedef short short3[3];
	short3 headLifeSpan;
	f.read((char*)&headLifeSpan,sizeof(headLifeSpan));
	short3 headDecay;
	f.read((char*)&headDecay,sizeof(headDecay));
	short3 tailLifeSpan;
	f.read((char*)&tailLifeSpan,sizeof(tailLifeSpan));
	short3 tailDecay;
	f.read((char*)&tailDecay,sizeof(tailDecay));
	bool head,tail,unshaded,unfogged;
	f.read((char*)&head,sizeof(head));
	f.read((char*)&tail,sizeof(tail));
	f.read((char*)&unshaded,sizeof(unshaded));
	f.read((char*)&unfogged,sizeof(unfogged));
	bool blockByY0 = false;
	if(mVersion >= 22)
	{
		f.read((char*)&blockByY0,sizeof(blockByY0));
	}
	u8 textureFilenameLen;
	f.read((char*)&textureFilenameLen,sizeof(textureFilenameLen));
	char filename[256];
	f.read((char*)filename,textureFilenameLen);
	filename[textureFilenameLen] = 0;

	//初始化例子发射器
	ParticleEmitter pe;
	pe.mBoneName = b;
	pe.mTextureFile = "particle/image/";
	pe.mTextureFile += filename;
	pe.mBlendMode = blendMode;
	pe.mInitNumber = initialNum;
	pe.mLimitNumber = limitNum;
	pe.mAttatchEmitter = attachToEmitter;
	pe.mMoveWithEmitter = moveWithEmitter;
	pe.mForSword = forTheSword;
	pe.mSwordInitAngle = forTheSwordInitialAngle;
	pe.mWander = wander;
	pe.mWanderRadius = wanderRadius;
	pe.mWanderSpeed = wanderSpeed;
	//
	pe.mColorStart = colorStart;
	pe.mColorMiddle = colorMiddle;
	pe.mColorEnd = colorEnd;
	//
	pe.mAlpha = alpha;
	pe.mScale = scale;
	pe.mScaleVar = scaleVar;
	//
	pe.mFixedSize = fixedSize;
	pe.mHeadLifeSpan = Vector3(headLifeSpan[0], headLifeSpan[1], headLifeSpan[2]);
	pe.mHeadDecay = Vector3(headDecay[0], headDecay[1], headDecay[2]);
	pe.mTailLifeSpan = Vector3(tailLifeSpan[0], tailLifeSpan[1], tailLifeSpan[2]);
	pe.mTailDecay = Vector3(tailDecay[0], tailDecay[1], tailDecay[2]);
	//
	pe.mRows = rows;
	pe.mCols = cols;
	pe.mChangeStyle = changeStyle;
	pe.mChangeInterval = changeInterval;

	pe.mTime = timeMiddle;
	pe.mLifeSpan = life;
	pe.mLifeVar = lifeVar;
	pe.mTailLength = tailLength;
	pe.mAspectRadio = aspectRatio;
	pe.mInitAngleBegin = angleBegin;
	pe.mInitAngleEnd = angleEnd;
	pe.mRotateSpeed = rotationSpeed;
	pe.mRotateSpeedVar = rotationSpeedVar;

	pe.mHead = head;
	pe.mTail = tail;
	pe.mUnShaded = unshaded;
	pe.mUnFogged = unfogged;
	pe.mBlockedByY0 = blockByY0;
	u32 nKeyframes;
	//visibility
	f.read((char*)&nKeyframes,sizeof(nKeyframes));
	pe.mVisibility.setStaticData(visible);
	for (u32 j = 0;j < nKeyframes;j++)
	{
		sKeyFrame<bool> kf;
		f.read((char*)&kf,sizeof(kf));
		if (nKeyframes = 1 && kf.v == visible)
		{
			break;
		}
		pe.mVisibility.addKeyFrame(kf);
	}
	//speed
	pe.mSpeedKFs.setStaticData(speed);
	f.read((char*)&nKeyframes,sizeof(nKeyframes));
	for (u32 j = 0;j < nKeyframes;j++)
	{
		sKeyFrame<float> kf;
		f.read((char*)&kf,sizeof(kf));
		if (nKeyframes = 1 && kf.v == speed)
		{
			break;
		}
		pe.mSpeedKFs.addKeyFrame(kf);
	}
	//variation
	pe.mVariationKFs.setStaticData(variation);
	f.read((char*)&nKeyframes,sizeof(nKeyframes));
	for (u32 j = 0;j < nKeyframes;j++)
	{
		sKeyFrame<float> kf;
		f.read((char*)&kf,sizeof(kf));
		if (nKeyframes = 1 && kf.v == variation)
		{
			break;
		}
		pe.mVariationKFs.addKeyFrame(kf);
	}
	//latitude
	pe.mLatitude.setStaticData(coneAngle);
	f.read((char*)&nKeyframes,sizeof(nKeyframes));
	for (u32 j = 0;j < nKeyframes;j++)
	{
		sKeyFrame<float> kf;
		f.read((char*)&kf,sizeof(kf));
		if (nKeyframes = 1 && kf.v == coneAngle)
		{
			break;
		}
		pe.mLatitude.addKeyFrame(kf);
	}
	//gravity
	pe.mGravity.setStaticData(gravity);
	f.read((char*)&nKeyframes,sizeof(nKeyframes));
	for (u32 j = 0;j < nKeyframes;j++)
	{
		sKeyFrame<float> kf;
		f.read((char*)&kf,sizeof(kf));
		if (nKeyframes = 1 && kf.v == gravity)
		{
			break;
		}
		pe.mGravity.addKeyFrame(kf);
	}
	if (mVersion >= 17)
	{
		//explosiveForce
		pe.mExplosiveForce.setStaticData(explosiveForce);
		f.read((char*)&nKeyframes,sizeof(nKeyframes));
		for (u32 j = 0;j < nKeyframes;j++)
		{
			sKeyFrame<float> kf;
			f.read((char*)&kf,sizeof(kf));
			if (nKeyframes = 1 && kf.v == explosiveForce)
			{
				break;
			}
			pe.mExplosiveForce.addKeyFrame(kf);
		}
	}
	//emissionRate
	pe.mEmitRate.setStaticData(emissionRate);
	f.read((char*)&nKeyframes,sizeof(nKeyframes));
	for (u32 j = 0;j < nKeyframes;j++)
	{
		sKeyFrame<float> kf;
		f.read((char*)&kf,sizeof(kf));
		if (nKeyframes = 1 && kf.v == emissionRate)
		{
			break;
		}
		pe.mEmitRate.addKeyFrame(kf);
	}
	//width
	pe.mWidth.setStaticData(width);
	f.read((char*)&nKeyframes,sizeof(nKeyframes));
	for (u32 j = 0;j < nKeyframes;j++)
	{
		sKeyFrame<float> kf;
		f.read((char*)&kf,sizeof(kf));
		if (nKeyframes = 1 && kf.v == width)
		{
			break;
		}
		pe.mWidth.addKeyFrame(kf);
	}
	//length
	pe.mLength.setStaticData(length);
	f.read((char*)&nKeyframes,sizeof(nKeyframes));
	for (u32 j = 0;j < nKeyframes;j++)
	{
		sKeyFrame<float> kf;
		f.read((char*)&kf,sizeof(kf));
		if (nKeyframes = 1 && kf.v == length)
		{
			break;
		}
		pe.mLength.addKeyFrame(kf);
	}
	if (mVersion >= 16)
	{
		//height 
		pe.mHeigth.setStaticData(height);
		f.read((char*)&nKeyframes,sizeof(nKeyframes));
		for (u32 j = 0;j < nKeyframes;j++)
		{
			sKeyFrame<float> kf;
			f.read((char*)&kf,sizeof(kf));
			if (nKeyframes = 1 && kf.v == height)
			{
				break;
			}
			pe.mHeigth.addKeyFrame(kf);
		}
	}
	//
	mParticleEmitter.push_back(pe);
}

size_t Mz::getParticleSystemNumber()
{
	return mParticleEmitter.size();
}
