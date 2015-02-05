#ifndef DRAWLEVEL_H
#define DRAWLEVEL_H

#include "Graph.h"

class DrawLevel
{
public:
	
	static DrawLevel* getSingleton();

	void CreateShape(int posX,int posY);

	
	void Draw(Graph<std::pair<std::string,int>,int> * theGraph,sf::RenderWindow& window);

private:
	
	DrawLevel();
	~DrawLevel();
	vector<sf::CircleShape> shapes;
	sf::Font font;
};

#endif

