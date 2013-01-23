#include "Skin.h"

Skin::Skin()
{

}

bool Skin::create( const std::string& fileName )
{
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
				f.read((char*)&mVersion, sizeof(mVersion));
			}break;
		case 'MBON': // นว๗ภ
			{
				u32 numBones;
				f.read((char*)&numBones,sizeof(numBones));
				for (u32 i = 0;i < numBones;i++)
				{
					sBoneKFs s;
					u32 nKeyframes;
					// translate
					f.read((char*)&nKeyframes,sizeof(nKeyframes));
					for (u32 j = 0;j < nKeyframes;j++)
					{
						sKeyFrame<Vector3> kf;
						f.read((char*)&kf,sizeof(kf));
						s.translationKFs.addKeyFrame(kf);
					}
					// rotate
					f.read((char*)&nKeyframes,sizeof(nKeyframes));
					for (u32 j = 0;j < nKeyframes;j++)
					{
						sKeyFrame<Quaternion> kf;
						f.read((char*)&kf,sizeof(kf));
						s.rotationKFs.addKeyFrame(kf);
					}
					//scale
					f.read((char*)&nKeyframes,sizeof(nKeyframes));
					for (u32 j = 0;j < nKeyframes;j++)
					{
						sKeyFrame<Vector3> kf;
						f.read((char*)&kf,sizeof(kf));
						s.scaleKFs.addKeyFrame(kf);
					}
					//
					boneKFs.push_back(s);
				}
			}break;
		default:
			{
				f.ignore(s);
			}break;
		}
	}

	return true;
}

void Skin::_clear()
{
	boneKFs.clear();
}

void Skin::destroy()
{

}

sBoneKFs::sBoneKFs()
{
	_clear();
}

void sBoneKFs::_clear()
{
	translationKFs.setStaticData(Vector3(0.0f, 0.0f, 0.0f));
	rotationKFs.setStaticData(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
	scaleKFs.setStaticData(Vector3(1.0f, 1.0f, 1.0f));

	//
	translationKFs.setInterpolationType(eInterpolationType_Bezier_Linear);
	rotationKFs.setInterpolationType(eInterpolationType_Bezier_Linear);
	scaleKFs.setInterpolationType(eInterpolationType_Bezier_Linear);
}
