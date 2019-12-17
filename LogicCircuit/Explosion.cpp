#include "Explosion.h"


void Explosion::commenceIncident() 
{
	if (!commenceCalled)
	{
		commenceCalled = true;
		circle.setRadius(startingRadius);
		circle.setOrigin(sf::Vector2f(startingRadius, startingRadius));
		texture.loadFromFile("resources/Images/lightning3.png");
		circle.setTexture(&texture);
	}
	
	long ms = data->lifeTime.getElapsedTime().asMilliseconds();

	if (ms > downtime && ms < timeLimit)
	{
		rotate += 1.1;

		sf::Transform t;
		t.translate(data->pos.x, data->pos.y);
		t.scale(std::sin(ms / 10.f) * 400, std::sin((ms + 10) / 10.f) * 400);
		t.rotate(rotate);
		data->window->draw(circle, t);

		
	}
	else if (ms > timeLimit)
	{
		int xPos = rand() % (width / 2) - width / 4;
		int yPos = rand() % (height / 2) - height / 4;
		data->pos = sf::Vector2f(center.x + xPos, center.y + yPos);
		data->lifeTime.restart();
	}

}

