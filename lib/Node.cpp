#include "Node.hpp"

Node::Node()
{
	// default
}

Node::Node(string id, int index, float pressure)
{
	this->id = id;
	this->index = index;
	this->pressure = pressure;
}

Node::~Node()
{
	// default
}

ostream&
operator<<(ostream &os, const Node &node)
{
	os << node.id << " -> [ ";
	for (auto& i : node.getAdjacentNodes()) {
		os << i.getId() << " ";
	}
	os << "]";
	return os;
}

string
Node::getId() const
{
	return this->id;
}

void
Node::setId(string id)
{
	this->id = id;
}

int
Node::getIndex() const
{
	return this->index;
}

void
Node::setIndex(int index)
{
	this->index = index;
}

float
Node::getPressure() const
{
	return this->pressure;
}

void
Node::setPressure(float pressure)
{
	this->pressure = pressure;
}

list<AdjacentNode>
Node::getAdjacentNodes() const
{
	return this->adjacentNodes;
}

string
Node::getAdjacentNodesCsv() const
{
	string nodesCsv = "";
	for (auto& i : this->getAdjacentNodes()) {
		nodesCsv += i.getId() + ";";
	}
	return nodesCsv;
}

void
Node::addAdjacentNode(AdjacentNode adjacentNode)
{
	this->adjacentNodes.push_back(adjacentNode);
}
