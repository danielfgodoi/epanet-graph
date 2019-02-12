#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <list>

#include "AdjacentNode.hpp"

using namespace std;

class Node
{
public:
	Node();
	Node(string id, int index, float pressure);
	~Node();

	friend ostream& operator<<(ostream &os, const Node &node);

	string             getId() const;
	void               setId(string id);
	int	               getIndex() const;
	void               setIndex(int index);
	float              getPressure() const;
	void               setPressure(float pressure);
	list<AdjacentNode> getAdjacentNodes() const;
	string			   getAdjacentNodesCsv() const;
	void               addAdjacentNode(AdjacentNode adjacentNode);

private:
	string 			   id;
	int 			   index;
	float			   pressure;
	list<AdjacentNode> adjacentNodes;
};

#endif
