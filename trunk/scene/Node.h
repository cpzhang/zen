#pragma once
#include "misc/stdHead.h"
class Node
{
public:
	Node();
	~Node();
public:
	void addChild();
private:
	typedef std::map<std::string, Node*> NameNodeMap;
	NameNodeMap Children_;
};

class NodeManager : public Singleton<NodeManager>
{
public:
	Node* createNode()
	{
		Node* n = new Node;
		return n;
	}
};