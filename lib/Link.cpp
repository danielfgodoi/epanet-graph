#include "Link.hpp"

Link::Link()
{
	// default
}

Link::Link(string id, int index, Node* sourceNode, Node* targetNode, float diameter, float length, float roughness, float flow)
{
	this->id = id;
	this->index = index;
	this->sourceNode = sourceNode;
	this->targetNode = targetNode;
	this->diameter = diameter;
	this->length = length;
	this->roughness = roughness;
	this->flow = flow;
}

Link::~Link()
{
	// default
}

ostream&
operator<<(ostream &os, const Link &link)
{
	os << link.id << " ( " << link.sourceNode->getId() << " -> " << link.targetNode->getId() << " )";
	return os;
}

string
Link::getId() const
{
	return this->id;
}

void
Link::setId(string id)
{
	this->id = id;
}

int
Link::getIndex() const
{
	return this->index;
}

void
Link::setIndex(int index)
{
	this->index = index;
}

Node
Link::getSourceNode() const
{
	return *this->sourceNode;
}

Node*
Link::getSourceNodePointer()
{
	return this->sourceNode;
}

void
Link::setSourceNode(Node* sourceNode)
{
	this->sourceNode = sourceNode;
}

Node
Link::getTargetNode() const
{
	return *this->targetNode;
}

Node*
Link::getTargetNodePointer()
{
	return this->targetNode;
}

void
Link::setTargetNode(Node* targetNode)
{
	this->targetNode = targetNode;
}

float
Link::getDiameter() const
{
	return this->diameter;
}

void
Link::setDiameter(float diameter)
{
	this->diameter = diameter;
}

float
Link::getLength() const
{
	return this->length;
}

void
Link::setLength(float length)
{
	this->length = length;
}

float
Link::getRoughness() const
{
	return this->roughness;
}

void
Link::setRoughness(float roughness)
{
	this->roughness = roughness;
}

float
Link::getFlow() const
{
	return this->flow;
}

void
Link::setFlow(float flow)
{
	this->flow = flow;
}
