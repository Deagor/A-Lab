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
	srtTex.loadFromFile("startButton.png");
	srtBtn.setTexture(srtTex);
	srtBtn.setPosition(20, 550);
	srtBtn.setScale(.5f, .5f);

	resetTex.loadFromFile("resetButton.png");
	resetBtn.setTexture(resetTex);
	resetBtn.setPosition(650, 550);
	resetBtn.setScale(.8f, .8f);

	lMousePrevClicked = false;
	rMousePrevClicked = false;
}


DrawLevel::~DrawLevel()
{
}


void DrawLevel::CreateShape(int posX,int posY)
{
	sf::CircleShape shape(20);
	shape.setPosition(posX, posY);
	shape.setFillColor(sf::Color::Green);

	shapes.push_back(shape);
}

void DrawLevel::Draw(Graph<std::pair<std::string,int>,int>* theGraph,sf::RenderWindow& window)
{
	sf::Text text;
	text.setFont(font);

	window.clear();

	/*GraphNode<std::pair<std::string,int>,int>*/auto nodes = theGraph->nodeArray();
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
		if (shapes[i].getFillColor() != sf::Color::White && nodes[i]->marked() && !nodes[i]->isInPath()) { shapes[i].setFillColor(sf::Color::White); }
		else if (shapes[i].getFillColor() != sf::Color::Yellow && nodes[i]->marked() && nodes[i]->isInPath()) { shapes[i].setFillColor(sf::Color::Yellow); }
		else if (shapes[i].getFillColor() != sf::Color::Green && !nodes[i]->marked()) { shapes[i].setFillColor(sf::Color::Green); }

		window.draw(shapes[i]);
		text.setString(nodes[i]->data().first);
		text.setCharacterSize(15);
		text.setPosition(nodes[i]->getX() + 15, nodes[i]->getY());
		text.setColor(sf::Color::Blue);
		window.draw(text);

		text.setString(std::to_string(nodes[i]->getGValue()));
		text.setPosition(nodes[i]->getX(), nodes[i]->getY() + 15);
		window.draw(text);

		text.setColor(sf::Color::Red);
		text.setString(std::to_string(nodes[i]->getHValue()));
		text.setPosition(nodes[i]->getX(), nodes[i]->getY() + 30);
		window.draw(text);
	}
	window.draw(srtBtn);
	window.draw(resetBtn);
}

std::tuple<int,int,int> DrawLevel::CheckMouseClicks(sf::Event theEvent,sf::RenderWindow &window)
{
	if (!lMousePrevClicked)
	{
		if (theEvent.type == sf::Event::MouseButtonPressed && theEvent.mouseButton.button == sf::Mouse::Button::Left)
		{
			for (int i = 0; i < shapes.size(); i++)
			{
				if (shapes[i].getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
				{
					shapes[i].setFillColor(sf::Color::Cyan);
					start = i;
				}
				if (shapes[i].getFillColor() == sf::Color::Magenta || shapes[i].getFillColor() == sf::Color::Cyan)
				{
					int i = 0;
				
				}
				else { shapes[i].setFillColor(sf::Color::Green); }
			}
			if (srtBtn.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) && start != NULL && end != NULL)
			{
				return std::make_tuple(1, start, end);
			}
			else if (resetBtn.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
			{
				return std::make_tuple(2, NULL, NULL);
			}
			lMousePrevClicked = true;
		}
		
	}
	else if (!rMousePrevClicked)
	{
		if (theEvent.type == sf::Event::MouseButtonPressed && theEvent.mouseButton.button == sf::Mouse::Right)
		{
			for (int i = 0; i < shapes.size(); i++)
			{
				if (shapes[i].getGlobalBounds().contains(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y))
				{
					shapes[i].setFillColor(sf::Color::Magenta);
					end = i;
				}
				if (shapes[i].getFillColor() == sf::Color::Magenta || shapes[i].getFillColor() == sf::Color::Cyan)
				{
					int i = 0;

				}
				else { shapes[i].setFillColor(sf::Color::Green); }
			}

		}
	}
	else { lMousePrevClicked = false; rMousePrevClicked = false; }
	return std::make_tuple(0, NULL, NULL);
}