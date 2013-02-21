#pragma once
#include "ConfigScene.h"
#include "misc/stdHead.h"
#include "misc/Singleton.h"
class EntityInstance;
class ApiScene_ Node
{
public:
	Node();
	~Node();
public:
	void release();
//	bool load(const std::string& resID);
//	void addChild();
	void attach(EntityInstance* e);
	void setName(const std::string& name);
	void render();
	void update(float delta);
private:
	typedef std::map<std::string, Node*> NameNodeMap;
	NameNodeMap Children_;
	EntityInstance* EntityInstance_;
	std::string Name_;
};

class ApiScene_ NodeManager : public Singleton<NodeManager>
{
public:
	Node* createNode(const std::string& name);
};