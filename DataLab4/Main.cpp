#include "stdafx.h"
#include <iostream>
#include <fstream>

#include "Graph.h"
#include <utility>
#include <string>

#ifdef _DEBUG
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#else
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-audio.lib")
#pragma comment(lib,"sfml-system.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-network.lib")
#endif


using std::pair;

using namespace std;


//typedef GraphArc<pair<string,int>, int> Arc;//<char,int> info at each arc and weight info 
typedef GraphNode<pair<string, int>, int> Node;

void visit( Node * pNode ) {
	cout << "Visiting: " << pNode->data().first << endl;
}

int main(int argc, char *argv[])
{
	
	Graph<pair<string, int>, int> graph(32);//create the graph(9=number of vertices)

	sf::RenderWindow window(sf::VideoMode(800, 600), "Window");
	sf::RenderWindow* pwindow = &window;

	string c;
	int posX, posY;
	int i = 0;
	ifstream myfile;
	myfile.open("townNodes.txt");

	while (myfile >> c >> posX >> posY)
	{
		graph.addNode(make_pair(c, INT_MAX), i++, posX, posY);
	}
	myfile.close();
	myfile.open("townArcs.txt");

	int from, to;
	int weight;
	while (myfile >> from >> to >> weight)
	{

		/*ifrom = (from <= '9') ? from - '0' : from - 'a';
		ito = (to <= '9') ? to - '0' : to - 'a';*/

		/*ifrom = from.at(0) - 'A';
		ito = to.at(0) - 'A';*/
		graph.addArc(from, to, weight);
	}
	myfile.close();
	//graph.test();

	std::vector<Node*> path;

	graph.aStar(graph.nodeArray()[0], graph.nodeArray()[20], visit, path);
	graph.test();
	/*for (int i = path.size() - 1; i >= 0; i--)
	{
	cout << path.at(i)->data().first << endl;
	}
	Node *final = graph.nodeArray()[5];*/

	while (window.isOpen())
	{
		// Process events
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			// Close window :: Exit
			if (Event.type == sf::Event::Closed)
			{
				window.close();
			}
			// Escape key :
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
				window.close();
		}
		DrawLevel::getSingleton()->Draw(&graph,*pwindow );

		window.display();
	}
}