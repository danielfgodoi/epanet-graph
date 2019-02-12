// http://www.cplusplus.com/forum/beginner/132595/

#include <iostream>
#include <fstream>

#include <ctime>
#include <conio.h>
#include <windows.h>

#include "../lib/Graph.hpp"
#include "../include/epanet2.h"

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursorPosition;
string dateTime;

int solveH(int t, string complete)
{
	int		 timestep = t*60,
			 numNodes,
			 numLinks,
			 nodeIndex,
			 linkIndex;
	long	 step,
			 timestamp;
	ofstream listsFile,
			 nodesFile,
			 linksFile;

	listsFile.open(dateTime + "/lists.txt");
	nodesFile.open(dateTime + "/nodes.csv");
	linksFile.open(dateTime + "/links.csv");

	// Get num of nodes and links
	ENgetcount(EN_NODECOUNT, &numNodes);
	ENgetcount(EN_LINKCOUNT, &numLinks);
	cout << "   -> Nodes: " << numNodes << endl;
	cout << "   -> Links: " << numLinks << endl;

	// Set timestep (in seconds) for hydraulic simulation
	ENsettimeparam(EN_HYDSTEP, timestep);

	cout << "-> Solving hydraulic simulation - t=" << t << endl;
	ENopenH();
	ENinitH(0);

	// Create vector of graphs called 'history' to store graphs history
	vector<Graph> history;

	// Files header
	nodesFile << "t;id;pressure;adjacent nodes\n";
	linksFile << "t;id;sourceNode;targetNode;diameter;length;roughness;flow\n";

	do
	{
		// Run simulation for timestamp
		ENrunH(&timestamp);

		// Create graph object to store links and nodes for time t
		Graph graph = Graph();

		// Resize nodes and links vectors
		graph.setNumNodes(numNodes);
		graph.setNumLinks(numLinks);

		// Loop through nodes and set its pressures
		for (nodeIndex = 1; nodeIndex <= numNodes; nodeIndex++)
		{
			char  nodeId[32];
			float pressure;
			ENgetnodeid(nodeIndex, nodeId);
			ENgetnodevalue(nodeIndex, EN_PRESSURE, &pressure);
			graph.setNode(nodeIndex, Node(nodeId, nodeIndex, pressure));
		}

		for (linkIndex = 1; linkIndex <= numLinks; linkIndex++)
		{
			int	  sourceNodeIndex,
				  targetNodeIndex,
				  direction;
			char  linkId[32],
				  sourceNodeId[32],
				  targetNodeId[32];
			float diameter,
				  length,
				  roughness,
				  flow;

			ENgetlinkid(linkIndex, linkId);
			ENgetlinknodes(linkIndex, &sourceNodeIndex, &targetNodeIndex);
			ENgetlinkvalue(linkIndex, EN_DIAMETER, &diameter);
			ENgetlinkvalue(linkIndex, EN_LENGTH, &length);
			ENgetlinkvalue(linkIndex, EN_ROUGHNESS, &roughness);
			ENgetlinkvalue(linkIndex, EN_FLOW, &flow);

			if (flow < 0) direction = -1;
			else direction = 1;

			ENgetnodeid(sourceNodeIndex, sourceNodeId);
			ENgetnodeid(targetNodeIndex, targetNodeId);

			// Set link values pointing the source and targe nodes acconding to the direction
			// also insert the node pointer to the adjacent nodes list
			Link link;
			Node* sourceNode = graph.getNodePointer(sourceNodeIndex);
			Node* targetNode = graph.getNodePointer(targetNodeIndex);

			if (direction == 1)
			{
				link = Link(linkId, linkIndex, sourceNode, targetNode, diameter, length, roughness, flow);
				graph.setLink(linkIndex, link);
				AdjacentNode adjacentNode = AdjacentNode(targetNodeId, sourceNode, &link);
				sourceNode->addAdjacentNode(adjacentNode);
			}

			else if (direction == -1)
			{
				link = Link(linkId, linkIndex, targetNode, sourceNode, diameter, length, roughness, flow);
				graph.setLink(linkIndex, link);
				AdjacentNode adjacentNode = AdjacentNode(sourceNodeId, targetNode, &link);
				targetNode->addAdjacentNode(adjacentNode);
			}
		}

		history.push_back(graph);
		ENnextH(&step);

		string h = "Nodes adjacency lists for t=" + to_string(timestamp/60) + "\n";
		listsFile << h;
		for (int i = 0; i < h.length()-1; ++i)
		{
			listsFile << "=";
		}
		listsFile << "\n";

		for (nodeIndex = 1; nodeIndex <= numNodes; nodeIndex++)
		{
			auto   node = graph.getNode(nodeIndex);

			// nodes.csv
			string pressure = to_string(node.getPressure());
			string s = to_string(timestamp/60) + ";"
					 + node.getId() + ";"
					 + pressure.replace(pressure.find("."), sizeof(",")-1, ",") + ";"
					 + node.getAdjacentNodesCsv();
			s.pop_back();
			s += "\n";
			nodesFile << s;

			// lists.txt
			listsFile << node << "\n";
		}
		
		listsFile << "\n";

		// link.csv
		for (linkIndex = 1; linkIndex <= numLinks; linkIndex++)
		{
			auto   link = graph.getLink(linkIndex);
			string diameter = to_string(link.getDiameter());
			string length = to_string(link.getLength());
			string roughness = to_string(link.getRoughness());
			string flow = to_string(link.getFlow());
			string s = to_string(timestamp/60) + ";"
					 + link.getId() + ";"
					 + link.getSourceNode().getId() + ";"
					 + link.getTargetNode().getId() + ";"
					 + diameter.replace(diameter.find("."), sizeof(",")-1, ",") + ";"
					 + length.replace(length.find("."), sizeof(",")-1, ",") + ";"
					 + roughness.replace(roughness.find("."), sizeof(",")-1, ",") + ";"
					 + flow.replace(flow.find("."), sizeof(",")-1, ",") + "\n";
			linksFile << s;
		}

	} while (step > 0);

	nodesFile.close();
	linksFile.close();
	listsFile.close();

	cout << "-> Simulation completed\n";
	return 1;
}

int solveH(int t)
{
	int 	 timestep = t*60,
			 numNodes,
			 numLinks,
			 nodeIndex,
			 linkIndex;
	long	 step,
			 timestamp;
	ofstream listsFile,
			 nodesFile,
			 linksFile;

	listsFile.open(dateTime + "/lists.txt");
	nodesFile.open(dateTime + "/nodes.csv");
	linksFile.open(dateTime + "/links.csv");

	// Get num of nodes and links
	ENgetcount(EN_NODECOUNT, &numNodes);
	ENgetcount(EN_LINKCOUNT, &numLinks);
	cout << "   -> Nodes: " << numNodes << endl;
	cout << "   -> Links: " << numLinks << endl;

	cout << "-> Solving hydraulic simulation - t=" << t << endl;
	ENopenH();
	ENinitH(0);

	// Loop through all timestamps so we can get the result for the required timestamp
	timestamp = -1;
	while (timestamp != timestep)
	{
		ENrunH(&timestamp);
		ENnextH(&step);
	}

	// Create graph object to store links and nodes for time t
	Graph graph = Graph();

	// Resize nodes and links vectors
	graph.setNumNodes(numNodes);
	graph.setNumLinks(numLinks);

	// Loop through nodes and set its pressures
	for (nodeIndex = 1; nodeIndex <= numNodes; nodeIndex++)
	{
		char  nodeId[32];
		float pressure;
		ENgetnodeid(nodeIndex, nodeId);
		ENgetnodevalue(nodeIndex, EN_PRESSURE, &pressure);
		graph.setNode(nodeIndex, Node(nodeId, nodeIndex, pressure));
	}

	for (linkIndex = 1; linkIndex <= numLinks; linkIndex++)
	{
		int	  sourceNodeIndex,
			  targetNodeIndex,
			  direction;
		char  linkId[32],
			  sourceNodeId[32],
			  targetNodeId[32];
		float diameter,
			  length,
			  roughness,
			  flow;

		ENgetlinkid(linkIndex, linkId);
		ENgetlinknodes(linkIndex, &sourceNodeIndex, &targetNodeIndex);
		ENgetlinkvalue(linkIndex, EN_DIAMETER, &diameter);
		ENgetlinkvalue(linkIndex, EN_LENGTH, &length);
		ENgetlinkvalue(linkIndex, EN_ROUGHNESS, &roughness);
		ENgetlinkvalue(linkIndex, EN_FLOW, &flow);

		if (flow < 0) direction = -1;
		else direction = 1;

		ENgetnodeid(sourceNodeIndex, sourceNodeId);
		ENgetnodeid(targetNodeIndex, targetNodeId);

		// Set link values pointing the source and targe nodes acconding to the direction
		// also insert the node pointer to the adjacent nodes list
		Link link;
		Node* sourceNode = graph.getNodePointer(sourceNodeIndex);
		Node* targetNode = graph.getNodePointer(targetNodeIndex);

		if (direction == 1)
		{
			link = Link(linkId, linkIndex, sourceNode, targetNode, diameter, length, roughness, flow);
			graph.setLink(linkIndex, link);
			AdjacentNode adjacentNode = AdjacentNode(targetNodeId, sourceNode, &link);
			sourceNode->addAdjacentNode(adjacentNode);
		}

		else if (direction == -1)
		{
			link = Link(linkId, linkIndex, targetNode, sourceNode, diameter, length, roughness, flow);
			graph.setLink(linkIndex, link);
			AdjacentNode adjacentNode = AdjacentNode(sourceNodeId, targetNode, &link);
			targetNode->addAdjacentNode(adjacentNode);
		}
	}

	string h = "Nodes adjacency lists for t=" + to_string(t) + "\n";
	listsFile << h;
	for (int i = 0; i < h.length()-1; ++i)
	{
		listsFile << "=";
	}
	listsFile << "\n";

	nodesFile << "t;id;pressure;adjacent nodes\n";
	for (nodeIndex = 1; nodeIndex <= numNodes; nodeIndex++)
	{
		auto   node = graph.getNode(nodeIndex);
		
		// nodes.csv
		string pressure = to_string(node.getPressure());
		string s = to_string(t) + ";"
				 + node.getId() + ";"
				 + pressure.replace(pressure.find("."), sizeof(",")-1, ",") + ";"
				 + node.getAdjacentNodesCsv();
		s.pop_back();
		s += "\n";
		nodesFile << s;

		// lists.txt
		listsFile << node << "\n";
	}
	listsFile.close();
	nodesFile.close();

	// link.csv
	linksFile << "t;id;sourceNode;targetNode;diameter;length;roughness;flow\n";
	for (linkIndex = 1; linkIndex <= numLinks; linkIndex++)
	{
		auto   link = graph.getLink(linkIndex);
		string diameter = to_string(link.getDiameter());
		string length = to_string(link.getLength());
		string roughness = to_string(link.getRoughness());
		string flow = to_string(link.getFlow());
		string s = to_string(t) + ";"
				 + link.getId() + ";"
				 + link.getSourceNode().getId() + ";"
				 + link.getTargetNode().getId() + ";"
				 + diameter.replace(diameter.find("."), sizeof(",")-1, ",") + ";"
				 + length.replace(length.find("."), sizeof(",")-1, ",") + ";"
				 + roughness.replace(roughness.find("."), sizeof(",")-1, ",") + ";"
				 + flow.replace(flow.find("."), sizeof(",")-1, ",") + "\n";
		linksFile << s;
	}
	linksFile.close();

	cout << "-> Simulation completed\n";
	return 1;
}

void gotoCoord(int x, int y)
{
	cursorPosition.X = x;
	cursorPosition.Y = y;
	SetConsoleCursorPosition(console, cursorPosition);
}

COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
	{
		return cbsi.dwCursorPosition;
	}
	else
	{
		// The function failed. Call GetLastError() for details.
		COORD invalid = { 0, 0 };
		return invalid;
	}
}

int getMenu()
{
	COORD currentCursorPosition = GetConsoleCursorPosition(console);

	bool running = true;
	int  y = currentCursorPosition.Y + 1,
		 minY = y,
		 maxY = y+2,
		 menuItem = 0;

	cout << "-> Please, choose an option below (use up and down arrows)\n";
	cout << "->";
	while (running)
	{
		gotoCoord(3, minY);		cout << "1. Run complete simulation";
		gotoCoord(3, minY+1);	cout << "2. Run simulation for specific time";
		gotoCoord(3, maxY);		cout << "3. Quit\n";
		
		system("pause>nul");

		if (GetAsyncKeyState(VK_DOWN) && y != maxY)  // down key
		{
			gotoCoord(0, y);
			cout << "   ";
			++y;
			gotoCoord(0, y);
			cout << "-> ";
			++menuItem;
			continue;
		}

		if (GetAsyncKeyState(VK_UP) && y != minY) // up key
		{
			gotoCoord(0, y);
			cout << "   ";
			--y;
			gotoCoord(0, y);
			cout << "-> ";
			--menuItem;
			continue;
		}

		if (GetAsyncKeyState(VK_RETURN) & 0x8000) // enter key
		{
			running = false;
		}
	}

	return menuItem;
}

int main(int argc, char const *argv[])
{
	int	   errorCode;
	string inpFile,
		   rptFile;
	bool   quit = false,
		   opened = true;
	time_t rawtime;
	tm*    timeinfo;
	char   st[16];

	system("cls");
	while (!quit)
	{
		cout << "   EPANET Graph Generator\n";
		cout << "===============================================================================\n";

		// Get menu option
		opened = true;
		int menu = getMenu();
		if (menu == 2)
		{
			cout << "-> Closing application\n";
			return 0;
		}

		// Get date and time and create dir
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(st, 80, "%Y%m%dT%H%M%S", timeinfo);
		dateTime = st;
		CreateDirectory((char*) dateTime.c_str(), NULL);

		// Get .inp and .rpt file names
		cout << "-> Specify .inp file: ";
		cin >> inpFile;

		cout << "-> Specify .rpt file: ";
		cin >> rptFile;

		// Check valid file names
		if (inpFile.length() >= 4)
		{
			if (inpFile.substr(inpFile.length() - 4, 4) != ".inp") inpFile += ".inp";
		} else inpFile += ".inp";
		if (rptFile.length() >= 4)
		{
			if (rptFile.substr(rptFile.length() - 4, 4) != ".rpt") rptFile += ".rpt";
		} else rptFile += ".rpt";

		// Start toolkit
		cout << "-> Starting toolkit\n";
		errorCode = ENopen((char*) inpFile.c_str(), (char*) rptFile.c_str(), (char *) "");
		if (errorCode > 0)
		{
			cout << "-> .inp file opened with error code " << errorCode << endl;
			cout << "-> Closing toolkit\n\n";
			opened = false;
			ENclose();
			cout << "Press any key to continue";
			_getch();
			system("cls");
		}
		
		// Run methods to solve hydraulic simulation
		if (opened)
		{
			int success = 0;
			cout << "-> .inp file opened successfully\n";
			switch (menu)
			{
				case 0:
				{
					int t = -1;
					while (t == -1)
					{
						cout << "-> Inform timestep in minutes: ";
						cin >> t;
						if (t <= 0)
						{
							t = -1;
							cout << "   -> Timestep must be greater than zero\n";
						}
					}					
					success = solveH(t, "complete");
					break;
				}

				case 1:
				{
					int t = -1;
					cout << "-> Inform time in minutes: ";
					cin >> t;
					success = solveH(t);
					break;
				}
			}

			if (success)
			{
				// cout << "\n-> Lists file exported to '" << dateTime << "/lists.txt'\n";
				cout << "-> Nodes file exported to '" << dateTime << "/nodes.csv'\n";
				cout << "-> Link file exported to '" << dateTime << "/links.csv'\n\n";
			}

			cout << "-> Closing toolkit\n\n";
			ENclose();
			cout << "Press any key to continue";
			_getch();
			system("cls");
		}
	}
	
	return 0;
}
