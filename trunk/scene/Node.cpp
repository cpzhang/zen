#include "Node.h"
#include "model/EntityInstance.h"
Node::Node()
{
	EntityInstance_ = NULL;
}

Node::~Node()
{

}

void Node::release()
{
	delete this;
}

// bool Node::load(const std::string& resID)
// {
// 	EntityInstance_ = new EntityInstance;
// 	return EntityInstance_->create(resID);
// }

// void Node::addChild()
// {
// 
// }

void Node::attach( EntityInstance* e )
{
	EntityInstance_ = e;
}

void Node::setName( const std::string& name )
{
	Name_ = name;
}

void Node::render()
{
	if (EntityInstance_)
	{
		EntityInstance_->render();
	}
}

void Node::update( float delta )
{
	if (EntityInstance_)
	{
		EntityInstance_->update(delta);
	}
}

EntityInstance* Node::getEntityInstance()
{
	return EntityInstance_;
}

Node* NodeManager::createNode( const std::string& name)
{
	Node* n = new Node;
	n->setName(name);
	return n;
}
