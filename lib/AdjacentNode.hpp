#ifndef ADJACENTNODE_H
#define ADJACENTNODE_H

#include <iostream>
#include <list>

using namespace std;

// using forward declaration to avoid cyclic dependencies 
class Node;
class Link;

class AdjacentNode
{
public:
	AdjacentNode();
	AdjacentNode(string id, Node* sourceNode, Link* sourceLink);
	~AdjacentNode();

	friend ostream& operator<<(ostream &os, const AdjacentNode &adjacentNode);

	string getId() const;
	void   setId(string id);
	Node*  getSourceNode();
	void   setSourceNode(Node* sourceNode);
	Link*  getSourceLink();
	void   setSourceLink(Link* sourceLink);

private:
	string id;
	Node*  sourceNode;
	Link*  sourceLink;
};

#endif
