
#include <iostream>
#include <fstream>

#include "Graph.h"
#include <utility>
#include <string>
using std::pair;

using namespace std;


typedef GraphArc<pair<string,int>, int> Arc;//<char,int> info at each arc and weight info 
typedef GraphNode<pair<string, int>, int> Node;

void visit( Node * pNode ) {
	cout << "Visiting: " << pNode->data().first << " " << pNode->data().second << endl;
}

int main(int argc, char *argv[]) {
   
	Graph<pair<string, int>, int> graph(6);//create the graph(9=number of vertices) 
	
	string c; 
	int i = 0;
	ifstream myfile;
	myfile.open ("townNodes.txt");

	while ( myfile >> c ) {
		graph.addNode(make_pair(c,INT_MAX),i++);
	}

	myfile.close();
	myfile.open("townArcs.txt");

	string from, to;
	int weight;
	int ifrom, ito;
	while ( myfile >> from >> to >> weight ) {
		ifrom = from.at(0) - 'A';
		ito = to.at(0) - 'A';
		graph.addArc(ifrom, ito, weight);
	}
    myfile.close();


	std::vector<Node*> path;
	graph.UCS(graph.nodeArray()[0], graph.nodeArray()[5], visit, path);

	for (int i = path.size() - 1; i >= 0; i--)
	{
		cout << path.at(i)->data().first << endl;
	}
	Node *final = graph.nodeArray()[5];
	system("PAUSE");
	
}
