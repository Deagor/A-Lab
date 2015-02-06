#ifndef DRAWLEVEL_H
#define DRAWLEVEL_H

#include "Graph.h"

class DrawLevel
{
public:
	
	static DrawLevel* getSingleton();

	void CreateShape(int posX,int posY);
	std::tuple<int,int,int> CheckMouseClicks(sf::Event theEvent,sf::RenderWindow& window);
	void DrawLevel::ResetShapes();
	
	void Draw(Graph<std::pair<std::string,int>,int> * theGraph,sf::RenderWindow& window);

private:
	
	DrawLevel();
	~DrawLevel();
	vector<sf::CircleShape> shapes;
	sf::Font font;
	sf::Texture srtTex, resetTex;
	sf::Sprite srtBtn, resetBtn;

	sf::Mouse mouse;
	int start = -1, end = -1;
	bool lMousePrevClicked, rMousePrevClicked;
	bool hasRun;
};

#endif

