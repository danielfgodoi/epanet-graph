#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>

#include "Node.hpp"
#include "Link.hpp"

using namespace std;

class Graph
{
public:
	Graph();
	~Graph();

	friend ostream& operator<<(ostream &os, const Graph &graph);

	vector<Node> getNodes() const;
	vector<Link> getLinks() const;
	int          getNumNodes() const;
	void         setNumNodes(int size);
	int          getNumLinks() const;
	void         setNumLinks(int size);
	void         setNode(int index, Node node);
	Node         getNode(int index) const;
	Node*        getNodePointer(int index);
	void         setLink(int index, Link link);
	Link         getLink(int index) const;
	Link*        getLinkPointer(int index);

private:
	vector<Node> nodes;
	vector<Link> links;
};

#endif
