#include "Graph.hpp"

Graph::Graph()
{
	
}

Graph::~Graph() {}

ostream&
operator<<(ostream &os, const Graph &graph)
{
	os << "Graph\n========================\n" << "nodes: " << graph.getNodes().size() << "\nlinks: " << graph.getLinks().size() << endl << endl;
	os << "Nodes\n========================\n";
	for (auto& i : graph.getNodes()) {
		os << i << endl;
	}
	os << "\nLinks\n========================\n";
	for (auto& i : graph.getLinks()) {
		os << i << endl;
	}
	return os;
}

vector<Node>
Graph::getNodes() const
{
	return this->nodes;
}

vector<Link>
Graph::getLinks() const
{
	return this->links;
}

int
Graph::getNumNodes() const
{
	return this->nodes.size();
}

void
Graph::setNumNodes(int size)
{
	this->nodes.resize(size);
}

int
Graph::getNumLinks() const
{
	return this->links.size();
}

void
Graph::setNumLinks(int size)
{
	this->links.resize(size);
}

void
Graph::setNode(int index, Node node)
{
	this->nodes[index-1] = node;
}

Node
Graph::getNode(int index) const
{
	return this->nodes[index-1];
}

Node*
Graph::getNodePointer(int index)
{
	return &(this->nodes[index-1]);
}

void
Graph::setLink(int index, Link link)
{
	this->links[index-1] = link;
}

Link
Graph::getLink(int index) const
{
	return this->links[index-1];
}

Link*
Graph::getLinkPointer(int index)
{
	return &(this->links[index-1]);
}
