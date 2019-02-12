#ifndef LINK_H
#define LINK_H

#include <iostream>
#include "Node.hpp"

using namespace std;

class Link
{
public:
	Link();
	Link(string id, int index, Node* sourceNode, Node* targetNode, float diameter, float length, float roughness, float flow);
	~Link();

	friend ostream& operator<<(ostream &os, const Link &link);

	string getId() const;
	void   setId(string id);
	int    getIndex() const;
	void   setIndex(int index);
	Node   getSourceNode() const;
	Node*  getSourceNodePointer();
	void   setSourceNode(Node* sourceNode);
	Node   getTargetNode() const;
	Node*  getTargetNodePointer();
	void   setTargetNode(Node* targetNode);
	float  getDiameter() const;
	void   setDiameter(float diameter);
	float  getLength() const;
	void   setLength(float length);
	float  getRoughness() const;
	void   setRoughness(float roughness);
	float  getFlow() const;
	void   setFlow(float flow);

private: 
	string id;
	int    index;
	Node*  sourceNode;
	Node*  targetNode;
	float  diameter;
	float  length;
	float  roughness;
	float  flow;
};

#endif
