#pragma once
#include "render/KeyFrames.h"
#include "Particle.h"
#include "ConfigModel.h"
class IMaterial;
class Texture;
class ParticleEmitter;
class ApiModel_ ParticleCluster
{
public:
	ParticleCluster();
	~ParticleCluster();
public:
	void render();
	void update(float delta);
	bool create(ParticleEmitter* e);
	ParticleEmitter* getEmitter()
	{
		return mEmitter;
	}
	void destroy();
private:
	void _clear();
private:
	ParticleList mParticles;
	ParticleEmitter* mEmitter;
	Texture* mTexture;
	IMaterial* mMaterial;
	Matrix mPVW;
	AnimationTime mAT;
};

typedef std::vector<ParticleCluster> ParticleClusterVec;