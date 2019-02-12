#include "AdjacentNode.hpp"
#include "Node.hpp"
#include "Link.hpp"

AdjacentNode::AdjacentNode()
{
	// default
}

AdjacentNode::AdjacentNode(string id, Node* sourceNode, Link* sourceLink)
{
	this->id = id;
	this->sourceNode = sourceNode;
	this->sourceLink = sourceLink;
}

AdjacentNode::~AdjacentNode()
{
	// default
}

ostream&
operator<<(ostream &os, const AdjacentNode &adjacentNode)
{
	os << adjacentNode.id << " - " << *adjacentNode.sourceNode << " - " << *adjacentNode.sourceLink;
	return os;
}

string
AdjacentNode::getId() const
{
	return this->id;
}

void
AdjacentNode::setId(string id)
{
	this->id = id;
}

Node*
AdjacentNode::getSourceNode()
{
	return this->sourceNode;
}

void
AdjacentNode::setSourceNode(Node* sourceNode)
{
	this->sourceNode = sourceNode;
}

Link*
AdjacentNode::getSourceLink()
{
	return this->sourceLink;
}

void
AdjacentNode::setSourceLink(Link* sourceLink)
{
	this->sourceLink = sourceLink;
}
