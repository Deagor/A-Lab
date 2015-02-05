#include "stdafx.h"
#include "DrawLevel.h"

static DrawLevel * DrawLevelInstance = 0;

DrawLevel* DrawLevel::getSingleton()
{
	if (DrawLevelInstance != 0) { return DrawLevelInstance; }
	else { DrawLevelInstance = new DrawLevel();  return DrawLevelInstance; }
}

DrawLevel::DrawLevel()
{
	if (!font.loadFromFile("arial.ttf"))
	{
		// error...
	}
}


DrawLevel::~DrawLevel()
{
}


void DrawLevel::CreateShape(int posX,int posY)
{
	sf::CircleShape shape(20);
	shape.setPosition(posX, posY);

	shapes.push_back(shape);
}

void DrawLevel::Draw(Graph<std::pair<std::string,int>,int>* theGraph,sf::RenderWindow& window)
{
	sf::Text text;
	text.setFont(font);

	window.clear();

	auto nodes = theGraph->nodeArray();
	int numNodes = theGraph->getCount();

	for (int i = 0; i < numNodes; i++)
	{
		
		auto iter = nodes[i]->arcList().begin();
		auto enditer = nodes[i]->arcList().end();
		for (; iter != enditer; iter++)
		{
			sf::Vertex vertices[] =
			{
				sf::Vertex(sf::Vector2f(nodes[i]->getX() + 20, nodes[i]->getY() + 20), sf::Color::Red),
				sf::Vertex(sf::Vector2f((*iter).node()->getX() + 20, (*iter).node()->getY() +20),sf::Color::Red)
			};
			window.draw(vertices, 2, sf::Lines);
		}
		window.draw(shapes[i]);
		text.setString(nodes[i]->data().first);
		text.setCharacterSize(15);
		text.setPosition(nodes[i]->getX() + 15, nodes[i]->getY());
		text.setColor(sf::Color::Blue);
		window.draw(text);
	}

	
}